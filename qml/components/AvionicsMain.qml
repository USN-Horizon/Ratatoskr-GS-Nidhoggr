import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtLocation
import QtPositioning
import com.horizon.components

RowLayout {
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.margins: 0
    spacing: 0

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.preferredWidth: 2

        RealtimeGraph {
            id: realtimeAccelerationGraph
            title: "Acceleration"
            model: accelerationM
            timer: topBar.missionTimer
            windowSize: 60       // Show last 60 seconds of data
            yAxisMax: 300
            yAxisMin: -300
            yAxisInterval: 300
            yAxisLabel: "Acceleration (m/s)^2"
            unitText: "(m/s)²"
            lineColor: "#2CDE85"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RealtimeGraph {
            id: realtimeRotationGraph
            title: "Rotation"
            model: rotationM
            timer: topBar.missionTimer
            windowSize: 60      // Show last 120 seconds of data
            yAxisMax: 1000
            yAxisInterval: 500
            yAxisLabel: "Rotation (deg/s)"
            unitText: "deg/s"
            lineColor: "#DBEB00"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RealtimeGraph {
            id: realtimePressureGraph
            title: "Pressure"
            model: pressureM
            timer: topBar.missionTimer
            windowSize: 60      // Show last 120 seconds of data
            yAxisMax: 1200
            yAxisInterval: 300
            yAxisLabel: "Pressure (kPa)"
            unitText: "kPa"
            lineColor: "#DBEB00"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RealtimeGraph {
            id: realtimeAltitudeGraph
            title: "Altitude"
            model: altitudeM
            timer: topBar.missionTimer
            windowSize: 60      // Show last 120 seconds of data
            yAxisMax: 6000
            yAxisInterval: 2000
            yAxisLabel: "Altitude (m)"
            unitText: "m"
            lineColor: "#DBEB00"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RealtimeGraph {
            id: realtimeSpeedGraph
            title: "Speed"
            model: velocityM
            timer: topBar.missionTimer
            windowSize: 60      // Show last 120 seconds of data
            yAxisMax: 500
            yAxisMin: -500
            yAxisInterval: 500
            yAxisLabel: "Speed (m/s)"
            unitText: "m/s"
            lineColor: "#DBEB00"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.preferredWidth: 1

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent

                // Map column
                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    CursorMap {
                        model: locationM
                        timer: topBar.missionTimer
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: tempColumn.implicitHeight + 16
            color: "#1e1e1e"
            border.color: "#333333"
            border.width: 1
            radius: 4

            ColumnLayout {
                id: tempColumn
                anchors.fill: parent
                anchors.margins: 8

                Text {
                    id: tempLabel
                    text: qsTr("Temperature")
                    color: "#888888"
                    font.pixelSize: 12
                }

                Text {
                    id: tempValue
                    text: qsTr("34 deg")
                    color: "white"
                    font.pixelSize: 16
                }
            }
        }

        FlightStateView {
            id: flightStateView
            stateModel: stateM
            timer: topBar.missionTimer
            windowSize: 60
            Layout.fillWidth: true
            Layout.preferredHeight: 170
            Layout.maximumHeight: 170
        }
    }
}

