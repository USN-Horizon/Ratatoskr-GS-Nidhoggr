#include "serialreader.h"
#include <QDebug>

SerialReader::SerialReader(QObject *parent)
    : QObject(parent)
    , serialPort(new QSerialPort(this))
{
    connect(serialPort, &QSerialPort::readyRead,
            this, &SerialReader::handleReadyRead);
    connect(serialPort, &QSerialPort::errorOccurred,
            this, &SerialReader::handleError);
}

SerialReader::~SerialReader()
{
    closePort();
}

bool SerialReader::openPort(const QString &portName, qint32 baudRate)
{
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadOnly)) {
        qDebug() << "Serial port opened:" << portName << "at" << baudRate << "baud";
        byteBuffer.clear();
        return true;
    } else {
        emit errorOccurred("Failed to open port: " + serialPort->errorString());
        return false;
    }
}

void SerialReader::closePort()
{
    if (serialPort->isOpen()) {
        serialPort->close();
        qDebug() << "Serial port closed";
    }
}

bool SerialReader::isOpen() const
{
    return serialPort->isOpen();
}

QStringList SerialReader::availablePorts()
{
    QStringList ports;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        ports << info.portName() + " (" + info.description() + ")";
    }
    return ports;
}

void SerialReader::handleReadyRead()
{
    // Append new data to buffer
    byteBuffer.append(serialPort->readAll());

    // Process any complete packets in buffer
    processBuffer();
}

void SerialReader::processBuffer()
{
    while (true)
    {
        int newlineIndex = byteBuffer.indexOf('\n');
        if (newlineIndex == -1) break;

        QByteArray packet = byteBuffer.left(newlineIndex + 1);
        byteBuffer.remove(0, newlineIndex + 1);
        emit rawPacketReceived(packet);
    }
}

void SerialReader::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError && error != QSerialPort::TimeoutError) {
        emit errorOccurred(serialPort->errorString());
        qDebug() << "Serial error:" << serialPort->errorString();
    }
}
