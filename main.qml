import QtQuick 2.15
import QtQuick.Window 2.15
import QtCharts 2.15

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: ""

    signal updateRequested()

    // Text {
    //     text: context_interface.percent
    // }
    //
    // Component.onCompleted: {
    // }
    //
    //
    // Connections {
    //     target: context_interface
    //
    //     function onStatsUpdated() {
    //     }
    // }

    ChartView {
        title: "Bar series"
        anchors.fill: parent
        legend.alignment: Qt.AlignBottom
        antialiasing: true

        BarSeries {
            id: mySeries
            axisX: BarCategoryAxis { categories: context_interface.stats.keys() }
            BarSet { label: "Bob"; values: context_interface.stats.values() }
        }
    }

}

