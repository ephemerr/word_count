#include <TextStats.h>

#include <QDebug>
#include <QString>
#include <QFile>
#include <QRegExp>

void TextStats::updateFromString(const QString& token)
{
    int found_in_top = -1;
    for ( int i = 0; i < top.size(); i++ )
    {
        auto &item = top[i];

        if (found_in_top == -1)
        {
            // search for token in list and increment it's counter
            if (item.first == token)
            {
                item.second++;
                found_in_top = i;
            }
        }
        else
        {
            // after incrementation bubble value up
            if (top[i].second < top[found_in_top].second)
            {
                top.swapItemsAt(i, found_in_top);
                found_in_top = i;
            }
        }

    }
    if (found_in_top == -1)
    {
        word_stat_t new_stat = {token, 1};
        if (top.size() < TOP_SIZE)
        {
            top.push_front(new_stat);
        }
        else
        {
            // value not present in top then search it among the rest
            auto found = rest.find(new_stat.first);
            if (found != rest.end())
            {
                // if value found among the rest we should increment it and check if it should be placed to top
                new_stat.second = ++(found.value());
                if (new_stat.second > top[0].second)
                {
                    auto i = 0;
                    for(; i < TOP_SIZE; i++)
                    {
                        if (new_stat.second < top[i].second)
                        {
                            // find value in the top that would be bigger than updated value and insert before it
                            top.insert(i, new_stat);
                            break;
                        }
                    }
                    // save to map and remove dublicates
                    rest[top[0].first] = top[0].second;
                    top.pop_front();
                    rest.erase(found);
                }
            }
            else
            {
                // save new value to map
                rest[new_stat.first] = new_stat.second;
            }
        }
    }
}

void TextStats::updateFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    QString text;
    while ( !(stream >> text).atEnd() )
    {
        updateFromString(text.toLower());
    }

}

void TextStats::printTop()
{
    qDebug() << "Print top";
    for (auto &elem : top)
    {
        qDebug() << elem.first << ": " << elem.second ;
    }
    qDebug() << "unique words: " << rest.size() + top.size();
}
