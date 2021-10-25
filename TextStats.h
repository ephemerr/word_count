#pragma once

#include "SortedResults.h"

#include <QString>

#include <QPair>
#include <QList>
#include <QHash>
#include <QThread>
#include <QString>
#include <QMap>
#include <QMutex>

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
    void statsUpdated(const SortedResults, int percent);
    void finished();

private:
    QString filename;
    typedef QPair<QString,int>  word_stat_t;
    QList<word_stat_t> top;
    QHash<QString, int> rest;
};

