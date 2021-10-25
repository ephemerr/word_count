#include <TextStats.h>

#include <QDebug>
#include <QString>
#include <QFile>
#include <QUrl>

#include <chrono>

auto timeFuncInvocation =
    [](const auto&& func, auto&&... params) {
        const auto& start = std::chrono::high_resolution_clock::now();
        std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
        const auto& stop = std::chrono::high_resolution_clock::now();
        return stop - start;
     };

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

void TextStats::start()
{
    const auto& start = std::chrono::high_resolution_clock::now();

    top.clear();
    rest.clear();

    QFile file(QUrl(filename).toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Could not open file: " << filename;
        emit finished();
        return;
    }
    QString text;
    QByteArray chunk;
    quint64 readed = 0;
    do {
        try {
            chunk = file.read( CHUNK_SIZE );
            readed += chunk.size();
        } catch( ... ) {
            qWarning() << "Error reading file.";
            emit finished();
            return;
        }
        QTextStream stream(&chunk);
        while ( !(stream >> text).atEnd() )
        {
            updateFromString(text.toLower());
        }
        SortedResults results;
        for(auto &item : top)
        {
            results[item.first] = item.second;
        }
        int percent = (readed*100)/file.size();
        emit statsUpdated(results, percent);
        qDebug() << readed << " " << file.size() << " " << percent;
    }
    while( !chunk.isEmpty() );

    const auto& stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> du = stop - start;;
    qDebug() << "time spent: " << du.count();

    emit finished();
}

void TextStats::processFile(const QString& filename_) {
    this->filename = filename_;
    start();
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

