#!/usr/bin/env python3
"""
Standalone MAVLink v2 serial emulator for the Horizon Ground Station.

Creates a virtual serial port (a PTY) and streams MAVLink v2 frames that match
exactly what the ground station parses:

    SCALED_IMU       (26)    -> acceleration graph
    SCALED_IMU2      (116)   -> rotation graph
    SCALED_PRESSURE  (29)    -> pressure / temperature / altitude
    VFR_HUD          (74)    -> velocity
    NAMED_VALUE_INT  (252)   -> "BARO_T" temperature
    COSMIC_RADIATION (16002) -> radiation (Horizon custom dialect)
    HEARTBEAT        (0)
    FLIGHT_STATES    (16000) -> flight phase (Horizon custom dialect)
    PAYLOAD_TEMPERATURE(16001)-> stack temps (Horizon custom dialect)

No external dependencies (no pymavlink) - the v2 framing and X25 CRC are
implemented inline, and the per-message CRC_EXTRA values are taken straight
from HorizonDialect.h.

Usage:
    ./mavlink_emulator.py                  # create a PTY, print its path, stream
    ./mavlink_emulator.py --port /dev/ttyX # write to an existing device instead
    ./mavlink_emulator.py --hz 50          # set base update rate (default 50)
    ./mavlink_emulator.py --no-flight      # steady idle values instead of a flight arc

Point the ground station at the printed /dev/pts/N device (see README.md).
"""

import argparse
import math
import os
import struct
import sys
import time

# ---------------------------------------------------------------------------
# MAVLink v2 framing
# ---------------------------------------------------------------------------

STX_V2 = 0xFD
SYSID = 1
COMPID = 1

# msgid -> CRC_EXTRA, copied from MAVLINK_MESSAGE_CRCS in HorizonDialect.h
CRC_EXTRA = {
    0: 50,       # HEARTBEAT
    26: 170,     # SCALED_IMU
    29: 115,     # SCALED_PRESSURE
    74: 20,      # VFR_HUD
    116: 76,     # SCALED_IMU2
    252: 44,     # NAMED_VALUE_INT
    16000: 171,  # FLIGHT_STATES
    16001: 46,   # PAYLOAD_TEMPERATURE
    16002: 134,  # COSMIC_RADIATION
}


def x25_crc(data: bytes) -> int:
    """MAVLink X25/MCRF4XX checksum."""
    crc = 0xFFFF
    for b in data:
        tmp = (b ^ (crc & 0xFF)) & 0xFF
        tmp = (tmp ^ (tmp << 4)) & 0xFF
        crc = ((crc >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4)) & 0xFFFF
    return crc


class Framer:
    def __init__(self):
        self.seq = 0

    def frame(self, msgid: int, payload: bytes) -> bytes:
        # v2 payload truncation: drop trailing zero bytes (keep at least 1)
        p = payload
        while len(p) > 1 and p[-1] == 0:
            p = p[:-1]
        length = len(p)

        header = struct.pack(
            "<BBBBBB",
            length,        # payload length
            0,             # incompat_flags
            0,             # compat_flags
            self.seq,      # sequence
            SYSID,
            COMPID,
        ) + struct.pack("<I", msgid)[:3]  # 24-bit msgid, little-endian

        crc = x25_crc(header + p + bytes([CRC_EXTRA[msgid]]))
        self.seq = (self.seq + 1) & 0xFF
        return bytes([STX_V2]) + header + p + struct.pack("<H", crc)


# ---------------------------------------------------------------------------
# Message packers (field order = MAVLink wire order)
# ---------------------------------------------------------------------------

def msg_heartbeat() -> (int, bytes):
    # custom_mode u32, type u8, autopilot u8, base_mode u8, system_status u8, mavlink_version u8
    payload = struct.pack("<IBBBBB", 0, 13, 0, 0, 4, 3)  # type=13 ROCKET-ish, MAV_STATE_ACTIVE
    return 0, payload


