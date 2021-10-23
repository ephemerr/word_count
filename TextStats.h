#include <QString>

#include <utility>
#include <list>
#include <map>

class TextStats {
    typedef std::pair<const QString,int>  word_stat_t;
    std::list<word_stat_t> top;
    std::map<QString, int> rest;

public:
    void updateFromString(const QString& text);
    void updateFromFile(const QString& filename);
    void printTop();
};

