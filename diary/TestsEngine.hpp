#ifndef TESTSENGINE_HPP
#define TESTSENGINE_HPP

#include <QObject>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <vector>


namespace diaryth {



    struct TestOption
    {
        QString text;
        qreal rate;

        QString description;
    };

    struct TestQuestion
    {
        QString text;
        QString type; //radio, check, input

        int groupId = -1; //alt-question + other relations

        std::vector<TestOption> options;
    };


    class TestsEngine : public QObject
    {
        Q_OBJECT

    public:
        TestsEngine() = default;

        void parseJSON(const QString& json);
        void parseFromFile(const QString& filename);

        //when request question - option to mix them

    private:

        void addQuestions(const QJsonArray& questions);
        void addOptions(TestQuestion& question, const QJsonArray& options);

        bool hasRootErros(const QJsonDocument& root);

        QString readTextFile(const QString& filename);

        QString _testName;
        QString _testDescription;

        std::vector<TestQuestion> _questions;

        //Lie detection markers
    };


}


#endif // TESTSENGINE_HPP
