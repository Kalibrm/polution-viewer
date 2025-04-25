import QtQuick
import QtQuick.Controls
import QtGraphs

Item {
    id: view
    signal back()

    property var sensorData: appController.dataModel
    property string errorMessage: appController.errorMessage

    function buildSeries() {
        lineSeries.clear();

       if (sensorData.length === 0)
           return;

       var minY = sensorData[0].value;
       var maxY = sensorData[0].value;
       for (let i = 0; i < sensorData.length; i++) {
           let x = i;
           let y = sensorData[i].value;
           lineSeries.append(x, y);
           minY = Math.min(minY, y);
           maxY = Math.max(maxY, y);
       }
       axisX.min = 0;
       axisX.max = sensorData.length - 1;
       axisX.tickInterval = 4;
       axisY.min = Math.floor(minY - 5);
       axisY.max = Math.ceil(maxY + 5);
    }

    function formatTimestamp(xVal) {
        if (xVal < 0 || xVal >= sensorData.length)
            return "";

        var timestamp = sensorData[Math.floor(xVal)].timestamp;
        var d = new Date(timestamp * 1000);
        return d.toLocaleString();
    }

    Component.onCompleted: {
        buildSeries()
    }

    Connections {
        target: appController
        function onDataModelChanged() {
            buildSeries()
        }
    }

    Column {
        id: column
        anchors.fill: parent
        spacing: 10

        Rectangle {
            id: rectangle
            width: parent.width
            height: title.implicitHeight + 15
            color: "lightgray"
            border.color: "black"
            z: 100

            Text {
                id: title
                text: "Dane pomiarowe"
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenterOffset: 0
                anchors.centerIn: parent
                font.pointSize: 20
            }
            Button {
                x: 8
                y: 16
                text: "← Wróć"
                onClicked: back()
            }
        }

        Rectangle {
            id: errorView
            width: chart.width
            height: chart.height
            visible: errorMessage !== ""
            color: "#ffe6e6"
            border.color: "#cc0000"
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            z: 99

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: errorMessage
                color: "black"
                wrapMode: Text.Wrap
                font.bold: true
                font.pointSize: 14
            }
        }

        GraphsView {
            id: chart
            visible: errorMessage == ""
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height - 100
            theme: GraphsTheme {
                grid.mainColor: "#ccc"
                grid.subColor: "#eee"
                labelTextColor: "black"
                plotAreaBackgroundColor: "white"
                backgroundColor: "white"
                colorScheme: Qt.Light
            }

            axisX: ValueAxis {
                id: axisX
                labelFormat: "%d h"
                visible: false
            }

            axisY: ValueAxis {
                id: axisY
                labelFormat: "%.1f"
            }

            LineSeries {
                id: lineSeries
                width: 2
                color: "blue"

                pointDelegate: Rectangle {
                    width: 8
                    height: 8
                    radius: 3
                    color: "blue"
                    border.color: "white"
                    border.width: 1

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            tooltip.text = "Wartość: " + pointValueY.toFixed(2) + "\nCzas: " + formatTimestamp(pointValueX);
                            tooltip.x = chart.mapToItem(view, Qt.point(parent.x, parent.y)).x + 10;
                            tooltip.y = chart.mapToItem(view, Qt.point(parent.x, parent.y)).y - 40;
                            tooltip.visible = true;
                        }
                        onExited: {
                            tooltip.visible = false;
                        }
                    }

                    property real pointValueX;
                    property real pointValueY;
                }
            }
        }

        Rectangle {
            id: tooltip
            color: "#fffacd"
            border.color: "black"
            radius: 5
            opacity: 0.9
            visible: false
            z: 10
            // width: tooltipText.implicitWidth + 10
            // height: tooltipText.implicitHeight + 6

            Text {
                id: tooltipText
                text: tooltip.text
                color: "black"
                wrapMode: Text.WordWrap
            }

            property string text: ""
        }

    }
}
