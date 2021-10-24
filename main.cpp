#include "TextStats.h"
#include "QmlContextInterface.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QDebug>
#include <QQmlContext>

#include <chrono>

auto timeFuncInvocation =
    [](const auto&& func, auto&&... params) {
        // get time before function invocation
        const auto& start = std::chrono::high_resolution_clock::now();
        // function invocation using perfect forwarding
        std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
        // get time after function invocation
        const auto& stop = std::chrono::high_resolution_clock::now();
        return stop - start;
     };


int main(int argc, char *argv[])
{
    QThread thread;
    TextStats stats;
    stats.setFileName("file3.txt");
    stats.moveToThread(&thread);
    QObject::connect(&thread, &QThread::started, &stats, &TextStats::start);

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


    qRegisterMetaType<SortedResults>("SortedResults");

    QObject::connect(&stats, &TextStats::statsUpdated,
            &contextInterface, &QmlContextInterface::onStatsUpdate);

    thread.start();

    return app.exec();
}
