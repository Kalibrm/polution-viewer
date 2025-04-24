#include "apimanager.h"
#include <qurlquery.h>
#include <QDebug>

ApiManager::ApiManager(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ApiManager::onReplyFinished);
    connect(&retryTimer, &QTimer::timeout, this, &ApiManager::retryFailedPages);
    retryTimer.start(60000);
    retryTimer.setSingleShot(false);
    retryTimer.start();

}

void ApiManager::queueRetry(const QUuid &id, int page) {
    RetryState &state = retryQueue[id];

    if(!state.missingPages.contains(page))
        retryQueue[id].missingPages.append(page);
}

void ApiManager::fetchPagedData(const RequestContext &context) {
    requestContexts.insert(context.id, context);
    requestStates[context.id] = RequestState{{}, {}, -1};

    retryQueue.remove(context.id);
    fetchPage(context.id, 0);
}

void ApiManager::retryFailedPages() {
    for (auto it = retryQueue.begin(); it != retryQueue.end(); ++it) {
        const QUuid &id = it.key();
        RetryState &state = it.value();
        const auto &params = requestContexts[id].params;

        const QList<int> &pages = state.missingPages;
        for(const int &page : pages) {
            QUrl url(requestContexts[id].endpoint);
            QUrlQuery query;

            for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
                query.addQueryItem(it.key(), it.value());
            }

            query.addQueryItem("page", QString::number(page));
            url.setQuery(query);

            QNetworkRequest request(url);
            request.setAttribute(QNetworkRequest::User, id);
            manager->get(request);
        }
    }
}

void ApiManager::fetchPage(const QUuid &id, int page)
{
    if(!requestContexts.contains(id)) return;

    const RequestContext &context = requestContexts[id];
    QUrl url(BASE_URL + context.endpoint);
    QUrlQuery query;

    for (auto it = context.params.constBegin(); it != context.params.constEnd(); ++it) {
        query.addQueryItem(it.key(), it.value());
    }

    query.addQueryItem("page", QString::number(page));
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User, id);
    manager->get(request);
}

void ApiManager::onReplyFinished(QNetworkReply *reply) {
    QUuid id = reply->request().attribute(QNetworkRequest::User).toUuid();

    QUrl fullUrl = reply->url();
    RequestState &state = requestStates[id];
    RequestContext &context = requestContexts[id];
    int page = QUrlQuery(fullUrl).queryItemValue("page").toInt();

    if(state.pageData.contains(page)) {
        reply->deleteLater();
        return;
    }

    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray data = reply->readAll();
    reply->deleteLater();

    bool success = (httpStatus == 200 && reply->error() == QNetworkReply::NoError);
    if(success) {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        int totalPages = obj.value("totalPages").toInt(1);

        if(state.totalPages == -1)
            state.totalPages = totalPages;

        QStringList keys = {"@context", "totalPages", "links", "meta"};
        for (const QString &key : keys) {
            obj.remove(key);
        }

        state.pageData[page] = obj;
        state.pagesCompleted[page] = true;

        if (page == 0 && state.totalPages > 1) {
            for (int i = 1; i < state.totalPages; ++i) {
                if (!state.pageData.contains(i)) {
                    fetchPage(id, i);
                }
            }
        }
    } else {
        QString errorMsg = QString("Błąd HTTP %1: %2").arg(httpStatus).arg(reply->errorString());
        emit errorOccured(context, errorMsg);
        qWarning() << errorMsg;
        state.pagesCompleted[page] = false;
        if(httpStatus != 400)
            queueRetry(id, page);
    }


    const int totalPages = state.totalPages;
    if(state.pagesCompleted.size() == totalPages) {
        QJsonArray finalArray;

        RequestContext &context = requestContexts[id];

        const auto &pages = state.pageData;
        for (const auto &value : pages)
            finalArray.append(value);


        bool hasAnyFailed = false;
        for (auto it = state.pagesCompleted.begin(); it != state.pagesCompleted.end(); ++it) {
            if(!it.value()) {
                hasAnyFailed = true;
                break;
            }
        }

        if(hasAnyFailed){
            emit rawDataReady(context, finalArray, true);
        }else {
            emit rawDataReady(context, finalArray);
            requestStates.remove(id);
            retryQueue.remove(id);
            requestContexts.remove(id);
        }
    }
}
