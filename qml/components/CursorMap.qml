import QtQuick
import QtQuick.Layouts
import QtLocation
import QtPositioning
import QtQuick.Controls
import com.horizon.components

Item {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true

    // Properties to configure the graph
    property alias model: timeWindow.sourceModel
    property alias timer: timeWindow.timer

    // Internal time window proxy model
    TimeWindowProxyModel {
        id: timeWindow
        invertTimeAxis: true
        windowSize: 30

        // Monitor changes to ensure graph updates
        onCurrentTimeChanged: {
            var n = timeWindow.rowCount()
            if (n > 0) {
                var point = timeWindow.get(n - 1)
                map.center = QtPositioning.coordinate(point.y_lat, point.y_lon)
            }
        }
    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
        plugin: mapPlugin
        center: QtPositioning.coordinate(59.6645, 9.6440) // Krona
        zoomLevel: 16
        property geoCoordinate startCentroid

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            //margins: 10
        }


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
