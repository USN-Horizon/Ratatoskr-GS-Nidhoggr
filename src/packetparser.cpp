#include "packetparser.h"
#include <QDebug>
#include <cmath>

PacketParser::PacketParser(QObject *parent) : QObject(parent) {}

void PacketParser::parse(const QByteArray &packet)
{
    QString line = QString::fromUtf8(packet).trimmed();

    if (line.isEmpty() || line.startsWith("runId"))
        return;

    QStringList f = line.split(',');
    if (f.size() < 19) {
        qDebug() << "Bad packet (" << f.size() << "fields):" << line;
        return;
    }

    // Convert units
    double ax = f[2].toDouble() / 1000.0;   // mg -> g
    double ay = f[3].toDouble() / 1000.0;
    double az = f[4].toDouble() / 1000.0;

    double gx = f[5].toDouble() / 1000.0;   // mrad/s -> rad/s
    double gy = f[6].toDouble() / 1000.0;
    double gz = f[7].toDouble() / 1000.0;

    double pressure    = f[11].toDouble();
    double temperature = f[12].toDouble() / 100.0;  // centi-C -> C
    double geiger      = f[13].toDouble() + f[14].toDouble();

    // Derive altitude from pressure (barometric formula)
    double altitude = 44330.0 * (1.0 - pow(pressure / 1013.25, 0.1903));

    emit accelerationReceived(ax, ay, az);
    emit rotationReceived(gx, gy, gz);
    emit pressureReceived(pressure);
    emit altitudeReceived(altitude);
    emit radiationReceived(geiger);
    emit temperatureReceived(temperature);
}
