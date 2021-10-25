#include "TextStats.h"
#include "QmlContextInterface.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QDebug>
#include <QQmlContext>
#include <qobject.h>
#include <qqmlapplicationengine.h>




int main(int argc, char *argv[])
{
    QThread thread;
    TextStats stats;
    stats.moveToThread(&thread);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url] (QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    QmlContextInterface contextInterface;
    engine.rootContext()->setContextProperty("context_interface",&contextInterface);
    engine.load(url);


    qRegisterMetaType<QMap<QString, int>>("SortedResults");

    QObject::connect(&stats, &TextStats::statsUpdated,
            &contextInterface, &QmlContextInterface::onStatsUpdate);

    QObject::connect(&stats, &TextStats::finished,
            &contextInterface, &QmlContextInterface::finished);

    QObject::connect( &contextInterface, &QmlContextInterface::fileChoosed,
            &stats, &TextStats::processFile);

    QObject::connect( &app, &QCoreApplication::aboutToQuit, &thread, &QThread::quit);

    thread.start();

    return app.exec();
}
