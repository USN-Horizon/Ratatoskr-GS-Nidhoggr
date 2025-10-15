import QtQuick
import QtGraphs

GraphsView {
    // antialiasing: true
    theme: GraphsTheme {
        readonly property color c1: "#DBEB00"
        readonly property color c2: "#373F26"
        readonly property color c3: Qt.lighter(c2, 1.5)
        colorScheme: GraphsTheme.ColorScheme.Dark
        seriesColors: ["#2CDE85", "#DBEB00"]
        grid.mainColor: c3
        grid.subColor: c2
        axisX.mainColor: c3
        axisY.mainColor: c3
        axisX.subColor: c2
        axisY.subColor: c2
        axisX.labelTextColor: c1
        axisY.labelTextColor: c1
    }
    axisX: ValueAxis {
        min: -120000
        max: 0
        tickInterval: 6000
        subTickCount: 2
        labelDecimals: 1
    }
    axisY: ValueAxis {
        max: 70
        tickInterval: 10
        subTickCount: 4
        labelDecimals: 1
    }

    LineSeries {
        id: lineSeries
        name: "LineSeries"

        XYModelMapper {
            model: fmc.humidityModel
            series: lineSeries
            xSection: 0
            ySection: 1
        }

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
