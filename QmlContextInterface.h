#include <QObject>
#include <QMap>
#include <QDebug>

class QmlContextInterface: public QObject {
    Q_OBJECT
public:
    QmlContextInterface(QObject *const parent = 0) {};
    ~QmlContextInterface() {};
    typedef QMap<QString, int> SortedResults;
    Q_PROPERTY(SortedResults stats MEMBER m_stats NOTIFY statsUpdated)
    Q_PROPERTY(int percent MEMBER m_percent NOTIFY percentUpdated)

public slots:
    void onStatsUpdate(const SortedResults& stats, int percent) {
        m_stats = stats;
        m_percent = percent;
        emit statsUpdated();
        emit percentUpdated();
    }
signals:
    void statsUpdated();
    void percentUpdated();
private:
    SortedResults m_stats;
    int m_percent;
};

