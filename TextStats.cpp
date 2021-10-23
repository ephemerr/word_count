#include <TextStats.h>

#include <QDebug>
#include <QString>
#include <QFile>
#include <QRegExp>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <locale>

void TextStats::updateFromString(const QString& token)
{
    auto found_in_top = top.end();
    for ( auto i = top.begin(); i != top.end(); i++ )
    {
        if (found_in_top == top.end())
        {
            if (i->first == token)
            {
                i->second++;
                found_in_top = i;
                top.sort([](auto &x, auto &y)
                {
                    return x.second < y.second;
                });
                break;
            }
        }
        // else if ( i->second >= found_in_top->second)
        // {
        //     top.splice(i.base(), top, (++found_in_top).base());
        //     break;
        // }

    }
    if (found_in_top == top.end())
    {
        word_stat_t new_stat = {token, 1};
        if (top.size() < 15)
        {
            top.push_front(new_stat);
        }
        else
        {
            // value not present in top 15 then search it among the rest
            auto found = rest.find(new_stat.first);
            if (found != rest.end())
            {
                // if value found among the rest we should increment it and check if it should be placed to top
                new_stat.second = ++found->second;
                if (new_stat.second > top.begin()->second)
                {
                    auto i = top.begin();
                    for(; i != top.end(); i++)
                    {
                        // find value in the top that would be less than updatet value
                        if (i->second > new_stat.second)
                        {
                            top.insert(i, new_stat);
                            break;
                        }
                    }
                    if (i == top.end())
                    {
                        top.push_back(new_stat);
                    }
                    auto drop_stat = *top.begin();
                    rest.insert(rest.begin(), drop_stat);
                    top.pop_front();
                    rest.erase(found);
                }
                else
                {
                    // std::cout << "not push " << new_stat.first << " " << new_stat.second;
                    // std::cout << " top first " << top.begin()->first << " " << top.begin()->second << std::endl;
                }
            }
            else
            {
                rest.insert(rest.begin(), new_stat);
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
    std::cout << "Print top" << std::endl;
    for (auto &elem : top)
    {
        qDebug() << elem.first << ": " << elem.second ;
    }
    qDebug() << "unique words: " << rest.size() + top.size();
}
