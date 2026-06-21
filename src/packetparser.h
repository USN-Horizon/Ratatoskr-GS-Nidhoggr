#ifndef PACKETPARSER_H
#define PACKETPARSER_H
#pragma once
#include <QObject>
#include <QByteArray>
#include "mavlink/HorizonDialect/mavlink.h"

class PacketParser : public QObject
{
    Q_OBJECT
public:
    explicit PacketParser(QObject *parent = nullptr);
    void parse(const QByteArray &data);

signals:
    void altitudeReceived(double value);
    void velocityReceived(double value);
    void accelerationReceived(double x, double y, double z);
    void rotationReceived(double x, double y, double z);
    void pressureReceived(double value);
    void radiationReceived(double value);
    void temperatureReceived(double value);

private:
    mavlink_message_t m_msg;
    mavlink_status_t m_status;
    void handleMessage(const mavlink_message_t &msg);
};

#endif // PACKETPARSER_H
