#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#pragma once

#include <QObject>
#include <QJsonArray>
#include <QVariantList>
#include <JsonRepository.h>
#include <RequestContext.h>

/**
 * @brief Główna klasa kontrolera logiki aplikacji.
 *
 * AppController pośredniczy między warstwą widoku QML a logiką pobierania i przetwarzania danych
 * o stacjach pomiarowych, czujnikach i danych pomiarowych.
 */
class AppController : public QObject
{
    Q_OBJECT

    /**
     * @brief Lista danych pomiarowych do wyświetlenia.
     */
    Q_PROPERTY(QVariantList dataModel READ dataModel NOTIFY dataModelChanged)

    /**
     * @brief Lista dostępnych stacji pomiarowych.
     */
    Q_PROPERTY(QVariantList sensorModel READ sensorModel NOTIFY sensorModelChanged)

    /**
     * @brief Lista czujników przypisanych do wybranej stacji.
     */
    Q_PROPERTY(QVariantList stationModel READ stationModel NOTIFY stationModelChanged)

    /**
     * @brief ID aktualnie wybranej stacji.
     */
    Q_PROPERTY(int currentStationId READ currentStationId WRITE setCurrentStationId NOTIFY currentStationIdChanged FINAL)

    /**
     * @brief ID aktualnie wybranego czujnika.
     */
    Q_PROPERTY(int currentSensorId READ currentSensorId WRITE setCurrentSensorId NOTIFY currentSensorIdChanged FINAL)

    /**
     * @brief Wiadomość błędu
     */
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
    /**
     * @brief Konstruktor kontrolera.
     * @param repo Wskaźnik do repozytorium danych (JsonRepository).
     * @param parent Wskaźnik do obiektu rodzica.
     */
    explicit AppController(JsonRepository *repo, QObject *parent = nullptr);

    QString errorMessage() const;
    Q_INVOKABLE void setErrorMessage(const QString &msg);

    int currentStationId() const;
    void setCurrentStationId(int id);

    int currentSensorId() const;
    void setCurrentSensorId(int id);

    QVariantList dataModel() const;
    QVariantList stationModel() const;
    QVariantList sensorModel() const;

    /**
     * @brief Wczytuje wszystkie dostępne stacje pomiarowe.
     */
    Q_INVOKABLE void loadStations();

    /**
     * @brief Wczytuje listę czujników dla danej stacji.
     * @param stationId ID stacji.
     */
    Q_INVOKABLE void loadSensorsForStation(int stationId);

    /**
     * @brief Wczytuje dane pomiarowe dla wybranego czujnika.
     * @param sensorId ID czujnika.
     */
    Q_INVOKABLE void loadDataForSensor(int sensorId);

    /**
     * @brief Powraca do widoku listy stacji.
     */
    Q_INVOKABLE void goBackToStations();

    /**
     * @brief Powraca do widoku listy czujników.
     */
    Q_INVOKABLE void goBackToSensors();

signals:
    void currentStationIdChanged();
    void currentSensorIdChanged();
    void dataModelChanged();
    void stationModelChanged();
    void sensorModelChanged();
    void errorMessageChanged();

public slots:
    void handleError(const RequestContext &context, const QString &error);

private slots:
    /**
     * @brief Slot obsługujący gotowe dane zwrócone z repozytorium.
     * @param context Kontekst zapytania HTTP/API.
     * @param data Tablica danych w formacie JSON.
     */
    void handleDataReady(const RequestContext &context, const QJsonArray &data);

private:
    int m_currentStationId = -1;    ///< Aktualne ID stacji.
    int m_currentSensorId = -1;     ///< Aktualne ID czujnika.
    QVariantList m_dataModel;       ///< Model danych pomiarowych.
    QVariantList m_stationModel;    ///< Model stacji.
    QVariantList m_sensorModel;     ///< Model czujników.
    QString m_errorMessage;          ///< Tekst błędu.

    JsonRepository *m_repo;         ///< Wskaźnik do repozytorium danych.
};

#endif // APPCONTROLLER_H
