import QtQuick 2.15
import QtQuick.Window 2.15
import QtCharts 2.15

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: ""
    property var numbers: context_interface.numbers

    signal updateRequested()

    Text {
        text: context_interface.percent
        z: 12
    }

    Component.onCompleted: {
        // chart.setRubberBand(2)
    }


    Connections {
        target: context_interface

        function onNumbersChanged() {
            console.log("update")
            // numbers.values = [0]
            // chart.zoom(1/2)
            // // numbers.values = context_interface.numbers
            // numbers.values = [ context_interface.numbers[0] , context_interface.numbers[1] , 2057 , 2204 , 2735 , 2844 , 2964 , 2989 , 3422 , 4437 , 4961 , 7219 , 7737 , 11467 , 17072 ]
            // chart.seriesAdded(mySeries)
            // chart.zoomReset()
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
        Item {
        }
    }

    Component {
        id: chart_component
        ChartView {
            id: chart
            title: "Bar series"
            anchors.fill: parent
            legend.alignment: Qt.AlignBottom
            antialiasing: true
            BarSeries {
                id: mySeries
                axisX: BarCategoryAxis { categories: context_interface.words }
                BarSet {
                    id: numbers
                    // values: window.numbers
                    values:  context_interface.numbers.slice(0,15)
                }
            }
        }
    }
}


