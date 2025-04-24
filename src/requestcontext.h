#ifndef REQUESTCONTEXT_H
#define REQUESTCONTEXT_H

#include <QUuid>
#include <QString>
#include <QMap>

/**
 * @class RequestContext
 * @brief Klasa reprezentująca kontekst pojedynczego żądania API.
 *
 * Przechowuje informacje potrzebne do wykonania zapytania, takie jak endpoint,
 * identyfikatory stacji/sensora, parametry zapytania oraz unikalne ID.
 */
class RequestContext {
public:
    /**
     * @brief Domyślny konstruktor – generuje nowy UUID.
     */
    RequestContext();

    /**
     * @brief Konstruktor kontekstowy tworzący pełny endpoint na podstawie szablonu i ID.
     *
     * Jeśli `endpointTemplate` zawiera `%1`, zostanie on zastąpiony przez `stationId` lub `sensorId`.
     * @param endpointTemplate Szablon endpointu (np. "station/sensors/%1").
     * @param params Parametry zapytania (query string).
     * @param stationId Opcjonalny identyfikator stacji.
     * @param sensorId Opcjonalny identyfikator sensora.
     */
    explicit RequestContext(const QString &endpointTemplate,
                            const QMap<QString, QString> &params = {},
                            const int &stationId = 0,
                            const int &sensorId = 0);

    /**
     * @brief Zwraca bazowy endpoint bez ID na końcu.
     *
     * Przykład: `station/sensors/12` → `station/sensors`
     * @param endpoint Pełny endpoint.
     * @return Bazowy endpoint bez końcowego ID.
     */
    static QString getBaseEndpoint(const QString &endpoint);

    QUuid id;                      ///< Unikalny identyfikator zapytania
    QString endpoint;             ///< Docelowy endpoint API (może być parametryzowany)
    int stationId;                ///< Opcjonalny ID stacji (jeśli dotyczy)
    int sensorId;                 ///< Opcjonalny ID sensora (jeśli dotyczy)
    QMap<QString, QString> params; ///< Dodatkowe parametry zapytania (np. page, date, etc.)
    bool isArchival = false;      ///< Flaga wskazująca, czy zapytanie dotyczy danych archiwalnych
    QString date;                 ///< Używana przy danych archiwalnych – konkretny dzień
};





#endif // REQUESTCONTEXT_H
