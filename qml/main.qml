import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FmControllers
import FermidityCollection
import com.horizon.components
import "components"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Horizon Groundstation")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            Action { text: qsTr("Import...") }
            Action { text: qsTr("Export...") }
            MenuSeparator { }
            Action { text: qsTr("Quit") }
        }
        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("Cu&t") }
            Action { text: qsTr("&Copy") }
            Action { text: qsTr("&Paste") }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About") }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 0
        spacing: 0


        TopBar {
            id: topBar
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true


            RowLayout {
                Layout.alignment: left
                Layout.fillHeight: true
                spacing: 0

                TabBar {
                    id: tabBar
                    Layout.fillHeight: true
                    Layout.preferredWidth: 100
                    Layout.minimumWidth: 100
                    padding: 0
                    leftPadding: 0
                    rightPadding: 0



                    // Make tabs vertical
                    contentItem: ListView {
                        model: tabBar.contentModel
                        currentIndex: tabBar.currentIndex
                        spacing: 0
                        orientation: ListView.Vertical
                    }

                    TabButton {
                        text: "Avionics"
                        rotation: 0
                        implicitHeight: tabBar.height / 2
                        width: tabBar.width
                    }
                    TabButton {
                        text: "Payload"
                        implicitHeight: tabBar.height / 2
                        width: tabBar.width
                    }
                }

                StackLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    currentIndex: tabBar.currentIndex

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
                            timer: topBar.missionTime
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

                                // 2. Reset state model to initial state (Calibrating)
                                stateM.setCurrentStateByInt(0)

                                // 3. Add some test state data if no real data is available
                                var currentTime = topBar.missionTimer.seconds * 1000
                                stateM.appendStateData(currentTime, 0) // Calibrating
                                stateM.appendStateData(currentTime + 1000, 1) // Ready
                                stateM.appendStateData(currentTime + 2000, 2) // Thrusting
                            }
                        }
                    }

                    Rectangle { color: "lightgreen"; Text { text: "Content 2"; anchors.centerIn: parent } }
                }
            }

        }
    }
}
