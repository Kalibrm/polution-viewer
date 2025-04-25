#include "appcontroller.h"

AppController::AppController(JsonRepository *repo, QObject *parent) : QObject{parent}, m_repo(repo)
{
    connect(m_repo,&JsonRepository::dataReady, this, &AppController::handleDataReady);
}

int AppController::currentStationId() const {
    return m_currentStationId;
}

void AppController::setCurrentStationId(int id) {
    if (m_currentStationId != id) {
        m_currentStationId = id;
        emit currentStationIdChanged();
    }
}

int AppController::currentSensorId() const {
    return m_currentSensorId;
}

void AppController::setCurrentSensorId(int id) {
    if (m_currentSensorId != id) {
        m_currentSensorId = id;
        emit currentSensorIdChanged();
    }
}

QVariantList AppController::dataModel() const {
    return m_dataModel;
}

QVariantList AppController::stationModel() const {
    return m_stationModel;
}

QVariantList AppController::sensorModel() const {
    return m_sensorModel;
}

void AppController::loadStations() {
    m_stationModel.clear();
    RequestContext context("station/findAll");
    m_repo->loadData(context);
}

void AppController::loadSensorsForStation(int stationId) {
    m_sensorModel.clear();
    setCurrentStationId(stationId);
    RequestContext context(QString("station/sensors/%1"),{},stationId);
    m_repo->loadData(context);
}

void AppController::loadDataForSensor(int sensorId) {
    m_dataModel.clear();
    setCurrentSensorId(sensorId);
    RequestContext context(QString("data/getData/%1"),{},m_currentStationId, sensorId);
    m_repo->loadData(context);
}

void AppController::goBackToStations() {
    loadStations();
}

void AppController::goBackToSensors() {
    if (m_currentStationId != -1)
        loadSensorsForStation(m_currentStationId);
}

void AppController::handleDataReady(const RequestContext &context, const QJsonArray &data) {
    QVariantList simpleData;

    QString baseEndpoint = RequestContext::getBaseEndpoint(context.endpoint);

    if(baseEndpoint == "station/findAll") {
        for (const QJsonValue &value : data) {
            QJsonObject stationObj = value.toObject();
            QVariantMap simplifiedStation;

            simplifiedStation["stationId"] = stationObj.value("Identyfikator stacji").toInt();
            simplifiedStation["stationName"] = stationObj.value("Nazwa stacji").toString();
            simplifiedStation["stationCode"] = stationObj.value("Kod stacji").toString();
            simplifiedStation["cityName"] = stationObj.value("Nazwa miasta").toString();
            simplifiedStation["district"] = stationObj.value("Powiat").toString();
            simplifiedStation["province"] = stationObj.value("Województwo").toString();
            simplifiedStation["street"] = stationObj.value("Ulica").toString();
            simplifiedStation["longitude"] = stationObj.value("WGS84 λ E").toString();
            simplifiedStation["latitude"] = stationObj.value("WGS84 φ N").toString();

            simpleData.append(simplifiedStation);
        }
        m_stationModel = simpleData;
        emit stationModelChanged();
    } else if(baseEndpoint == "station/sensors") {
        for (const QJsonValue &value : data) {
            QJsonObject sensorObj = value.toObject();
            QVariantMap simplifiedSensor;

            simplifiedSensor["stationId"] = sensorObj.value("Identyfikator stacji").toInt();
            simplifiedSensor["sensorId"] = sensorObj.value("Identyfikator stanowiska").toInt();
            simplifiedSensor["sensorName"] = sensorObj.value("Wskaźnik").toString();
            simplifiedSensor["sensorCode"] = sensorObj.value("Wskaźnik - kod").toString();
            simplifiedSensor["sensorFormula"] = sensorObj.value("Wskaźnik - wzór").toString();

            simpleData.append(simplifiedSensor);   
        }
        m_sensorModel = simpleData;
        emit sensorModelChanged();
    } else if(baseEndpoint == "data/getData"){
        for (const QJsonValue &value : data) {
            QJsonObject dataObj = value.toObject();
            QVariantMap simplifiedDataEntry;

            QString timestampStr = dataObj.value("Data").toString();
            QDateTime timestamp = QDateTime::fromString(timestampStr, "yyyy-MM-dd HH:mm:ss");
            qint64 timestampUnix = timestamp.toSecsSinceEpoch();

            simplifiedDataEntry["timestamp"] = timestampUnix;
            simplifiedDataEntry["value"] = dataObj.value("Wartość").toDouble();

            simpleData.append(simplifiedDataEntry);
        }

        QVector<QVariantMap> sortedList;
        sortedList.reserve(simpleData.size());
        for (auto it = simpleData.constBegin(); it != simpleData.constEnd(); ++it) {
            sortedList.append(it->toMap());
        }

        std::sort(sortedList.begin(), sortedList.end(), [](const QVariantMap &a, const QVariantMap &b) {
            return a["timestamp"].toLongLong() < b["timestamp"].toLongLong();
        });

        simpleData.clear();
        for (const QVariantMap &item : sortedList) {
            simpleData.append(item);
        }

        m_dataModel = simpleData;
        emit dataModelChanged();
    }
}
