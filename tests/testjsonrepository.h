#ifndef TESTJSONREPOSITORY_H
#define TESTJSONREPOSITORY_H

#include <QDir>
#include <QStringList>

/**
 * @class TestJsonRepository
 * @brief Zestaw testów jednostkowych dla klasy JsonRepository.
 *
 * Testuje poprawność generowania ścieżek plików cache na podstawie różnych endpointów.
 */
class TestJsonRepository : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Testuje generowanie ścieżki cache dla endpointu "station/findAll".
     */
    void testGetCacheFilePath_stationFindAll();

    /**
     * @brief Testuje generowanie ścieżki cache dla endpointu "station/sensors" z ID stacji.
     */
    void testGetCacheFilePath_stationSensors();

    /**
     * @brief Testuje generowanie ścieżki cache dla endpointu "data/getData" z ID stacji i sensora.
     */
    void testGetCacheFilePath_dataGetData();

    /**
     * @brief Testuje generowanie ścieżki cache dla nieznanego endpointu.
     */
    void testGetCacheFilePath_unknownEndpoint();
};

#endif // TESTJSONREPOSITORY_H
