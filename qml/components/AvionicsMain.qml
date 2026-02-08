import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtLocation
import QtPositioning

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

                    Plugin {
                        id: mapPlugin
                        name: "osm"
                    }

                    Map {
                        id: map
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        plugin: mapPlugin
                        center: QtPositioning.coordinate(59.6645, 9.6440) // Krona
                        zoomLevel: 16
                        property geoCoordinate startCentroid

                        Rectangle {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.margins: 8
                            width: coordsColumn.implicitWidth + 16
                            height: coordsColumn.implicitHeight + 16
                            color: "#cc1e1e1e"  // Semi-transparent
                            border.color: "#333333"
                            border.width: 1
                            radius: 4

                            ColumnLayout {
                                id: coordsColumn
                                anchors.fill: parent
                                anchors.margins: 8

                                Text {
                                    id: coordsLabel
                                    text: qsTr("Coordinates")
                                    color: "#888888"
                                    font.pixelSize: 12
                                }

                                RowLayout {
                                    Text {
                                        id: coordsValue
                                        text: qsTr("34N 34S")
                                        color: "white"
                                        font.pixelSize: 16
                                    }

                                    Button {
                                        text: qsTr("📋")
                                    }

                                }
                            }
                        }

                        PinchHandler {
                            id: pinch
                            target: null
                            onActiveChanged: if (active) {
                                map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
                            }
                            onScaleChanged: (delta) => {
                                map.zoomLevel += Math.log2(delta)
                                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                            }
                            onRotationChanged: (delta) => {
                                map.bearing -= delta
                                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                            }
                            grabPermissions: PointerHandler.TakeOverForbidden
                        }
                        WheelHandler {
                            id: wheel
                            // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
                            // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
                            // and we don't yet distinguish mice and trackpads on Wayland either
                            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                                            ? PointerDevice.Mouse | PointerDevice.TouchPad
                                            : PointerDevice.Mouse
                            rotationScale: 1/120
                            property: "zoomLevel"
                        }
                        DragHandler {
                            id: drag
                            target: null
                            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
                        }
                        Shortcut {
                            enabled: map.zoomLevel < map.maximumZoomLevel
                            sequence: StandardKey.ZoomIn
                            onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
                        }
                        Shortcut {
                            enabled: map.zoomLevel > map.minimumZoomLevel
                            sequence: StandardKey.ZoomOut
                            onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
                        }
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

            Component.onCompleted: {
                topBar.missionTimer.start()
                stateM.setCurrentStateByInt(0)
            }
        }
    }
}

