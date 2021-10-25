#include <TextStats.h>

#include <QDebug>
#include <QString>
#include <QFile>
#include <QUrl>

#include <chrono>

void TextStats::updateFromString(const QString& token)
{
    auto in_best = best.find(token);
    if (in_best != best.end())
    {
        (*in_best)++;
    }
    else
    {
        if (best.size() < 15)
        {
            best[token] = 1;
        }
        else
        {
            auto in_rest = rest.find(token);
            if (in_rest != rest.end())
            {
                (*in_rest)++;
                auto min = std::min_element(best.begin(), best.end(),
                    [] (const auto& p1, const auto& p2) {
                        return p1 < p2;
                    }
                );
                if (*min < *in_rest)
                {
                   best[in_rest.key()] = in_rest.value();
                   rest[min.key()] = min.value();
                   best.erase(min);
                   rest.erase(in_rest);
                }
            }
            else
            {
                rest[token] = 1;
            }
        }
    }
}

void TextStats::processFile(const QString& filename) {

    const auto& start = std::chrono::high_resolution_clock::now();

    rest.clear();
    best.clear();

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
        int percent = (readed*100)/file.size();
        emit statsUpdated(best, percent);
    }
    while( !chunk.isEmpty() );

    const auto& stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> du = stop - start;;
    qDebug() << "time spent: " << du.count();

    emit finished();
}

void TextStats::printTop()
{
    qDebug() << "Print top";
    for (auto &elem : best.toStdMap())
    {
        qDebug() << elem.first << ": " << elem.second ;
    }
    qDebug() << "unique words: " << rest.size() + best.size();
}

