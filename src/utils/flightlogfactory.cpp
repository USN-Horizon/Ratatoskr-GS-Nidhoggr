#include "utils/flightlogfactory.h"
#include "utils/timeseriesfactory.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

TimeSeriesModel* FlightLogFactory::createAltitudeModel(const QString& flightLogPath)
{
    TimeSeriesModel* model = TimeSeriesFactory::fromCsv(
        flightLogPath,
        "seconds",
        "altitude[m]",
        1000.0  // Convert seconds to milliseconds
        );

    // Set appropriate column labels
    model->setColumnLabels("Time (ms)", "Altitude (m)");
    return model;
}

TimeSeriesModel* FlightLogFactory::createVelocityModel(const QString& flightLogPath)
{
    TimeSeriesModel* model = TimeSeriesFactory::fromCsv(
        flightLogPath,
        "seconds",
        "velocity[m/s]",
        1000.0  // Convert seconds to milliseconds
        );

    // Set appropriate column labels
    model->setColumnLabels("Time (ms)", "Velocity (m/s)");
    return model;
}

TimeSeriesModel* FlightLogFactory::createBatteryModel(const QString& flightLogPath)
{
    // Use custom processing to convert decivolts to volts
    QVector<qreal> xValues;
    QVector<qreal> yValues;

    TimeSeriesFactory::processCsv(
        flightLogPath,
        [](const QStringList& fields, int lineNumber, QVector<qreal>& xValues, QVector<qreal>& yValues) {
            if (fields.size() >= 8) {
                bool okTime, okBattery;
                qreal timeValue = fields[0].toDouble(&okTime);  // seconds column
                qreal batteryValue = fields[7].toDouble(&okBattery);  // battery[decivolts] column

                if (okTime && okBattery) {
                    // Convert seconds to milliseconds and decivolts to volts
                    xValues.append(timeValue * 1000.0);
                    yValues.append(batteryValue / 10.0); // Convert decivolts to volts
                }
            }
        },
        xValues,
        yValues
        );

    TimeSeriesModel* model = new TimeSeriesModel();
    model->setData(xValues, yValues);
    model->setColumnLabels("Time (ms)", "Battery (V)");
    return model;
}

TimeSeriesModel* FlightLogFactory::createLatitudeModel(const QString& flightLogPath)
{
    // Use custom processing to convert deg/10000 to degrees
    QVector<qreal> xValues;
    QVector<qreal> yValues;

    TimeSeriesFactory::processCsv(
        flightLogPath,
        [](const QStringList& fields, int lineNumber, QVector<qreal>& xValues, QVector<qreal>& yValues) {
            if (fields.size() >= 5) {
                bool okTime, okLat;
                qreal timeValue = fields[0].toDouble(&okTime);  // seconds column
                qreal latValue = fields[3].toDouble(&okLat);  // lat[deg/10000] column

                if (okTime && okLat) {
                    // Convert seconds to milliseconds and deg/10000 to degrees
                    xValues.append(timeValue * 1000.0);
                    yValues.append(latValue / 10000.0); // Convert to degrees
                }
            }
        },
        xValues,
        yValues
        );

    TimeSeriesModel* model = new TimeSeriesModel();
    model->setData(xValues, yValues);
    model->setColumnLabels("Time (ms)", "Latitude (deg)");
    return model;
}

TimeSeriesModel* FlightLogFactory::createLongitudeModel(const QString& flightLogPath)
{
    // Use custom processing to convert deg/10000 to degrees
    QVector<qreal> xValues;
    QVector<qreal> yValues;

    TimeSeriesFactory::processCsv(
        flightLogPath,
        [](const QStringList& fields, int lineNumber, QVector<qreal>& xValues, QVector<qreal>& yValues) {
            if (fields.size() >= 6) {
                bool okTime, okLon;
                qreal timeValue = fields[0].toDouble(&okTime);  // seconds column
                qreal lonValue = fields[4].toDouble(&okLon);  // lon[deg/10000] column

                if (okTime && okLon) {
                    // Convert seconds to milliseconds and deg/10000 to degrees
                    xValues.append(timeValue * 1000.0);
                    yValues.append(lonValue / 10000.0); // Convert to degrees
                }
            }
        },
        xValues,
        yValues
        );

    TimeSeriesModel* model = new TimeSeriesModel();
    model->setData(xValues, yValues);
    model->setColumnLabels("Time (ms)", "Longitude (deg)");
    return model;
}

