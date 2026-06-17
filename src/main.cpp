#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <models/timewindowproxymodel.h>
#include <models/flightstatemodel.h>
#include <utils/flightlogfactory.h>
#include "humiditycollection.h"
#include "missionmanager.h"
#include "timer.h"
#include "serialreader.h"
#include "mavlink/HorizonDialect/HorizonDialect.hpp"

using namespace mavlink;

void decode_message(const mavlink_message_t* msg) {
    mavlink::MsgMap map(msg);

    if (msg->msgid == mavlink::HorizonDialect::msg::COSMIC_RADIATION::MSG_ID) {
        mavlink::HorizonDialect::msg::COSMIC_RADIATION rad;
        rad.deserialize(map);

        qDebug() << "Radiation " << rad.radiation;
    }
}

void process_byte_buffer(const uint8_t* buffer, size_t length) {
    mavlink_message_t msg;
    mavlink_status_t status;

    for(size_t i = 0; i < length; i++) {
        uint8_t byte = buffer[i];

        if (mavlink_frame_char(MAVLINK_COMM_0, byte, &msg, &status) == MAVLINK_FRAMING_OK) {
            qDebug() << "Got message ID " << msg.msgid << " length " << msg.len;
            decode_message(&msg);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.addImportPath( "qrc:" );

    HumidityCollection* fmCollection = new HumidityCollection();

    FlightStateModel* stateModel = FlightLogFactory::createStateModel();
    LocationModel* locationModel = FlightLogFactory::createLocationModel();

    FlightModels models = {
        FlightLogFactory::createModel("acceleration[m/s]"),
        FlightLogFactory::createModel("rotation[deg/s]"),
        FlightLogFactory::createModel("pressure[m/s]"),
        FlightLogFactory::createModel("altitude[m]"),
        FlightLogFactory::createModel("velocity[m/s]"),
        FlightLogFactory::createModel("cosmic_radiation[c/m]"),
        locationModel,
        stateModel
    };

    MissionManager* missionManager = new MissionManager(models);

    //Serial reader
    SerialReader* serialReader = new SerialReader(&app);

    // Debug: print every raw packet received
    QObject::connect(serialReader, &SerialReader::rawPacketReceived,
                     [](const QByteArray &packet) {
                         qDebug() << "Packet received:" << packet.trimmed();
                     });

    // Debug: print any serial errors
    QObject::connect(serialReader, &SerialReader::errorOccurred,
                     [](const QString &error) {
                         qDebug() << "Serial error:" << error;
                     });

    // Expose SerialReader to QML so you can call openPort()
    engine.rootContext()->setContextProperty("serialReader", serialReader);

    const QString Port = "COM5";
    if (serialReader->openPort(Port, 115200)) {
        qDebug() << "Listening for connection on" << Port;
    } else {
        qDebug() << "Could not open" << Port << "- check the port name";
    }

    qmlRegisterType<CountupTimer>("com.horizon.components", 1, 0, "CountupTimer");
    qmlRegisterType<TimeSeriesModel>("com.horizon.components", 1, 0, "AltitudeModel");
    qmlRegisterType<TimeWindowProxyModel>("com.horizon.components", 1, 0, "TimeWindowProxyModel");
    qmlRegisterType<HumidityCollection>("HumidityCollection", 1, 0, "HumidityCollection");
    qmlRegisterType<FlightStateModel>("com.horizon.components", 1, 0, "FlightStateModel");


    engine.rootContext()->setContextProperty("missionManager", missionManager );

    engine.rootContext()->setContextProperty( "fmc", fmCollection );

    engine.rootContext()->setContextProperty( "accelerationM", models.acceleration);
    engine.rootContext()->setContextProperty( "rotationM", models.rotation);
    engine.rootContext()->setContextProperty( "pressureM", models.pressure);
    engine.rootContext()->setContextProperty( "altitudeM", models.altitude);
    engine.rootContext()->setContextProperty( "velocityM", models.velocity);
    engine.rootContext()->setContextProperty( "radiationM", models.radiation);

    engine.rootContext()->setContextProperty("locationM", locationModel);
    engine.rootContext()->setContextProperty( "stateM", stateModel );

    // Connect to state transitions to log them
    QObject::connect(stateModel, &FlightStateModel::stateTransition,
                     [](FlightState fromState, FlightState toState, FlightEvent event) {
                         qDebug() << "State transition:"
                                  << FlightStateUtils::getStateName(fromState)
                                  << "->"
                                  << FlightStateUtils::getStateName(toState)
                                  << "triggered by"
                                  << FlightStateUtils::getEventName(event);
                     });

    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
