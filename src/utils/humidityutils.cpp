#include "humidityutils.h"
#include <QDebug>
#include <QFile>
#include <QtextStream>
#include <QString>

humidity_utils::HumidityList humidity_utils::parseCsv( const QString& a_fileName )
{
    HumidityList result;
    QFile file( a_fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file:" << file.errorString();
        return {};
    }

    QTextStream in(&file);

    for (int i = 0; !in.atEnd(); i++) {
        auto measurmentString = in.readLine();
        if(!measurmentString.isEmpty() && measurmentString != "Humidity"){
            result.append(measurmentString.toDouble());
        }
    }

    file.close();
    return result;
}
