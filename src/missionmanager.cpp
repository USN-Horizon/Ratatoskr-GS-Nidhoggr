#include "missionmanager.h"
#include "utils/flightlogfactory.h"

MissionManager::MissionManager(FlightModels &models, QObject *parent) : QObject(parent), m_models(models), m_state(CaptureState{}) {}

QString MissionManager::missionName() const {return m_missionName;}

QString MissionManager::modeText() const
{
    if (std::get_if<PlaybackState>(&m_state)) return "Capture Mode";
    return "Playback Mode";
}

void MissionManager::setMissionName(const QString &name) {
    if (name != m_missionName) {
        m_missionName = name;
        emit missionNameChanged();
    }
}

bool MissionManager::importMissionData(const QString &path)
{
    const ParsedFlightLog data = FlightLogFactory::parse(path);
    if (!data.valid) return false;

    FlightLogFactory::populateAltitude(m_models.altitude, data);
    FlightLogFactory::populateVelocity(m_models.velocity, data);
    if (m_models.state)
        FlightLogFactory::populateState(m_models.state, data);

    m_state = PlaybackState {
        path
    };
    emit stateChanged();

    return true;
}

bool MissionManager::isPlayback() const {
    return std::holds_alternative<PlaybackState>(m_state);
}

/**
 *	Though Capture mode is default state, we want to be able to start capture mode again after user has dismissed playback mode
 */
void MissionManager::startCapture() {
    m_state = CaptureState {};
    emit stateChanged();
}
