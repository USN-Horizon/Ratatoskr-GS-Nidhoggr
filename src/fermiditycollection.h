#pragma once
#include <QObject>
#include <QtQml/qqml.h>
#include "testcontroller.h"
#include "models/humiditymodel.h"

class FermidityCollection : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(TestController* testController MEMBER m_testController CONSTANT )
    Q_PROPERTY(HumidityModel* humidityModel MEMBER m_humidityModel CONSTANT )

public:
    explicit FermidityCollection(QObject *parent = nullptr);

private:
    void registerControllers();
    TestController* m_testController { nullptr };
    HumidityModel* m_humidityModel { nullptr };
};
