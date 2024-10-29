#pragma once
#include <QObject>
#include <QList>

namespace humidity_utils
{
using HumidityList = QList< qreal >;

auto parseCsv( const QString& a_fileName = R"(C:\Users\Hamsa\Downloads\Humidity_Data.csv)") -> HumidityList;
}


