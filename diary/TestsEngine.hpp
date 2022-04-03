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
        //map? optionName->rate ?
    };

    struct TestRate
    {
        QString text;

        qreal rangeBegin;
        qreal rangeEnd;

        QString description;

        bool isInRange(qreal rate)
        {
            return rate >= rangeBegin &&
                   rate <= rangeEnd;
        }
    };



    class TestsEngine : public QObject
    {
        Q_OBJECT

    public:
        TestsEngine() = default;

        void parseJSON(const QString& json);
        void parseFromFile(const QString& filename);

        int questionsCount() { return _questions.size(); }

        QString getQuestionText(int idx) { return _questions[idx].text; }
        QString getQuestionType(int idx) { return _questions[idx].type; }
        int getQuestionGroupId(int idx) { return _questions[idx].groupId; }

        int getOptionsCount(int questionIdx) { return _questions[questionIdx].options.size(); }
        QStringList getOptionsTexts(int questionIdx); //insure sequence


        //Get options
        //Get options rates - insure sequence the same

        //Get mixed options - as QVariantList


    private:

        void addQuestions(const QJsonArray& questions);
        void addOptions(TestQuestion& question, const QJsonArray& options);
        void addRates(const QJsonArray& rates);

        bool hasRootErros(const QJsonDocument& root);

        QString readTextFile(const QString& filename);

        QString _testName;
        QString _testDescription;

        std::vector<TestQuestion> _questions;
        std::vector<TestRate> _rates; //TODO subrates? review on Beck test

        //_subRates -> Question list, way to summate
    };


}


#endif // TESTSENGINE_HPP
