// #include <QGuiApplication>
// #include <QQmlApplicationEngine>

#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <chrono>

#include <boost/config.hpp>
#include <boost/tokenizer.hpp>
#include <boost/bimap/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/detail/concept_tags.hpp>

using namespace boost::bimaps;

struct counter {
    counter() : c(0) {}
    counter& operator++() { ++c; return *this; }
    unsigned int operator++(int) { return c++; }
    operator const unsigned int() const { return c; }
    private:
    unsigned int c;
};

typedef bimap
<
    multiset_of< counter >
    ,unordered_set_of< std::string >
> word_counter;

std::string text=
    "Relations between data in the STL are represented with maps."
    "A map is a directed relation, by using it you are representing "
    "a mapping. In this directed relation, the first type is related to "
    "the second type but it is not true that the inverse relationship "
    "holds. This is useful in a lot of situations, but there are some "
    "relationships that are bidirectional by nature.";

typedef boost::tokenizer<boost::char_separator<char> > text_tokenizer;

auto timeFuncInvocation =
    [](auto&& func, auto&&... params) {
        // get time before function invocation
        const auto& start = std::chrono::high_resolution_clock::now();
        // function invocation using perfect forwarding
        std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
        // get time after function invocation
        const auto& stop = std::chrono::high_resolution_clock::now();
        return stop - start;
     };

void vector_counter(const std::string& filename)
{
    std::ifstream file(filename);
    file.width(64*1024);
    std::string text;

    typedef std::pair<std::string, int> word_stat;
    std::vector<word_stat> simple_counter;

    while (file >> text)
    {
        text_tokenizer tok(text,boost::char_separator<char>(" \t\n.,;:!?(){}[]?'\""));

        for( auto it = tok.begin(), it_end = tok.end(); it != it_end ; ++it )
        {
            int i = 0;
            for ( i = 0; i < simple_counter.size(); i++)
            {
                auto &word = simple_counter[i];
                if (word.first == *it)
                {
                    word.second++;
                    break;
                }
            }
            if (i != simple_counter.size())
            {
                for(int j=i; j; j--)
                {
                    if(simple_counter[j].second > simple_counter[j-1].second)
                    {
                        std::iter_swap(simple_counter.begin() + j, simple_counter.begin() + j-1);
                    }
                    else break;
                }
            }
            else
            {
                simple_counter.push_back(word_stat(*it,1));
            }
        }
    }

    for (int i=0; i < 15 && i< simple_counter.size(); i++)
    {
        auto &elem = simple_counter[i];
        std::cout << elem.first << ": " << elem.second << std::endl;
    }
}

void vemap_counter(const std::string &filename)
{
    std::ifstream file(filename);
    file.width(64*1024);
    std::string text;

    typedef std::pair<std::string,int>  word_stat;
    std::list<word_stat> top;
    std::map<std::string, int> counter;
    struct word_stat_comp
    {
        bool operator()(word_stat const& lhs, word_stat const& rhs)
        {
            return lhs.second < rhs.second;
        }
    };

    while (file >> text)
    {
        text_tokenizer tok(text,boost::char_separator<char>(" \t\n.,;:!?(){}[]?'\""));

        for( auto it = tok.begin(), it_end = tok.end(); it != it_end ; ++it )
        {
            auto found_in_top = top.rend();
            for ( auto i = top.rbegin(); i != top.rend(); i++ )
            {
                if (found_in_top == top.rend())
                {
                    if (i->first == *it)
                    {
                        i->second++;
                        found_in_top = i;
                        // top.sort([](auto &x, auto &y)
                        // {
                        //     return x.second > y.second;
                        // });
                        // break;
                    }
                }
                else if ( i->second >= found_in_top->second)
                {
                    top.splice(i.base(), top, (++found_in_top).base());
                    break;
                }

            }
            if (found_in_top == top.rend())
            {
                // value not present in top 15 then search it among the rest
                word_stat new_stat = {*it, 1};
                auto found = counter.find(*it);
                if (found != end(counter))
                {
                    // if value found among the rest we should increment it and check if it should be placed to top
                    new_stat.second = found->second + 1;
                    auto i = top.rbegin();
                    for(; i != top.rend(); i++)
                    {
                        // find value in the top that would be less than updatet value
                        if (i->second > new_stat.second) break;
                    }
                    if (i != top.rbegin() || i == top.rend())
                    {
                        // if lesser values found in the top: insert new value
                        if (i == top.rend())
                        {
                            top.push_front(new_stat);
                        }
                        else
                        {
                            top.insert(i.base(), new_stat);
                        }
                        counter.erase(found);
                        if (top.size() > 15)
                        {
                            // pop last value from the top to the rest
                            auto drop_stat = *top.rbegin();
                            top.pop_back();
                            counter.insert(begin(counter), drop_stat);
                        }
                    }
                    else if (top.size() < 15)
                    {
                        top.push_back(new_stat);
                        counter.erase(found);
                    }
                }
                else
                {
                    counter.insert(begin(counter), new_stat);
                }
            }
        }
    }

    for (auto &elem : top)
    {
        std::cout << elem.first << ": " << elem.second<< std::endl;
    }

}

