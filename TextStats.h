#include <QString>

#include <QPair>
#include <QList>
#include <QHash>
#include <QThread>
#include <QString>
#include <QMap>


class TextStats : public QObject {
    Q_OBJECT

    typedef QMap<QString, int> SortedResults;
    Q_PROPERTY(StringIntMap results READ results WRITE setResults)
    Q_PROPERTY(QString getFileName READ getFileName)

    typedef QPair<QString,int>  word_stat_t;
    QList<word_stat_t> top;
    QHash<QString, int> rest;
    const int TOP_SIZE = 15;

public:
    TextStats(QObject *const parent = 0) {};
    ~TextStats() {};

    void updateFromString(const QString& text);
    void setFileName(const QString& filename);
    void printTop();
    const QString& getFileName() const;

public slots:
    void start();
    void onStatsRequested();
signals:
    void statsUpdated(const QList<word_stat_t>);

private:
    QString filename;
    SortedResults m_results;
};

