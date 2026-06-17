#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <models/timewindowproxymodel.h>
#include <models/flightstatemodel.h>
#include <utils/flightlogfactory.h>
#include <QDateTime>
#include <cmath>
#include <QElapsedTimer>
#include "humiditycollection.h"
#include "missionmanager.h"
#include "timer.h"
#include "serialreader.h"
#include "packetparser.h"
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

    //Serial reader & Parse packer
    SerialReader* serialReader = new SerialReader(&app);
    PacketParser* parser = new PacketParser(&app);

    QElapsedTimer* elapsedTimer = new QElapsedTimer();
    elapsedTimer->start();

    bool* timerReset = new bool(false);

    QObject::connect(serialReader, &SerialReader::rawPacketReceived,
                     parser, &PacketParser::parse);

    QObject::connect(parser, &PacketParser::altitudeReceived,
                     [models, elapsedTimer, timerReset](double value) {
                         if (!*timerReset) {
                             elapsedTimer->restart();
                             *timerReset = true;
                         }
                         models.altitude->appendData(elapsedTimer->elapsed(), value);
                     });
    QObject::connect(parser, &PacketParser::velocityReceived,
                     [models, elapsedTimer](double value) {
                         models.velocity->appendData(elapsedTimer->elapsed(), value);
                     });
    QObject::connect(parser, &PacketParser::pressureReceived,
                     [models, elapsedTimer](double value) {
                         models.pressure->appendData(elapsedTimer->elapsed(), value);
                     });
    QObject::connect(parser, &PacketParser::accelerationReceived,
                     [models, elapsedTimer](double x, double y, double z) {
                         qreal magnitude = std::sqrt(x*x + y*y + z*z);
                         models.acceleration->appendData(elapsedTimer->elapsed(), magnitude);
                     });
    QObject::connect(parser, &PacketParser::rotationReceived,
                     [models, elapsedTimer](double x, double y, double z) {
                         qreal magnitude = std::sqrt(x*x + y*y + z*z);
                         models.rotation->appendData(elapsedTimer->elapsed(), magnitude);
                     });
    QObject::connect(parser, &PacketParser::radiationReceived,
                     [models, elapsedTimer](double value) {
                         models.radiation->appendData(elapsedTimer->elapsed(), value);
                     });

    QObject::connect(serialReader, &SerialReader::errorOccurred,
                     [](const QString &error) {
                         qDebug() << "Serial error:" << error;
                     });

    engine.rootContext()->setContextProperty("serialReader", serialReader);

    const QString Port = "COM5";
    if (serialReader->openPort(Port, 115200)) {
        qDebug() << "Listening on" << Port;
    } else {
        qDebug() << "Could not open" << Port;
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

    QObject* timerStarter = new QObject(&app);
    QObject::connect(parser, &PacketParser::altitudeReceived,
                     timerStarter, [&engine, timerStarter]() {
                         QObject* root = engine.rootObjects().isEmpty() ? nullptr : engine.rootObjects().first();
                         if (!root) return;
                         QObject* topBarObj = root->findChild<QObject*>("topBar");
                         if (!topBarObj) return;
                         QObject* timer = topBarObj->property("missionTimer").value<QObject*>();
                         if (!timer) return;
                         QMetaObject::invokeMethod(timer, "resume");

                         // Only disconnect this specific connection by deleting the context object
                         timerStarter->deleteLater();
                     });

    return app.exec();
}
