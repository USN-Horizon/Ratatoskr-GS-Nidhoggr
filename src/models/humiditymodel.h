#pragma once
#include <QAbstractListModel>
#include "utils/humidityutils.h"

class HumidityModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        X_role = Qt::UserRole + 1,
        Y_role,
    };

    explicit HumidityModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setHumidityList(const humidity_utils::HumidityList &newHumidityList);
    void appendData( qreal a_value );

private:
    struct DataPoint {
        qreal x;
        qreal y;
    };

    QList<DataPoint> dataPoints;
};
