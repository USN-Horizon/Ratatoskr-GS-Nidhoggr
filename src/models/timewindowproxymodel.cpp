#include "timewindowproxymodel.h"
#include <QDebug>

TimeWindowProxyModel::TimeWindowProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent),
    m_timer(nullptr),
    m_windowSize(60.0), // Default 60 second window
    m_invertTimeAxis(true),
    m_lastTime(0.0)
{
    // Set dynamic sort/filter to ensure updates work properly
    setDynamicSortFilter(true);

    // Update more frequently for smoother animation
    connect(&m_updateTimer, &QTimer::timeout, this, &TimeWindowProxyModel::updateFilter);
    m_updateTimer.start(50); // 50ms for smoother updates (20 fps)
}

CountupTimer* TimeWindowProxyModel::timer() const
{
    return m_timer;
}

void TimeWindowProxyModel::setTimer(CountupTimer* timer)
{
    if (m_timer != timer) {
        if (m_timer) {
            disconnect(m_timer, &CountupTimer::secondsChanged, this, &TimeWindowProxyModel::onTimerChanged);
        }

        m_timer = timer;

        if (m_timer) {
            connect(m_timer, &CountupTimer::secondsChanged, this, &TimeWindowProxyModel::onTimerChanged);
        }

        emit timerChanged();
        updateFilter();
    }
}

double TimeWindowProxyModel::windowSize() const
{
    return m_windowSize;
}

void TimeWindowProxyModel::setWindowSize(double size)
{
    if (m_windowSize != size) {
        m_windowSize = size;
        emit windowSizeChanged();
        updateFilter();
    }
}

bool TimeWindowProxyModel::invertTimeAxis() const
{
    return m_invertTimeAxis;
}

void TimeWindowProxyModel::setInvertTimeAxis(bool invert)
{
    if (m_invertTimeAxis != invert) {
        m_invertTimeAxis = invert;
        emit invertTimeAxisChanged();
        invalidate(); // Need to recalculate all mappings
    }
}

double TimeWindowProxyModel::currentTime() const
{
    return m_timer ? m_timer->seconds() : 0.0;
}

QModelIndex TimeWindowProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid())
        return QModelIndex();

    return QSortFilterProxyModel::mapFromSource(sourceIndex);
}

QModelIndex TimeWindowProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QModelIndex();

    return QSortFilterProxyModel::mapToSource(proxyIndex);
}

bool TimeWindowProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (!m_timer)
        return true;

    QModelIndex timeIndex = sourceModel()->index(source_row, 0, source_parent);
    double timeValue = sourceModel()->data(timeIndex, Qt::DisplayRole).toDouble();

    // Get current time from timer
    double currentTime = m_timer->seconds() * 1000.0; // Convert to ms to match data

    // Keep only data points within the time window
    return (timeValue <= currentTime) && (timeValue >= (currentTime - m_windowSize * 1000.0));
}

QVariant TimeWindowProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // Handle special roles for the proxy model
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0 && m_invertTimeAxis) {
            // Get the source data
            QModelIndex sourceIndex = mapToSource(index);
            double timeValue = sourceModel()->data(sourceIndex, role).toDouble();

            // Calculate time relative to current time
            double currentTime = m_timer ? m_timer->seconds() * 1000.0 : 0.0;

            // Return negative offset: 0 = newest, -windowSize*1000 = oldest
            return timeValue - currentTime;
        }
    }

    // For all other roles and columns, pass through to source model
    return QSortFilterProxyModel::data(index, role);
}

void TimeWindowProxyModel::updateFilter()
{
    if (m_timer) {
        double current = m_timer->seconds();

        // Only invalidate if time has changed significantly
        if (std::abs(current - m_lastTime) > 0.05) { // More sensitive threshold
            m_lastTime = current;

            // Force update of all data points when time has changed
            if (m_invertTimeAxis) {
                emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
            }

            // Update filter to show only points in the current time window
            invalidateFilter();
            emit currentTimeChanged();
        }
    }
}

void TimeWindowProxyModel::onTimerChanged()
{
    updateFilter();
}
