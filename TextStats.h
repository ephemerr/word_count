#pragma once

#include <QString>
#include <QHash>
#include <QObject>
#include <QMap>

class TextStats : public QObject {
    Q_OBJECT

    static const int TOP_SIZE = 15;
    static const int CHUNK_SIZE = 1024*1024;

public:
    TextStats(QObject *const parent = 0) {};
    ~TextStats() {};

    void updateFromString(const QString& text);
    void processFile(const QString& filename);
    void printTop();

public slots:
    void start();
signals:
    void statsUpdated(const QMap<QString, int>, int percent);
    void finished();

private:
    QString filename;
    QHash<QString, int> rest;
    QMap<QString, int> best;
};

