#include "testcontroller.h"

TestController::TestController(QObject *parent)
    : QObject{parent}
    , m_name { "Hello World!" }
{}

QString TestController::name() const
{
    return m_name;
}

void TestController::setname(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

humidity_utils::HumidityList TestController::humidityList() const
{
    return m_humidityList;
}

void TestController::setHumidityList(const humidity_utils::HumidityList &newHumidityList)
{
    if (m_humidityList == newHumidityList)
        return;
    m_humidityList = newHumidityList;
    emit humidityListChanged();
}
