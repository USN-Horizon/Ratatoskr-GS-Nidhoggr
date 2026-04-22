#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <models/timewindowproxymodel.h>
#include <models/flightstatemodel.h>
#include <utils/flightlogfactory.h>
#include "humiditycollection.h"
#include "timer.h"
#include "serialreader.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.addImportPath( "qrc:" );

    HumidityCollection* fmCollection = new HumidityCollection();

    const QString& flightLogPath = "flightlog.csv";
    TimeSeriesModel* altitudeModel = FlightLogFactory::createModel(flightLogPath, "altitude[m]");
    TimeSeriesModel* velocityModel = FlightLogFactory::createModel(flightLogPath, "velocity[m/s]");
    FlightStateModel* stateModel = FlightLogFactory::createStateModel(flightLogPath);

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

    engine.rootContext()->setContextProperty( "fmc", fmCollection );
    engine.rootContext()->setContextProperty( "altM", altitudeModel );
    engine.rootContext()->setContextProperty( "velM", velocityModel );
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
