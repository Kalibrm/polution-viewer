#ifndef JSONREPOSITORY_H
#define JSONREPOSITORY_H

#pragma once

#include "apimanager.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

/**
 * @class JsonRepository
 * @brief Klasa odpowiedzialna za zarządzanie danymi JSON z API oraz cache.
 *
 * Obsługuje pobieranie, zapisywanie i odczyt danych w formacie JSON, wspierając
 * mechanizm cache oraz fallback offline w przypadku braku sieci.
 */
class JsonRepository : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor repozytorium JSON.
     *
     * Tworzy repozytorium danych lokalnych oraz ustawia katalog cache.
     * @param cacheDir Ścieżka do katalogu, gdzie przechowywany jest cache.
     * @param parent Wskaźnik na obiekt nadrzędny (dla Qt).
     */
    explicit JsonRepository(const QString &cacheDir, QObject *parent = nullptr);

    /**
     * @brief Sprawdza, czy sieć jest dostępna (próbuje połączyć się z Google).
     * @return true, jeśli połączenie sieciowe działa.
     */
    static bool isNetworkAvailable();

public slots:
    /**
     * @brief Przetwarza dane zwrócone przez API.
     *
     * Ekstrahuje dane z odpowiedzi i zapisuje je do cache, jeśli to nie jest fragmentaryczna odpowiedź.
     * Emituje sygnał dataReady.
     *
     * @param context Kontekst żądania (endpoint, ID, itd.).
     * @param doc Surowe dane z API.
     * @param isPartial Czy dane są częściowe i nie powinny być zapisywane.
     */
    void processApiResponse(const RequestContext &context, const QJsonArray &doc, bool isPartial = false);

    /**
     * @brief Wczytuje dane – z cache lub z API.
     *
     * W przypadku braku sieci i dostępnego cache – ładuje dane lokalne.
     * W innym wypadku pobiera dane przez API.
     *
     * @param context Kontekst żądania.
     */
    void loadData(const RequestContext &context);

signals:
    /**
     * @brief Sygnał emitowany, gdy dane są gotowe do użycia.
     * @param context Kontekst żądania.
     * @param data Przetworzone dane w formacie JSON.
     */
    void dataReady(const RequestContext &context, const QJsonArray &data);

    void errorOccured(const RequestContext &context, const QString &error);

private:
    /**
     * @brief Zwraca ścieżkę pliku cache na podstawie kontekstu.
     * @param context Kontekst żądania.
     * @return Ścieżka pliku do zapisu/odczytu danych.
     */
    QString getCacheFilePath(const RequestContext &context) const;

    /**
     * @brief Tworzy katalog, jeśli nie istnieje.
     * @param filePath Ścieżka do pliku, dla którego ma zostać zapewniona ścieżka.
     */
    void ensureDirectoryForFile(const QString &filePath) const;

    /**
     * @brief Zapisuje dane JSON do pliku na dysku.
     * @param path Ścieżka docelowa pliku.
     * @param data Dane w formacie JSON do zapisania.
     */
    void saveData(const QString &path, const QJsonArray &data);

    /**
     * @brief Wyciąga tylko istotne dane z tablicy JSON.
     *
     * Szuka pól zawierających listy (np. "ListaSensorow") i je agreguje.
     * @param input Surowe dane z API.
     * @return Przefiltrowane dane.
     */
    QJsonArray extractRelevantData(const QJsonArray &input);

    /**
     * @brief Sprawdza, czy cache istnieje.
     * @param filePath Ścieżka do pliku cache.
     * @return true, jeśli plik cache istnieje.
     */
    bool isCacheAvailable(const QString &filePath) const;

    /**
     * @brief Sprawdza, czy plik cache jest przestarzały.
     * @param filePath Ścieżka do pliku cache.
     * @param endpoint Endpoint API, do którego dane się odnoszą.
     * @return true, jeśli cache jest nieaktualny.
     */
    bool isCacheExpired(const QString &filePath, const QString &endpoint) const;

    /**
     * @brief Odczytuje dane z lokalnego pliku cache.
     * @param filePath Ścieżka do pliku.
     * @return Tablica JSON z cache.
     */
    QJsonArray readFromCache(const QString &filePath) const;

    ApiManager m_apiManager;  ///< Obiekt zarządzający połączeniami API.
    QString m_cacheDir;       ///< Główny katalog cache.
};

#endif // JSONREPOSITORY_H
