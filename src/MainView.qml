import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    title: "Jakość powietrza"

    property string currentView: "stations"

    Loader {
        id: viewLoader
        anchors.fill: parent
        sourceComponent: {
            if(currentView == "stations") return stationList
            if(currentView == "sensors") return sensorList
            if(currentView == "data") return sensorData
        }
    }

    Component {
        id: stationList
        StationListView {
            onStationSelected: (stationId) => {
                appController.loadSensorsForStation(stationId)
                root.currentView = "sensors"
            }
        }
    }

    Component {
        id: sensorList
        SensorListView {
            onBack: {
                appController.goBackToStations()
                root.currentView = "stations"
            }
            onSensorSelected: (sensorId) => {
                appController.loadDataForSensor(sensorId)
                root.currentView = "data"
            }
        }
    }

    Component {
        id: sensorData
        SensorDataView {
            onBack: {
                appController.goBackToSensors()
                root.currentView = "sensors"
            }
        }
    }
}
