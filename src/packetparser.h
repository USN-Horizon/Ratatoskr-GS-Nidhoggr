#ifndef PACKETPARSER_H
#define PACKETPARSER_H
#pragma once
#include <QObject>
#include <QByteArray>

class PacketParser : public QObject
{
    Q_OBJECT
public:
    explicit PacketParser(QObject *parent = nullptr);
    void parse(const QByteArray &packet);

signals:
    void altitudeReceived(double value);
    void velocityReceived(double value);
    void accelerationReceived(double x, double y, double z);
    void rotationReceived(double x, double y, double z);
    void pressureReceived(double value);
    void radiationReceived(double value);
    void temperatureReceived(double value);
};

#endif // PACKETPARSER_H
