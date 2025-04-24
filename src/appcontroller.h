#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#pragma once

#include <QObject>
#include <QJsonArray>
#include <QVariantList>
#include <JsonRepository.h>
#include <RequestContext.h>

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList dataModel READ dataModel NOTIFY dataModelChanged)
    Q_PROPERTY(QVariantList sensorModel READ sensorModel NOTIFY sensorModelChanged)
    Q_PROPERTY(QVariantList stationModel READ stationModel NOTIFY stationModelChanged)
    Q_PROPERTY(int currentStationId READ currentStationId WRITE setCurrentStationId NOTIFY currentStationIdChanged FINAL)
    Q_PROPERTY(int currentSensorId READ currentSensorId WRITE setCurrentSensorId NOTIFY currentSensorIdChanged FINAL)


public:
    explicit AppController(JsonRepository *repo, QObject *parent = nullptr);

    int currentStationId() const;
    void setCurrentStationId(int id);

    int currentSensorId() const;
    void setCurrentSensorId(int id);

    QVariantList dataModel() const;
    QVariantList stationModel() const;
    QVariantList sensorModel() const;

    Q_INVOKABLE void loadStations();
    Q_INVOKABLE void loadSensorsForStation(int stationId);
    Q_INVOKABLE void loadDataForSensor(int sensorId);

    Q_INVOKABLE void goBackToStations();
    Q_INVOKABLE void goBackToSensors();

signals:
    void currentStationIdChanged();
    void currentSensorIdChanged();
    void dataModelChanged();
    void stationModelChanged();
    void sensorModelChanged();


private slots:
    void handleDataReady(const RequestContext &context, const QJsonArray &data);

private:
    int m_currentStationId = -1;
    int m_currentSensorId = -1;
    QVariantList m_dataModel;
    QVariantList m_stationModel;
    QVariantList m_sensorModel;

    JsonRepository *m_repo;
};

#endif // APPCONTROLLER_H
