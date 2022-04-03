#ifndef TESTSENGINE_HPP
#define TESTSENGINE_HPP

#include <QObject>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


namespace diaryth {


    struct CardQuestion
    {
        QString text;

    };


    class TestsEngine : public QObject
    {
        Q_OBJECT

    public:
        TestsEngine() = default;

        void parseJSON(const QString& json);
        void parseFromFile(const QString& filename);

    private:

        void addQuestions(const QJsonArray& questions);

        bool hasRootErros(const QJsonDocument& root);

        QString readTextFile(const QString& filename);


        QString _testName;
        QString _testDescription;


        //Alt questions relation + sequence
        //Lie detection markers
    };


}


#endif // TESTSENGINE_HPP
