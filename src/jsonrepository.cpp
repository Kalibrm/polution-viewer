#include "jsonrepository.h"

#include <QDir>
#include <QEventLoop>
#include <QNetworkInformation>
#include <QSaveFile>

JsonRepository::JsonRepository(const QString &cacheDir, QObject *parent)
    : QObject{parent}, m_cacheDir(cacheDir)
{
    QDir dir(m_cacheDir);
    if(!dir.exists())
        dir.mkdir(m_cacheDir);

    connect(&m_apiManager, &ApiManager::rawDataReady, this, &JsonRepository::processApiResponse);
    connect(&m_apiManager, &ApiManager::errorOccured, this, &JsonRepository::errorOccured);
}

void JsonRepository::processApiResponse(const RequestContext &context, const QJsonArray &array, bool isPartial)
{
    const QJsonArray extracted = extractRelevantData(array);
    if(isPartial){
        emit dataReady(context, extracted);
        return;
    }

    QString filePath = getCacheFilePath(context);
    ensureDirectoryForFile(filePath);
    QSaveFile file(filePath);

    if(file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(extracted);
        file.write(doc.toJson());
        file.commit();
        qInfo() << "Zapisano dane do" << filePath;
    } else {
        qWarning() << "Nie udało się zapisać danych do" << filePath;
    }

    emit dataReady(context, extracted);
}

void JsonRepository::loadData(const RequestContext &context)
{
    const QString filePath = getCacheFilePath(context);

    // qInfo() << filePath;

    try {
        if(!isNetworkAvailable()) {
            if(isCacheAvailable(filePath)) {
                QJsonArray cached = readFromCache(filePath);
                emit dataReady(context, cached);
                return;
            } else {
                throw std::runtime_error("Brak internetu i brak cache");
            }
        }

        if(isCacheAvailable(filePath) && !isCacheExpired(filePath, RequestContext::getBaseEndpoint(context.endpoint))) {
            QJsonArray cached = readFromCache(filePath);
            emit dataReady(context, cached);
            return;
        }

        m_apiManager.fetchPagedData(context);

    } catch(const std::exception &e) {
        emit errorOccured(context, e.what());
        emit dataReady(context, QJsonArray());
    }
}

QString JsonRepository::getCacheFilePath(const RequestContext &context) const
{
    QString sep = QDir::separator();
    QStringList pathParts = {m_cacheDir};

    const QString baseEndpoint = RequestContext::getBaseEndpoint(context.endpoint);

    if(baseEndpoint == "station/findAll") {
        pathParts << "stations.json";
    } else if (baseEndpoint == "station/sensors") {
        pathParts << QString::number(context.stationId) << "sensors.json";
    } else if (baseEndpoint == "data/getData") {
        pathParts << QString::number(context.stationId) << QString::number(context.sensorId) << "current.json";
    } else {
        pathParts << "unknown.json";
    }

    return QDir::cleanPath(pathParts.join(sep));
}

void JsonRepository::ensureDirectoryForFile(const QString &filePath) const
{
    QFileInfo info(filePath);
    QDir dir = info.dir();
    if(!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }
}

QJsonArray JsonRepository::extractRelevantData(const QJsonArray &input)
{
    QJsonArray extracted;

    for(const QJsonValue &val : input) {
        if(!val.isObject())
            continue;

        const QJsonObject obj = val.toObject();
        for(auto it = obj.begin(); it != obj.end(); ++it) {
            const QString &key = it.key();

            if(key.contains("Lista", Qt::CaseInsensitive)) {
                if(it.value().isArray()) {
                    const QJsonArray innerArray = it.value().toArray();
                    for (const QJsonValue &innerVal : innerArray) {
                        extracted.append(innerVal);
                    }
                }
            }
        }
    }

    return extracted;
}

bool JsonRepository::isCacheAvailable(const QString &filePath) const
{
    QFileInfo fileInfo(filePath);
    return fileInfo.exists();
}

bool JsonRepository::isCacheExpired(const QString &filePath, const QString &endpoint) const
{
    QFileInfo fileInfo(filePath);
    if(!fileInfo.exists())
        return true;

    static const QMap<QString, int> endpointMaxAgeSeconds = {
        {"station/findAll", 24 * 3600},
        {"station/sensors", 24 * 3600},
        {"data/getData", 10 * 60}
    };

    QString baseEndpoint = RequestContext::getBaseEndpoint(endpoint);
    int maxAge = endpointMaxAgeSeconds.value(baseEndpoint, 3600);

    QDateTime lastModified = fileInfo.lastModified();
    return lastModified.secsTo(QDateTime::currentDateTime()) > maxAge;
}

QJsonArray JsonRepository::readFromCache(const QString &filePath) const
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Nie udało się otworzyć pliku cache do odczytu:" << filePath;
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if(parseError.error != QJsonParseError::NoError) {
        qWarning() << "Błąd parsowania JSON w pliku" << filePath << ":" << parseError.errorString();
        return {};
    }

    if(!doc.isArray()) {
        qWarning() << "Plik cache nie zawiera JSON Array:" << filePath;
        return {};
    }

    return doc.array();
}

bool JsonRepository::isNetworkAvailable()
{
    QNetworkAccessManager nam;
    QNetworkRequest request(QUrl("http://clients3.google.com/generate_204"));

    QNetworkReply *reply = nam.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool online = (reply->error() == QNetworkReply::NoError);
    reply->deleteLater();
    return online;
}
