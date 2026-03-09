#ifndef MISSIONMANAGER_H
#define MISSIONMANAGER_H

#include <QObject>
#include <QString>
#include "models/timeseriesmodel.h"
#include "models/flightstatemodel.h"

struct FlightModels {
    TimeSeriesModel* acceleration;
    TimeSeriesModel* rotation;
    TimeSeriesModel* pressure;
    TimeSeriesModel* altitude;
    TimeSeriesModel* velocity;
    FlightStateModel* state;
};

struct PlaybackMetadata {
    QString* filepath;
};

struct CaptureState {};
struct PlaybackState {
    QString filepath;
};

/**
 * @brief Manages the state of current mission, with mission metadata, importing and exporting data.
 */
class MissionManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString missionName READ missionName WRITE setMissionName NOTIFY missionNameChanged)
    Q_PROPERTY(QString modeText READ modeText NOTIFY stateChanged)
    Q_PROPERTY(bool isPlayback READ isPlayback NOTIFY stateChanged)
public:
    explicit MissionManager(FlightModels &models, QObject *parent = nullptr);

    QString missionName() const;
    void setMissionName(const QString &name);
    QString modeText() const;
    Q_INVOKABLE bool importMissionData(const QString &path);
    Q_INVOKABLE void startCapture();
    bool isPlayback() const;

signals:
    void missionNameChanged();
    void stateChanged();

private:
    FlightModels m_models;
    QString m_missionName;
    std::variant<CaptureState, PlaybackState> m_state;
};

#endif // MISSIONMANAGER_H
