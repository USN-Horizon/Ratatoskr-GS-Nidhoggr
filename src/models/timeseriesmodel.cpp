#include "timeseriesmodel.h"
#include <QStandardItem>

TimeSeriesModel::TimeSeriesModel(QObject *parent)
    : QStandardItemModel(0, COLUMN_COUNT, parent)
{
    // Set default horizontal header labels
    setHorizontalHeaderLabels({"Time (ms)", "Value"});
}

TimeSeriesModel::TimeSeriesModel(int rows, int columns, QObject *parent)
    : QStandardItemModel(rows, columns, parent)
{
    // Set default horizontal header labels
    setHorizontalHeaderLabels({"Time (ms)", "Value"});
}

QHash<int, QByteArray> TimeSeriesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QStandardItemModel::roleNames();
    roles[XRole] = "x";
    roles[YRole] = "y";
    return roles;
}

void TimeSeriesModel::appendData(qreal x, qreal y)
{
    int row = rowCount();

    // Create items for new row
    QStandardItem *xItem = new QStandardItem();
    xItem->setData(x, Qt::DisplayRole);
    xItem->setData(x, XRole);

    QStandardItem *yItem = new QStandardItem();
    yItem->setData(y, Qt::DisplayRole);
    yItem->setData(y, YRole);

    // Create a row with the items
    QList<QStandardItem*> rowItems;
    rowItems << xItem << yItem;

    // Append the row
    appendRow(rowItems);
}

void TimeSeriesModel::removeData(int row)
{
    if (row >= 0 && row < rowCount()) {
        removeRow(row);
    }
}

QVariantMap TimeSeriesModel::get(int row) const
{
    QVariantMap map;
    if (row >= 0 && row < rowCount()) {
        QModelIndex xIndex = index(row, X_COLUMN);
        QModelIndex yIndex = index(row, Y_COLUMN);
        map["x"] = data(xIndex, Qt::DisplayRole).toReal();
        map["y"] = data(yIndex, Qt::DisplayRole).toReal();
    }
    return map;
}

void TimeSeriesModel::setColumnLabels(const QString& xLabel, const QString& yLabel)
{
    setHorizontalHeaderLabels({xLabel, yLabel});
}

void TimeSeriesModel::setData(const QVector<qreal>& xValues, const QVector<qreal>& yValues)
{
    // Clear existing data
    clearData();

    // Make sure both vectors have the same size
    int count = qMin(xValues.size(), yValues.size());

    // Reserve rows for better performance
    setRowCount(count);

    // Add data from the vectors
    for (int i = 0; i < count; ++i) {
        qreal x = xValues.at(i);
        qreal y = yValues.at(i);

        // Create and set items for X and Y columns
        QStandardItem *xItem = new QStandardItem();
        xItem->setData(x, Qt::DisplayRole);
        xItem->setData(x, XRole);

        QStandardItem *yItem = new QStandardItem();
        yItem->setData(y, Qt::DisplayRole);
        yItem->setData(y, YRole);

        setItem(i, X_COLUMN, xItem);
        setItem(i, Y_COLUMN, yItem);
    }
}

void TimeSeriesModel::clearData()
{
    removeRows(0, rowCount());
}
