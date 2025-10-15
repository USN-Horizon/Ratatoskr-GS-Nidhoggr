#pragma once
#include <QObject>
#include <QList>

struct TimestampedHumidity {
    qint64 timestamp;  // Relative timestamp in milliseconds since boot
    double humidity;   // Humidity value
};

namespace humidity_utils
{

using HumidityList = QList< TimestampedHumidity >;

auto parseCsv( const QString& a_fileName = R"(C:\Users\Hamsa\Downloads\Humidity_Data.csv)") -> HumidityList;
}


