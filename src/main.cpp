#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <models/timewindowproxymodel.h>
#include <models/flightstatemodel.h>
#include <utils/flightlogfactory.h>
#include "humiditycollection.h"
#include "missionmanager.h"
#include "timer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.addImportPath( "qrc:" );

    HumidityCollection* fmCollection = new HumidityCollection();

    FlightStateModel* stateModel = FlightLogFactory::createStateModel();

    FlightModels models = {
        FlightLogFactory::createModel("acceleration[m/s]"),
        FlightLogFactory::createModel("rotation[deg/s]"),
        FlightLogFactory::createModel("pressure[m/s]"),
        FlightLogFactory::createModel("altitude[m]"),
        FlightLogFactory::createModel("velocity[m/s]"),
        stateModel
    };

    MissionManager* missionManager = new MissionManager(models);

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
