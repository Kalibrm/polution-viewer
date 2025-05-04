#include "testrequestcontext.h"
#include <QTest>
#include "../src/requestcontext.h"

void TestRequestContext::testDefaultConstructor() {
    RequestContext context;

    QVERIFY(!context.id.isNull());
}

void TestRequestContext::testParameterizedConstructor() {
    QMap<QString, QString> params;
    params["key1"] = "value1";
    RequestContext context("station/sensors/%1", params, 42, 1001);

    QCOMPARE(context.endpoint, QString("station/sensors/1001"));
    QCOMPARE(context.stationId, 42);
    QCOMPARE(context.sensorId, 1001);
    QCOMPARE(context.params["key1"], QString("value1"));
}

void TestRequestContext::testGetBaseEndpointWithNumericSuffix() {
    QString endpoint = "station/sensors/123";
    QString baseEndpoint = RequestContext::getBaseEndpoint(endpoint);
    QCOMPARE(baseEndpoint, QString("station/sensors"));
}

void TestRequestContext::testGetBaseEndpointWithoutNumericSuffix() {
    QString endpoint = "station/sensors";
    QString baseEndpoint = RequestContext::getBaseEndpoint(endpoint);
    QCOMPARE(baseEndpoint, QString("station/sensors"));
}

void TestRequestContext::testGetBaseEndpointEmpty() {
    QString endpoint = "";
    QString baseEndpoint = RequestContext::getBaseEndpoint(endpoint);
    QCOMPARE(baseEndpoint, QString(""));
}

void TestRequestContext::testGetBaseEndpointNoNumericSuffix() {
    QString endpoint = "data/getData";
    QString baseEndpoint = RequestContext::getBaseEndpoint(endpoint);
    QCOMPARE(baseEndpoint, QString("data/getData"));
}

void TestRequestContext::testGetBaseEndpointWithEndpointEndingWithID() {
    QString endpoint = "data/getData/1234";
    QString baseEndpoint = RequestContext::getBaseEndpoint(endpoint);
    QCOMPARE(baseEndpoint, QString("data/getData"));
}

void TestRequestContext::testDefaultArchivalFlag() {
    RequestContext context;
    QCOMPARE(context.isArchival, false);
}

void TestRequestContext::testDefaultDate() {
    RequestContext context;
    QCOMPARE(context.date, QString(""));
}