def msg_scaled_imu(t_ms, ax, ay, az):
    # time_boot_ms u32, then xacc..zmag int16 (acc/gyro/mag); GS reads acc (milli-g)
    return 26, struct.pack(
        "<Ihhhhhhhhh", t_ms,
        int(ax), int(ay), int(az),  # acc (mg)
        0, 0, 0,                    # gyro
        0, 0, 0,                    # mag
    )


def msg_scaled_imu2(t_ms, gx, gy, gz):
    # GS reads gyro fields (mrad/s)
    return 116, struct.pack(
        "<Ihhhhhhhhh", t_ms,
        0, 0, 0,
        int(gx), int(gy), int(gz),
        0, 0, 0,
    )


def msg_scaled_pressure(t_ms, press_abs_hpa, temp_c):
    # time_boot_ms u32, press_abs f32, press_diff f32, temperature int16 (centi-degC)
    return 29, struct.pack(
        "<Iffh", t_ms, float(press_abs_hpa), 0.0, int(temp_c * 100),
    )


def msg_vfr_hud(groundspeed, alt, climb):
    # airspeed f, groundspeed f, heading i16, throttle u16, alt f, climb f
    return 74, struct.pack(
        "<ffhHff", float(groundspeed), float(groundspeed), 0, 0, float(alt), float(climb),
    )


def msg_named_value_int(t_ms, name: str, value: int):
    # time_boot_ms u32, value i32, name char[10]
    name_b = name.encode("ascii")[:10].ljust(10, b"\x00")
    return 252, struct.pack("<Ii", t_ms, int(value)) + name_b


def msg_cosmic_radiation(t_us, radiation):
    # time_usec u64, radiation u16
    return 16002, struct.pack("<QH", t_us, int(radiation) & 0xFFFF)


def msg_flight_states(t_us, phase):
    # time_usec u64, phase u8
    return 16000, struct.pack("<QB", t_us, int(phase) & 0xFF)


def msg_payload_temperature(t_us, top, mid, bottom):
    # time_usec u64, top f32, mid f32, bottom f32
    return 16001, struct.pack("<Qfff", t_us, float(top), float(mid), float(bottom))


# ---------------------------------------------------------------------------
# Flight simulation
# ---------------------------------------------------------------------------

# ROCKET_STATE_TYPE from HorizonDialect.h
IDLE, CALIBRATED, THRUSTING, COASTING, DROGUE, MAIN, TOUCHDOWN = range(7)


def flight_state(t):
    """Return (phase, altitude_m, vert_speed_m_s, accel_g) for elapsed seconds t.

    A simple, readable rocket arc: boost, coast to apogee, drogue, main, land.
    """
    if t < 3:
        return CALIBRATED, 0.0, 0.0, 1.0
    t -= 3
    if t < 4:                       # thrust
        a = 8.0
        v = a * 9.81 * t
        alt = 0.5 * a * 9.81 * t * t
        return THRUSTING, alt, v, a + 1
    v0 = 8.0 * 9.81 * 4
    alt0 = 0.5 * 8.0 * 9.81 * 16
    t -= 4
    t_coast = v0 / 9.81
    if t < t_coast:                 # coast to apogee
        v = v0 - 9.81 * t
        alt = alt0 + v0 * t - 0.5 * 9.81 * t * t
        return COASTING, alt, v, 0.0
    apogee = alt0 + v0 * t_coast - 0.5 * 9.81 * t_coast * t_coast
    t -= t_coast
    if t < 8:                       # drogue descent (fast)
        v = -30.0
        alt = max(0.0, apogee + v * t)
        return DROGUE, alt, v, 1.0
    alt_drogue = max(0.0, apogee + (-30.0) * 8)
    t -= 8
    v = -6.0                        # main descent (slow)
    alt = alt_drogue + v * t
    if alt <= 0:
        return TOUCHDOWN, 0.0, 0.0, 1.0
    return MAIN, alt, v, 1.0


