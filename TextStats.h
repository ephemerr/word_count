#include <QString>

#include <QPair>
#include <QList>
#include <QHash>


class TextStats {
    typedef QPair<QString,int>  word_stat_t;
    QList<word_stat_t> top;
    QHash<QString, int> rest;

public:
    void updateFromString(const QString& text);
    void updateFromFile(const QString& filename);
    void printTop();
    const int TOP_SIZE = 15;
};

