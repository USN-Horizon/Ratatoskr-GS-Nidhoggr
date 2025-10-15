import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FmControllers
import FermidityCollection
import com.horizon.components

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Horizon Groundstation")

    RowLayout {

        anchors.fill: parent

        ColumnLayout {

            RowLayout {
                Image {
                    Layout.margins: 0
                    id: horizonLogo
                    source: "./assets/horizon_logo.png"
                    Layout.preferredWidth: 230
                    Layout.preferredHeight: 100
                    Layout.alignment: Qt.AlignCenter
                    fillMode: Image.PreserveAspectFit
                    clip:true
                }
                Text {
                    id: name
                    text: qsTr("Horizon Groundstation")
                    color: "white"
                    font.pixelSize: 24
                    font.family: "Inter"
                    Layout.margins: 20
                }
            }

            RowLayout {
                Layout.margins: 8

                Rectangle {
                    id: timerContainer
                    width: 280
                    height: 130
                    color: "#1e1e1e"
                    border.color: "#333333"
                    radius: 5

                    CountupTimer {
                        id: missionTimer
                    }

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 5

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "MISSION TIME"
                            color: "#aaaaaa"
                            font.pixelSize: 14
                        }

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: missionTimer.timeString
                            color: "#00ff00"
                            font.pixelSize: 32
                            font.bold: true
                            font.family: "Monospace"
                        }

                        // Control buttons
                        RowLayout {
                            spacing: 8

                            Button {
                                text: "Start"
                                onClicked: missionTimer.resume()
                                width: 100
                            }

                            Button {
                                text: "Stop"
                                onClicked: missionTimer.stop()
                                width: 100
                            }

                            Button {
                                text: "Reset"
                                onClicked: {
                                    missionTimer.stop();
                                    missionTimer.reset();
                                }
                                width: 100
                            }
                        }
                    }


                    // Auto-start option (remove if not needed)
                    Component.onCompleted: {
                        // Uncomment to auto-start the timer when loaded
                        // missionTimer.start();
                    }
                }

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
                timer: missionTimer  // Connect to your existing timer
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
                timer: missionTimer  // Connect to your existing timer
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
                timer: missionTimer
                windowSize: 60
                Layout.fillWidth: true
                Layout.preferredHeight: 170

                Component.onCompleted: {
                    missionTimer.start()

                    // 2. Reset state model to initial state (Calibrating)
                    stateM.setCurrentStateByInt(0)

                    // 3. Add some test state data if no real data is available
                    var currentTime = missionTimer.seconds * 1000
                    stateM.appendStateData(currentTime, 0) // Calibrating
                    stateM.appendStateData(currentTime + 1000, 1) // Ready
                    stateM.appendStateData(currentTime + 2000, 2) // Thrusting
                }
            }
        }

        ColumnLayout {
            Image {
                id: rocket
                source: "./assets/RocketImageTransparent.png"
                Layout.preferredWidth: 300
                Layout.preferredHeight: 900
                Layout.alignment: Qt.AlignCenter
                fillMode: Image.PreserveAspectFit
                clip:true
            }
        }

    }
}
