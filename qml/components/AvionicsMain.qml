import QtQuick
import QtQuick.Layouts

RowLayout {
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.margins: 0
    spacing: 0

    ColumnLayout {
        RealtimeGraph {
            id: realtimeAccelerationGraph
            title: "Acceleration"
            model: velM
            timer: topBar.missionTimer
            windowSize: 60       // Show last 60 seconds of data
            yAxisMax: 500
            yAxisMin: -500
            yAxisInterval: 250
            yAxisLabel: "Acceleration (m/s)^2"
            lineColor: "#2CDE85"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RealtimeGraph {
            id: realtimeRotationGraph
            title: "Rotation"
            model: altM
            timer: topBar.missionTimer
            windowSize: 60      // Show last 120 seconds of data
            yAxisMax: 6000
            yAxisInterval: 1000
            yAxisLabel: "Rotation (deg/s)"
            lineColor: "#DBEB00"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RealtimeGraph {
            id: realtimePressureGraph
            title: "Pressure"
            model: altM
            timer: topBar.missionTimer
            windowSize: 60      // Show last 120 seconds of data
            yAxisMax: 6000
            yAxisInterval: 1000
            yAxisLabel: "Pressure (Pa)"
            lineColor: "#DBEB00"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RealtimeGraph {
            id: realtimeAltitudeGraph
            title: "Altitude"
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

        RealtimeGraph {
            id: realtimeSpeedGraph
            title: "Speed"
            model: altM
            timer: topBar.missionTimer
            windowSize: 60      // Show last 120 seconds of data
            yAxisMax: 6000
            yAxisInterval: 1000
            yAxisLabel: "Speed (m/s)"
            lineColor: "#DBEB00"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    ColumnLayout {
        // Flight state view

        RowLayout {
              Layout.fillWidth: true


          }



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
}

