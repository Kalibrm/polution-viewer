#include "testjsonrepository.h"
#include <QtTest>
#include "../src/JsonRepository.h"

void TestJsonRepository::testGetCacheFilePath_stationFindAll()
{
    JsonRepository repository("cacheDir");
    RequestContext context;
    context.endpoint = "station/findAll";

    QString filePath = repository.getCacheFilePath(context);

    QString expectedPath = QDir::cleanPath("cacheDir/stations.json");
    QCOMPARE(filePath, expectedPath);
}

void TestJsonRepository::testGetCacheFilePath_stationSensors()
{
    JsonRepository repository("cacheDir");
    RequestContext context;
    context.endpoint = "station/sensors";
    context.stationId = 123;

    QString filePath = repository.getCacheFilePath(context);

    QString expectedPath = QDir::cleanPath("cacheDir/123/sensors.json");
    QCOMPARE(filePath, expectedPath);
}

void TestJsonRepository::testGetCacheFilePath_dataGetData()
{
    JsonRepository repository("cacheDir");
    RequestContext context;
    context.endpoint = "data/getData";
    context.stationId = 123;
    context.sensorId = 456;

    QString filePath = repository.getCacheFilePath(context);

    QString expectedPath = QDir::cleanPath("cacheDir/123/456/current.json");
    QCOMPARE(filePath, expectedPath);
}

void TestJsonRepository::testGetCacheFilePath_unknownEndpoint()
{
    JsonRepository repository("cacheDir");
    RequestContext context;
    context.endpoint = "unknown/endpoint";

    QString filePath = repository.getCacheFilePath(context);

    QString expectedPath = QDir::cleanPath("cacheDir/unknown.json");
    QCOMPARE(filePath, expectedPath);
}
