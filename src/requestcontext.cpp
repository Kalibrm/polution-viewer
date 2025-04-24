#include "requestcontext.h"

RequestContext::RequestContext() : id(QUuid::createUuid()) {}

RequestContext::RequestContext(const QString &endpointTemplate, const QMap<QString, QString> &params, const int &stationId, const int &sensorId)
    : id(QUuid::createUuid()), params(params), stationId(stationId), sensorId(sensorId)
{
    if (endpointTemplate.contains("%1")) {
        if (sensorId) {
            endpoint = endpointTemplate.arg(sensorId);
        } else if (stationId) {
            endpoint = endpointTemplate.arg(stationId);
        } else {
            endpoint = endpointTemplate;
        }
    } else {
        endpoint = endpointTemplate;
    }
}

QString RequestContext::getBaseEndpoint(const QString &endpoint)
{
    QStringList parts = endpoint.split('/');

    if(parts.empty())
        return endpoint;

    bool isNumber = false;
    parts.last().toInt(&isNumber);
    if (isNumber)
        parts.removeLast();

    return parts.join('/');
}
