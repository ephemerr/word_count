// #include <QGuiApplication>
// #include <QQmlApplicationEngine>

#include "TextStats.h"

#include <string>
#include <iostream>
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

void stats_counter(const QString& filename)
{
    TextStats stats;
    stats.updateFromFile(filename);
    stats.printTop();
}

int main(int argc, char *argv[])
{
// #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//     QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
// #endif
//
//     QGuiApplication app(argc, argv);
//
//     QQmlApplicationEngine engine;
//     const QUrl url(QStringLiteral("qrc:/main.qml"));
//     QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                      &app, [url](QObject *obj, const QUrl &objUrl) {
//         if (!obj && url == objUrl)
//             QCoreApplication::exit(-1);
//     }, Qt::QueuedConnection);
//     engine.load(url);

    // feed the text into the container

    std::chrono::duration<double, std::milli> du = timeFuncInvocation(stats_counter, "file4.txt");
    std::cout << std::endl << "result: " << du.count() << std::endl;


    // list words with counters by order of appearance
    // for( word_counter::right_const_iterator
    //         wit = wc.right.begin(), wit_end = wc.right.end();
    //
    //      wit != wit_end; ++wit )
    // {
    //     std::cout << wit->second << ": " << wit->first << std::endl;
    // }

    return 0;//app.exec();
}
