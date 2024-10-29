#include "fermiditycollection.h"

#include "utils/humidityutils.h"


FermidityCollection::FermidityCollection(QObject *parent)
    : QObject{parent}
    , m_testController { new TestController( this ) }
    , m_humidityModel { new HumidityModel( this ) }
{
    registerControllers();

    const auto humidityList = humidity_utils::parseCsv();
    m_testController->setHumidityList( humidityList );
    m_humidityModel->setHumidityList( humidityList );
}

void FermidityCollection::registerControllers()
{
    qmlRegisterType<TestController>("FmControllers", 1, 0, "TestController");
    qmlRegisterType<HumidityModel>("FmControllers", 1, 0, "HumidityModel");
}

