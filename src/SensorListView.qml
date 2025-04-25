import QtQuick
import QtQuick.Controls

Item {
    signal sensorSelected(int sensorId)
    signal back()

    property string errorMessage: appController.errorMessage

    Column {
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
                    text: "Lista stanowisk pomiarowych"
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
            width: scroll.width
            height: scroll.height
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

        ScrollView {
            id: scroll
            width: parent.width
            height: parent.height - rectangle.height - 15
            clip: true
            visible: errorMessage == ""

            ListView {
                model: appController.sensorModel
                width: parent.width
                anchors.fill: parent
                clip: true
                spacing: 5

                delegate: Rectangle {
                    width: ListView.view.width
                    height: 50
                    color: "lightblue"
                    border.color: "gray"

                    Text {
                        anchors.centerIn: parent
                        text: modelData.sensorName
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            Qt.callLater(function() {
                                sensorSelected(modelData.sensorId)
                            })
                        }
                    }
                }
            }
        }
    }
}
