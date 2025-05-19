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
                    Layout.margins: 8
                    id: horizonLogo
                    source: "./assets/horizon_logo.png"
                    Layout.preferredWidth: 180
                    Layout.preferredHeight: 50
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
                Layout.margins: 20

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
                        text: qsTr("altitude")
                        color: "white"
                        font.pixelSize: 16
                        font.family: "Inter"
                    }
                    Text {
                        id: altitudeValue
                        text: qsTr("1000m")
                        color: "white"
                        font.pixelSize: 32
                        font.family: "Hack Nerd Font Mono"
                    }
                }
                ColumnLayout {
                    Layout.leftMargin: 64
                    Text {
                        id: stateTitle
                        text: qsTr("state")
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

            FermidityGraph {
                // model: fmc.testController.humidityList
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            FermidityGraph {
                // model: fmc.testController.humidityList
                Layout.fillWidth: true
                Layout.fillHeight: true
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
