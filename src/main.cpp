#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "fermiditycollection.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.addImportPath( "qrc:" );

    FermidityCollection* fmCollection = new FermidityCollection();


    qmlRegisterType<FermidityCollection>("FermidityCollection", 1, 0, "FermidityCollection");

    engine.rootContext()->setContextProperty( "fmc", fmCollection );

    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
