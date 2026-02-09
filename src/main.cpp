#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <models/timewindowproxymodel.h>
#include <models/flightstatemodel.h>
#include <utils/flightlogfactory.h>
#include "humiditycollection.h"
#include "timer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.addImportPath( "qrc:" );

    HumidityCollection* fmCollection = new HumidityCollection();

    const QString& flightLogPath = "flightlog.csv"; // TODO: no hardie codie

    TimeSeriesModel* accelerationModel = FlightLogFactory::createModel(flightLogPath, "acceleration[m/s]");
    TimeSeriesModel* rotationModel = FlightLogFactory::createModel(flightLogPath, "rotation[deg/s]");
    TimeSeriesModel* pressureModel = FlightLogFactory::createModel(flightLogPath, "pressure[m/s]");
    TimeSeriesModel* altitudeModel = FlightLogFactory::createModel(flightLogPath, "altitude[m]");
    TimeSeriesModel* velocityModel = FlightLogFactory::createModel(flightLogPath, "velocity[m/s]");

    FlightStateModel* stateModel = FlightLogFactory::createStateModel(flightLogPath);

    qmlRegisterType<CountupTimer>("com.horizon.components", 1, 0, "CountupTimer");
    qmlRegisterType<TimeSeriesModel>("com.horizon.components", 1, 0, "AltitudeModel");
    qmlRegisterType<TimeWindowProxyModel>("com.horizon.components", 1, 0, "TimeWindowProxyModel");
    qmlRegisterType<HumidityCollection>("HumidityCollection", 1, 0, "HumidityCollection");
    qmlRegisterType<FlightStateModel>("com.horizon.components", 1, 0, "FlightStateModel");

    engine.rootContext()->setContextProperty( "fmc", fmCollection );

    engine.rootContext()->setContextProperty( "accelerationM", accelerationModel);
    engine.rootContext()->setContextProperty( "rotationM", rotationModel );
    engine.rootContext()->setContextProperty( "pressureM", pressureModel );
    engine.rootContext()->setContextProperty( "altitudeM", altitudeModel );
    engine.rootContext()->setContextProperty( "velocityM", velocityModel );

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
