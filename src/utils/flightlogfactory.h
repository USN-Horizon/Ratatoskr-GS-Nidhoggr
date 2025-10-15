#ifndef FLIGHTLOGFACTORY_H
#define FLIGHTLOGFACTORY_H

#include "models/timeseriesmodel.h"
#include "models/flightstatemodel.h"
#include <QString>

/**
 * @brief The FlightLogFactory class creates time series models from flight log data
 */
class FlightLogFactory
{
public:
    /**
     * @brief Create a time series model for altitude data
     * @param flightLogPath Path to the flight log CSV
     * @return A new TimeSeriesModel populated with altitude data
     */
    static TimeSeriesModel* createAltitudeModel(const QString& flightLogPath);

    /**
     * @brief Create a time series model for velocity data
     * @param flightLogPath Path to the flight log CSV
     * @return A new TimeSeriesModel populated with velocity data
     */
    static TimeSeriesModel* createVelocityModel(const QString& flightLogPath);

    /**
     * @brief Create a time series model for battery data
     * @param flightLogPath Path to the flight log CSV
     * @return A new TimeSeriesModel populated with battery data
     */
    static TimeSeriesModel* createBatteryModel(const QString& flightLogPath);

    /**
     * @brief Create a time series model for latitude data
     * @param flightLogPath Path to the flight log CSV
     * @return A new TimeSeriesModel populated with latitude data
     */
    static TimeSeriesModel* createLatitudeModel(const QString& flightLogPath);

    /**
     * @brief Create a time series model for longitude data
     * @param flightLogPath Path to the flight log CSV
     * @return A new TimeSeriesModel populated with longitude data
     */
    static TimeSeriesModel* createLongitudeModel(const QString& flightLogPath);

    /**
     * @brief Create a time series model for any column in the flight log
     * @param flightLogPath Path to the flight log CSV
     * @param columnName Name of the column to use for data
     * @return A new TimeSeriesModel populated with the specified data
     */
    static TimeSeriesModel* createModel(const QString& flightLogPath, const QString& columnName);

    /**
     * @brief Create a flight state model from flight log data
     * @param flightLogPath Path to the flight log CSV
     * @return A new FlightStateModel populated with state data
     */
    static FlightStateModel* createStateModel(const QString& flightLogPath);
};

#endif // FLIGHTLOGFACTORY_H
