#ifndef TESTREQUESTCONTEXT_H
#define TESTREQUESTCONTEXT_H

#include <QObject>

/**
 * @class TestRequestContext
 * @brief Zestaw testów jednostkowych dla klasy RequestContext.
 *
 * Testuje konstrukcję kontekstu zapytania oraz logikę metody getBaseEndpoint().
 */
class TestRequestContext : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Testuje, czy domyślny konstruktor generuje poprawny UUID.
     */
    void testDefaultConstructor();

    /**
     * @brief Testuje konstruktor parametryzowany dla różnych kombinacji parametrów.
     */
    void testParameterizedConstructor();

    /**
     * @brief Testuje wyodrębnianie bazowego endpointu z końcówką liczbową.
     */
    void testGetBaseEndpointWithNumericSuffix();

    /**
     * @brief Testuje bazowy endpoint bez końcówki liczbowej.
     */
    void testGetBaseEndpointWithoutNumericSuffix();

    /**
     * @brief Testuje zachowanie metody getBaseEndpoint dla pustego stringa.
     */
    void testGetBaseEndpointEmpty();

    /**
     * @brief Testuje przypadek, gdy końcowy fragment endpointu nie jest liczbą.
     */
    void testGetBaseEndpointNoNumericSuffix();

    /**
     * @brief Testuje usunięcie ID z końca endpointu.
     */
    void testGetBaseEndpointWithEndpointEndingWithID();

    /**
     * @brief Sprawdza, czy flaga isArchival jest domyślnie ustawiona na false.
     */
    void testDefaultArchivalFlag();

    /**
     * @brief Sprawdza, czy pole date jest domyślnie puste.
     */
    void testDefaultDate();
};

#endif // TESTREQUESTCONTEXT_H
