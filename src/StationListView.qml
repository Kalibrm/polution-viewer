import QtQuick
import QtQuick.Controls

Item {
    signal stationSelected(int stationId)
    clip: true

    property string searchQuery: ""

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
                text: "Lista stacji pomiarowych"
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenterOffset: 0
                anchors.centerIn: parent
                font.pointSize: 20
            }
        }

        TextField {
            id: searchField
            width: parent.width - 20
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Wyszukaj stację..."
            text: searchQuery
            onTextChanged: searchQuery = searchField.text
        }

        ScrollView {
            width: parent.width
            height: parent.height - rectangle.height - 15
            clip: false

            ListView {
                width: parent.width
                anchors.fill: parent
                clip: true
                keyNavigationWraps: true
                spacing: 5
                model: appController.stationModel

                delegate: Rectangle {
                    width: ListView.view.width
                    height: 50
                    color: "lightgray"
                    border.color: "black"

                    Text {
                        anchors.centerIn: parent
                        text: modelData.stationName
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            Qt.callLater(function() {
                                stationSelected(modelData.stationId)
                            })
                        }
                    }
                }
            }

        }

    }

    Component.onCompleted: appController.loadStations()
}
