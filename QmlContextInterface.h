#pragma once

#include "SortedResults.h"

#include <QObject>
#include <QMap>
#include <QDebug>

class QmlContextInterface: public QObject {
    Q_OBJECT
public:
    QmlContextInterface(QObject *const parent = 0) {m_stats = {{"none", 0}};};
    ~QmlContextInterface() {};
    Q_PROPERTY(int percent MEMBER m_percent NOTIFY percentChanged)
    Q_PROPERTY(QStringList words READ getWords NOTIFY wordsChanged)
    Q_PROPERTY(QList<int> numbers READ getNumbers NOTIFY numbersChanged)

   const QStringList getWords() const { return m_stats.keys();}
   const QList<int> getNumbers() const { return m_stats.values();}

public slots:
    void onStatsUpdate(const SortedResults& stats, int percent) {
        m_stats = stats;
        m_percent = percent;
        emit wordsChanged();
        emit numbersChanged();
        emit percentChanged();
    }
signals:
    void finished();
    void wordsChanged();
    void numbersChanged();
    void percentChanged();
    void fileChoosed(const QString& filename);
private:
    SortedResults m_stats;
    int m_percent = 0;
};

