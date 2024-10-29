import QtQuick
import QtCharts

ChartView {
    title: "Line"
    // antialiasing: true

    Instantiator {
        model: fmc.humidityModel
        onObjectAdded: (index, object) => {
            lineSeries.insert(index, object.x, object.y)
        }
        onObjectRemoved: (index, object) => {
            lineSeries.remove(index)
        }
        QtObject{
            property real x: model.x
            property real y: model.y
        }
    }

    LineSeries {
        id: lineSeries
        name: "LineSeries"


        // Repeater {
        //     id: repeater
        //     model: fmc.testController.humidityList
        //     delegate: XYPoint {
        //         x: index; y: modelData

        //         Component.onCompleted: {
        //             print( index + " | " + modelData )
        //         }
        //     }
        // }
    }
}



// import QtQuick 2.15
// import QtCharts 2.15

// ChartView {
//     width: 800
//     height: 600
//     antialiasing: true

//     LineSeries {
//         name: "Custom Data"
//         XYPoint {
//             x: 0
//             y: 0
//         }

//         XYPoint {
//             x: 0
//             y: 0
//         }

//         XYPoint {
//             x: 0
//             y: 0
//         }
//         XYPoint {
//             x: 0
//             y: 0
//         }
//         model: customModel
//         xValue: "x"
//         yValue: "y"
//     }
// }
