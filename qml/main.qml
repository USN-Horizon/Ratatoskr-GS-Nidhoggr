import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FmControllers
import FermidityCollection

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Tabs")

    ColumnLayout {
        anchors.fill: parent

        TextField {
            text: fmc.testController.name
        }

        FermidityGraph {
            // model: fmc.testController.humidityList
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
