#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QObject>
#include <QThread>
#include "jsonrepository.h"
#include <appcontroller.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QThread *repoThread = new QThread;
    JsonRepository *repo = new JsonRepository("cache");
    AppController *controller = new AppController(repo);
    repo->moveToThread(repoThread);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    QObject::connect(&app, &QCoreApplication::aboutToQuit, repoThread, &QThread::quit);
    QObject::connect(repoThread, &QThread::finished, repoThread, &QObject::deleteLater);

    engine.rootContext()->setContextProperty("appController", controller);
    repoThread->start();
    engine.loadFromModule("polution_viewer", "MainView");
    if(engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