/**
 * @brief Create a flight state model from flight log data
 * @param flightLogPath Path to the flight log CSV
 * @return A new FlightStateModel populated with state data
 */
FlightStateModel* FlightLogFactory::createStateModel(const QString& flightLogPath)
{
    // Create a new model
    FlightStateModel* model = new FlightStateModel();

    // Load data from the flight log
    QVector<qreal> timeValues;
    QVector<int> stateValues;

    QFile file(flightLogPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file:" << file.errorString();
        return model;
    }

    QTextStream in(&file);
    bool hasHeader = true;
    int lineNumber = 0;

    // Find the state column index (typically 1 based on your CSV sample)
    int stateColumnIndex = 1;
    int timeColumnIndex = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) {
            continue;
        }

        // Skip header row
        if (hasHeader && lineNumber == 0) {
            lineNumber++;
            continue;
        }

        // Parse the line with European number format handling
        // First, split by commas but be careful with quoted values
        QStringList fields;
        bool inQuotes = false;
        QString currentField;

        for (int i = 0; i < line.size(); i++) {
            QChar c = line.at(i);

            if (c == '\"') {
                inQuotes = !inQuotes;
            } else if (c == ',' && !inQuotes) {
                fields.append(currentField);
                currentField.clear();
            } else {
                currentField.append(c);
            }
        }
        // Add the last field
        fields.append(currentField);

        // Ensure we have both time and state columns
        if (fields.size() > qMax(timeColumnIndex, stateColumnIndex)) {
            bool okTime, okState;

            // Parse time value
            QString timeStr = fields[timeColumnIndex];
            timeStr.remove('\"');
            timeStr.replace(',', '.');
            qreal timeValue = timeStr.toDouble(&okTime);

            // Parse state value
            QString stateStr = fields[stateColumnIndex];
            stateStr.remove('\"');
            int stateValue = stateStr.toInt(&okState);

            if (okTime && okState) {
                // Convert seconds to milliseconds
                timeValues.append(timeValue * 1000.0);
                stateValues.append(stateValue);
                qDebug() << timeValue * 1000.0 << ", " << stateValue;
            } else {
                if (!okTime) {
                    qDebug() << "Problem reading time value:" << fields[timeColumnIndex];
                }
                if (!okState) {
                    qDebug() << "Problem reading state value:" << fields[stateColumnIndex];
                }
            }
        }

        lineNumber++;
    }

    file.close();

    // Update the model with state data
    model->setStateData(timeValues, stateValues);

    qDebug() << "Loaded state model with" << model->rowCount() << "data points";

    return model;
}

TimeSeriesModel* FlightLogFactory::createModel(const QString& flightLogPath, const QString& columnName)
{
    // Handle special cases
    if (columnName == "altitude[m]") {
        return createAltitudeModel(flightLogPath);
    } else if (columnName == "velocity[m/s]") {
        return createVelocityModel(flightLogPath);
    } else if (columnName == "battery[decivolts]") {
        return createBatteryModel(flightLogPath);
    } else if (columnName == "lat[deg/10000]") {
        return createLatitudeModel(flightLogPath);
    } else if (columnName == "lon[deg/10000]") {
        return createLongitudeModel(flightLogPath);
    }

    // Default case: just use the column as-is
    return TimeSeriesFactory::fromCsv(
        flightLogPath,
        "seconds",
        columnName,
        1000.0  // Convert seconds to milliseconds
        );
}
