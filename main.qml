import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtCharts 2.15

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: "Word frequencies"

    ProgressBar {
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        z: 12
        to: 100
        value: context_interface.percent
    }


    Component.onCompleted: {
        window.showMaximized()
    }


    Connections {
        target: context_interface

        function onNumbersChanged() {
            loader.sourceComponent = dummy
            loader.sourceComponent = chart_component
        }
    }

    Loader {
        id: loader
        anchors.fill: parent
        sourceComponent: chart_component
    }

    Component {
        id: dummy
        Item {}
    }

    Component {
        id: chart_component
        ChartView {
            id: chart
            anchors.fill: parent
            legend.alignment: Qt.AlignBottom
            antialiasing: true
            BarSeries {
                id: mySeries
                axisX: BarCategoryAxis { categories: context_interface.words }
                BarSet {
                    id: numbers
                    values:  context_interface.numbers.slice(0,15)
                }
            }
        }
    }
}


