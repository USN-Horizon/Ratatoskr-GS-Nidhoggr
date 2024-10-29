#include "humiditymodel.h"
#include <QVariant>

HumidityModel::HumidityModel(QObject *parent) : QAbstractListModel(parent) {
    // Example data
    dataPoints.append({0, 1});
    dataPoints.append({1, 3});
    dataPoints.append({2, 2});
}

int HumidityModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return dataPoints.size();
}

QVariant HumidityModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= dataPoints.size())
        return QVariant();

    const DataPoint &point = dataPoints[index.row()];
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(point);
    } else if (role == X_role) {
        return point.x;
    } else if (role == Y_role) {
        return point.y;
    }
    return QVariant();
}

QHash<int, QByteArray> HumidityModel::roleNames() const {
    static const QHash<int, QByteArray> roles = {
                                                 { X_role, "x" },
                                                 { Y_role, "y" },
                                                 };
    return roles;
}

void HumidityModel::setHumidityList(const humidity_utils::HumidityList &newHumidityList)
{
    beginResetModel();

    dataPoints.clear();
    int i = 0;
    std::transform(newHumidityList.begin(),
                   newHumidityList.end(),
                   std::back_inserter( dataPoints ),
                   [&i]( const qreal& a_value ) {
                       return DataPoint{ ++i, a_value };
                   });
    endResetModel();
}

void HumidityModel::appendData(qreal a_value)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    dataPoints.append( a_value );
    endInsertRows();
}
