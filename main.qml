import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtCharts 2.15
import QtQuick.Dialogs 1.3

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: "Word frequencies"

    Connections {
        target: context_interface

        function onNumbersChanged() {
            loader.sourceComponent = dummy
            loader.sourceComponent = chart_component
        }

        function onFinished() {
            start_button.enabled = true
        }
    }

    ProgressBar {
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        z: 2
        to: 100
        value: context_interface.percent
    }

    Loader {
        id: loader
        anchors.fill: parent
        sourceComponent: chart_component
    }

    Button {
        id: start_button
        anchors.bottom: parent.bottom
        text: "Open"
        onClicked: fileDialog.open()
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
            legend.visible: false
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

    FileDialog {
        id: fileDialog
        visible: false
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            start_button.enabled = false
            context_interface.fileChoosed(fileDialog.fileUrls)
        }
    }
}


