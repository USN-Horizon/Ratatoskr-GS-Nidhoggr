#include "packetparser.h"
#include <cmath>
#include <cstring>

PacketParser::PacketParser(QObject *parent) : QObject(parent)
{
    memset(&m_msg, 0, sizeof(m_msg));
    memset(&m_status, 0, sizeof(m_status));
}

void PacketParser::parse(const QByteArray &data)
{
    for (uint8_t byte : data) {
        if (mavlink_parse_char(MAVLINK_COMM_0, byte, &m_msg, &m_status) == MAVLINK_FRAMING_OK)
            handleMessage(m_msg);
    }
}

void PacketParser::handleMessage(const mavlink_message_t &msg)
{
    switch (msg.msgid) {
    case MAVLINK_MSG_ID_SCALED_IMU: {
        mavlink_scaled_imu_t imu;
        mavlink_msg_scaled_imu_decode(&msg, &imu);
        emit accelerationReceived(imu.xacc / 1000.0, imu.yacc / 1000.0, imu.zacc / 1000.0);
        emit rotationReceived(imu.xgyro / 1000.0, imu.ygyro / 1000.0, imu.zgyro / 1000.0);
        break;
    }
    case MAVLINK_MSG_ID_SCALED_PRESSURE: {
        mavlink_scaled_pressure_t pres;
        mavlink_msg_scaled_pressure_decode(&msg, &pres);
        emit pressureReceived(pres.press_abs);
        emit temperatureReceived(pres.temperature / 100.0);
        double alt = 44330.0 * (1.0 - pow(pres.press_abs / 1013.25, 0.1903));
        emit altitudeReceived(alt);
        break;
    }
    case MAVLINK_MSG_ID_VFR_HUD: {
        mavlink_vfr_hud_t hud;
        mavlink_msg_vfr_hud_decode(&msg, &hud);
        emit velocityReceived(hud.groundspeed);
        break;
    }
    case MAVLINK_MSG_ID_COSMIC_RADIATION: {
        mavlink_cosmic_radiation_t rad;
        mavlink_msg_cosmic_radiation_decode(&msg, &rad);
        emit radiationReceived(rad.radiation);
        break;
    }
    default:
        break;
    }
}