def alt_to_pressure_hpa(alt_m):
    return 1013.25 * (1.0 - alt_m / 44330.0) ** (1.0 / 0.1903)


# ---------------------------------------------------------------------------
# Main loop
# ---------------------------------------------------------------------------

def open_output(port):
    if port:
        fd = os.open(port, os.O_WRONLY | os.O_NOCTTY)
        return fd, port

    # Create a PTY; the slave path is what the consumer opens.
    master, slave = os.openpty()
    slave_name = os.ttyname(slave)
    # Make the slave look like a raw serial line.
    try:
        import termios
        attrs = termios.tcgetattr(slave)
        attrs[0] = attrs[1] = attrs[3] = 0  # iflag, oflag, lflag
        termios.tcsetattr(slave, termios.TCSANOW, attrs)
    except Exception:
        pass
    return master, slave_name


def main():
    ap = argparse.ArgumentParser(description="MAVLink v2 serial emulator for Horizon GS")
    ap.add_argument("--port", help="write to an existing device instead of creating a PTY")
    ap.add_argument("--hz", type=float, default=50.0, help="base update rate (default 50)")
    ap.add_argument("--no-flight", action="store_true", help="stream steady idle values")
    args = ap.parse_args()

    fd, name = open_output(args.port)
    framer = Framer()

    print(f"[emulator] streaming MAVLink v2 on: {name}")
    if not args.port:
        print(f"[emulator] point the ground station at this device: {name}")
    print(f"[emulator] rate={args.hz} Hz  (Ctrl-C to stop)")

    period = 1.0 / args.hz
    t0 = time.time()
    tick = 0

    try:
        while True:
            now = time.time()
            elapsed = now - t0
            t_ms = int(elapsed * 1000) & 0xFFFFFFFF
            t_us = int(elapsed * 1e6)

            if args.no_flight:
                phase, alt, vspeed, accel_g = IDLE, 0.0, 0.0, 1.0
            else:
                phase, alt, vspeed, accel_g = flight_state(elapsed)

            out = bytearray()

            # High-rate: IMU + pressure + speed every tick
            wobble = math.sin(elapsed * 5.0)
            out += framer.frame(*msg_scaled_imu(
                t_ms, 50 * wobble, 50 * math.cos(elapsed * 5.0), accel_g * 1000))
            out += framer.frame(*msg_scaled_imu2(
                t_ms, 200 * wobble, 150 * math.cos(elapsed * 3.0), 80 * wobble))
            press = alt_to_pressure_hpa(alt)
            temp = 22.0 - alt * 0.0065        # rough lapse rate
            out += framer.frame(*msg_scaled_pressure(t_ms, press, temp))
            out += framer.frame(*msg_vfr_hud(abs(vspeed), alt, vspeed))

            # 5 Hz: temperature as NAMED_VALUE_INT "BARO_T", payload temps
            if tick % 10 == 0:
                out += framer.frame(*msg_named_value_int(t_ms, "BARO_T", int(temp)))
                out += framer.frame(*msg_payload_temperature(
                    t_us, temp + 1, temp, temp - 1))

            # 2 Hz: cosmic radiation + flight state
            if tick % 25 == 0:
                rad = 50 + int(20 * (1 + math.sin(elapsed)))
                out += framer.frame(*msg_cosmic_radiation(t_us, rad))
                out += framer.frame(*msg_flight_states(t_us, phase))

            # 1 Hz: heartbeat
            if tick % int(args.hz) == 0:
                out += framer.frame(*msg_heartbeat())

            os.write(fd, bytes(out))

            tick += 1
            # keep cadence
            sleep_for = period - (time.time() - now)
            if sleep_for > 0:
                time.sleep(sleep_for)
    except KeyboardInterrupt:
        print("\n[emulator] stopped")
    finally:
        os.close(fd)


if __name__ == "__main__":
    main()