// int bimap_counter()
// {
//     //[ code_repetitions_counter
//
//     typedef bimap
//     <
//         unordered_set_of< std::string >,
//         multiset_of< counter >
//     > word_counter;
//
//     typedef boost::tokenizer<boost::char_separator<char> > text_tokenizer;
//
//     std::string text=
//         "Relations between data in the STL are represented with maps."
//         "A map is a directed relation, by using it you are representing "
//         "a mapping. In this directed relation, the first type is related to "
//         "the second type but it is not true that the inverse relationship "
//         "holds. This is useful in a lot of situations, but there are some "
//         "relationships that are bidirectional by nature.";
//
//     // feed the text into the container
//     word_counter   wc;
//     text_tokenizer tok(text,boost::char_separator<char>(" \t\n.,;:!?'\"-"));
//
//     for( text_tokenizer::const_iterator it = tok.begin(), it_end = tok.end();
//          it != it_end ; ++it )
//     {
//         /*<< Because the right collection type is `list_of`, the right data
//              is not used a key and can be modified in the same way as with
//              standard maps. >>*/
//         ++ wc.left[*it];
//     }
//
//     // list words with counters by order of appearance
//     /*<< When we insert the elements using the left map view, the element
//          is inserted at the end of the list. >>*/
//     for( word_counter::right_const_iterator
//             wit = wc.right.begin(), wit_end = wc.right.end();
//
//          wit != wit_end; ++wit )
//     {
//         std::cout << wit->second << ": " << wit->first;
//     }
//     //]
//
//     return 0;
// }

int main(int argc, char *argv[])
{
// #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//     QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
// #endif
//
//     QGuiApplication app(argc, argv);
//
//     QQmlApplicationEngine engine;
//     const QUrl url(QStringLiteral("qrc:/main.qml"));
//     QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                      &app, [url](QObject *obj, const QUrl &objUrl) {
//         if (!obj && url == objUrl)
//             QCoreApplication::exit(-1);
//     }, Qt::QueuedConnection);
//     engine.load(url);

    // feed the text into the container

    std::chrono::duration<double, std::milli> du0 = timeFuncInvocation(vemap_counter, "file4.txt");
    std::cout << std::endl << "result: " << du0.count() << std::endl;
    std::chrono::duration<double, std::milli> du = timeFuncInvocation(vector_counter, "file4.txt");
    std::cout << std::endl << "result: " << du.count() << std::endl;


    // list words with counters by order of appearance
    // for( word_counter::right_const_iterator
    //         wit = wc.right.begin(), wit_end = wc.right.end();
    //
    //      wit != wit_end; ++wit )
    // {
    //     std::cout << wit->second << ": " << wit->first << std::endl;
    // }

    return 0;//app.exec();
}
