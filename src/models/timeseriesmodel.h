#ifndef TIMESERIESMODEL_H
#define TIMESERIESMODEL_H

#include <QStandardItemModel>
#include <QHash>

/**
 * @brief The TimeSeriesModel class represents generic time-series data
 * where X is time in milliseconds and Y is any double value.
 */
class TimeSeriesModel : public QStandardItemModel
{
    Q_OBJECT

public:
    enum Columns {
        X_COLUMN = 0,
        Y_COLUMN = 1,
        COLUMN_COUNT = 2
    };

    enum Roles {
        XRole = Qt::UserRole + 1,
        YRole
    };

    /**
     * @brief Constructor with default parameters
     * @param parent The parent object
     */
    explicit TimeSeriesModel(QObject *parent = nullptr);

    /**
     * @brief Constructor with row and column count
     * @param rows Number of rows
     * @param columns Number of columns (should be 2)
     * @param parent The parent object
     */
    TimeSeriesModel(int rows, int columns, QObject *parent = nullptr);

    /**
     * @brief Override roleNames to provide custom roles for QML
     * @return Hash map of role names
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Append data to the model
     * @param x X value (time in milliseconds)
     * @param y Y value (any double data)
     */
    Q_INVOKABLE void appendData(qreal x, qreal y);

    /**
     * @brief Remove data at specified row
     * @param row Row index to remove
     */
    Q_INVOKABLE void removeData(int row);

    /**
     * @brief Get data as a QVariantMap for a specific row
     * @param row Row index
     * @return Map with "x" and "y" keys
     */
    Q_INVOKABLE QVariantMap get(int row) const;

    /**
     * @brief Set the column names
     * @param xLabel Label for X column
     * @param yLabel Label for Y column
     */
    void setColumnLabels(const QString& xLabel, const QString& yLabel);

    /**
     * @brief Set data from pairs of X and Y values
     * @param xValues List of X values
     * @param yValues List of Y values
     */
    void setData(const QVector<qreal>& xValues, const QVector<qreal>& yValues);

    /**
     * @brief Clear all data in the model
     */
    void clearData();
};

#endif // TIMESERIESMODEL_H
