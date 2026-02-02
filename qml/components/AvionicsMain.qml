import QtQuick
import QtQuick.Layouts

ColumnLayout {
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.margins: 0
    spacing: 0


    RowLayout {
        Layout.margins: 8


        ColumnLayout {
            Layout.leftMargin: 50
            Text {
                id: connectionTitle
                text: qsTr("connection")
                color: "white"
                font.pixelSize: 16
                font.family: "Inter"
            }
            Text {
                id: connectionValue
                text: qsTr("Good")
                color: "white"
                font.pixelSize: 32
                font.family: "Hack Nerd Font Mono"
            }
        }
        ColumnLayout {
            Layout.leftMargin: 64
            Text {
                id: altitudeTitle
                text: qsTr("errors")
                color: "white"
                font.pixelSize: 16
                font.family: "Inter"
            }
            Text {
                id: altitudeValue
                text: qsTr("0")
                color: "white"
                font.pixelSize: 32
                font.family: "Hack Nerd Font Mono"
            }
        }
        ColumnLayout {
            Layout.leftMargin: 64
            Text {
                id: stateTitle
                text: qsTr("energetics")
                color: "white"
                font.pixelSize: 16
                font.family: "Inter"
            }
            Text {
                id: stateValue
                text: qsTr("Ready")
                color: "white"
                font.pixelSize: 32
                font.family: "Hack Nerd Font Mono"
            }
        }
    }

    RealtimeGraph {
        id: realtimeVelocityGraph
        title: "Velocity (Real-time)"
        model: velM
        timer: topBar.missionTimer
        windowSize: 60       // Show last 60 seconds of data
        yAxisMax: 500
        yAxisMin: -500
        yAxisInterval: 250
        yAxisLabel: "Velocity (m/s)"
        lineColor: "#2CDE85"
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    RealtimeGraph {
        id: realtimeAltitudeGraph
        title: "Altitude (Real-time)"
        model: altM
        timer: topBar.missionTimer
        windowSize: 60      // Show last 120 seconds of data
        yAxisMax: 6000
        yAxisInterval: 1000
        yAxisLabel: "Altitude (m)"
        lineColor: "#DBEB00"
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    // Flight state view
    FlightStateView {
        id: flightStateView
        stateModel: stateM
        timer: topBar.missionTimer
        windowSize: 60
        Layout.fillWidth: true
        Layout.preferredHeight: 170

        Component.onCompleted: {
            topBar.missionTimer.start()
            stateM.setCurrentStateByInt(0)
        }
    }
}

