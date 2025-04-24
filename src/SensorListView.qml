import QtQuick
import QtQuick.Controls

Item {
    signal sensorSelected(int sensorId)
    signal back()

    Column {
        anchors.fill: parent
        spacing: 10

        Rectangle {
            id: rectangle
            width: parent.width
            height: title.implicitHeight + 15
            color: "lightgray"
            border.color: "black"
            z: 1

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

        ScrollView {
            width: parent.width
            height: parent.height - rectangle.height - 15
            clip: false

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
