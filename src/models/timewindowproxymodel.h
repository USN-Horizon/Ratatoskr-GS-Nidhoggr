#ifndef TIMEWINDOWPROXYMODEL_H
#define TIMEWINDOWPROXYMODEL_H

#include <QAbstractProxyModel>
#include <QSortFilterProxyModel>
#include <QTimer>
#include "timer.h"

/**
 * @brief The TimeWindowProxyModel class filters data based on a time window
 * relative to the current time from a CountupTimer.
 */
class TimeWindowProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(CountupTimer* timer READ timer WRITE setTimer NOTIFY timerChanged)
    Q_PROPERTY(double windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY(bool invertTimeAxis READ invertTimeAxis WRITE setInvertTimeAxis NOTIFY invertTimeAxisChanged)
    Q_PROPERTY(double currentTime READ currentTime NOTIFY currentTimeChanged)

public:
    explicit TimeWindowProxyModel(QObject *parent = nullptr);

    // Timer that provides the current time
    CountupTimer* timer() const;
    void setTimer(CountupTimer* timer);

    // Size of the time window in seconds
    double windowSize() const;
    void setWindowSize(double size);

    // Whether to invert the time axis (newest on left)
    bool invertTimeAxis() const;
    void setInvertTimeAxis(bool invert);

    // Current time value
    double currentTime() const;

    // Map source coordinates to proxy coordinates
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

protected:
    // Filter function - determines which rows are included
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    // Data function - allows transforming data values
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void updateFilter();
    void onTimerChanged();

signals:
    void timerChanged();
    void windowSizeChanged();
    void invertTimeAxisChanged();
    void currentTimeChanged();

private:
    CountupTimer* m_timer;
    double m_windowSize;
    bool m_invertTimeAxis;
    QTimer m_updateTimer;
    double m_lastTime;
};

#endif // TIMEWINDOWPROXYMODEL_H
