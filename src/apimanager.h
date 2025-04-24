#ifndef APIMANAGER_H
#define APIMANAGER_H

#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonObject>
#include <QTimer>
#include <RequestContext.h>

/// Bazowy adres API GIOŚ
static constexpr const char* BASE_URL = "https://api.gios.gov.pl/pjp-api/v1/rest/";

/**
 * @class ApiManager
 * @brief Klasa do komunikacji z API.
 *
 * Obsługuje wysyłanie żądań HTTP GET z paginacją, buforowanie odpowiedzi
 * oraz emitowanie sygnałów po odbiorze danych.
 */
class ApiManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor klasy ApiManager.
     * @param parent Obiekt nadrzędny.
     */
    explicit ApiManager(QObject *parent = nullptr);

public slots:
    /**
     * @brief Wysyła żądanie do podanego endpointu z paginacją.
     *
     * Inicjuje wewnętrzne struktury i uruchamia pobieranie od strony 0.
     * @param context Kontekst żądania (endpoint, parametry, UUID).
     */
    void fetchPagedData(const RequestContext &context);

signals:
    /**
     * @brief Sygnał emitowany po zakończeniu pobierania danych (pełnych lub częściowych).
     *
     * @param context Kontekst żądania.
     * @param data Tablica wynikowa (połączone strony).
     * @param isPartial Czy dane są częściowe (błędy przy pobieraniu niektórych stron).
     */
    void rawDataReady(const RequestContext &context, const QJsonArray &data, bool isPartial = false);
    /**
     * @brief Sygnał emitowany, gdy wystąpi błąd podczas pobierania danych.
     * @param context Kontekst żądania.
     * @param error Opis błędu.
     */
    void errorOccured(const RequestContext &context, const QString &error);

private slots:
    /**
     * @brief Slot obsługujący zakończenie odpowiedzi HTTP.
     * @param reply Obiekt odpowiedzi.
     */
    void onReplyFinished(QNetworkReply *reply);


    /**
     * @brief Próbuje ponownie pobrać strony, które wcześniej nie udały się.
     */
    void retryFailedPages();

private:
    /**
     * @brief Wysyła żądanie pobrania danej strony.
     * @param id UUID kontekstu żądania.
     * @param page Numer strony do pobrania.
     */
    void fetchPage(const QUuid &id, int page);
    /**
     * @brief Dodaje stronę do kolejki ponownych prób.
     * @param id UUID żądania.
     * @param page Numer strony.
     */
    void queueRetry(const QUuid &id, int page);

    QNetworkAccessManager *manager; ///< Obiekt do wysyłania zapytań HTTP

    /**
     * @brief Stan żądania.
     */
    struct RequestState {
        QMap<int, bool> pagesCompleted;
        QMap<int, QJsonObject> pageData;
        int totalPages = -1;
    };

    /**
     * @brief Struktura do przechowywania brakujących stron do ponownego pobrania.
     */
    struct RetryState {
        QList<int> missingPages;
    };

    QMap<QUuid, RequestState> requestStates; ///< Stan wszystkich aktywnych żądań
    QMap<QUuid, RetryState> retryQueue; ///< Kolejka ponownych prób
    QMap<QUuid, RequestContext> requestContexts; ///< Konteksty aktywnych żądań
    QTimer retryTimer; ///< Timer uruchamiający ponowne próby co minutę
};

#endif // APIMANAGER_H
