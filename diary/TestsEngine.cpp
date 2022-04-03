#include "TestsEngine.hpp"

#include <QFile>


using namespace diaryth;


void TestsEngine::parseJSON(const QString& json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());

    if (hasRootErros(doc)) {
        qDebug() << "Tests engine root json has errors";
        return;
    }

    auto docObject = doc.object();

    _testName = docObject["name"].toString();

    if (docObject.contains("description"))
        _testDescription = docObject["description"].toString();
    else
        _testDescription = "";

    _questions.clear();
    addQuestions(docObject["questions"].toArray());

    _rates.clear();
    addRates(docObject["rates"].toArray());

    qDebug() << "Test engine loaded: " << _testName;
}


void TestsEngine::addRates(const QJsonArray& rates)
{
    for (const auto& r: rates)
    {
        const auto rateObject = r.toObject();

        TestRate testRate;
        testRate.text = rateObject["text"].toString();
        testRate.rangeBegin = rateObject["rangeBegin"].toDouble();
        testRate.rangeEnd = rateObject["rangeEnd"].toDouble();

        if (rateObject.contains("description"))
            testRate.description = rateObject["description"].toString();

    }
}


void TestsEngine::addQuestions(const QJsonArray& questions)
{
    for (const auto& q: questions)
    {
        const auto questionObject = q.toObject();

        TestQuestion testQuestion;
        testQuestion.text = questionObject["text"].toString();
        testQuestion.type = questionObject["type"].toString();

        if (questionObject.contains("groupId"))
            testQuestion.groupId = questionObject["groupId"].toInt();

        addOptions(testQuestion, questionObject["options"].toArray());
        _questions.push_back(testQuestion);
    }
}


void TestsEngine::addOptions(TestQuestion& question, const QJsonArray& options)
{
    for (const auto& o: options)
    {
        const auto optionObject = o.toObject();

        TestOption testOption;
        testOption.text = optionObject["text"].toString();
        testOption.rate = optionObject["rate"].toDouble();

        if (optionObject.contains("description"))
            testOption.description = optionObject["description"].toString();

        question.options.push_back(testOption);
    }
}


bool TestsEngine::hasRootErros(const QJsonDocument& root)
{
    const auto docObject = root.object();

    if (docObject.contains("name") == false) {
        qDebug() << "Error: Tests engine, failed to find name";
        return true;
    }

    if (docObject.contains("questions") == false) {
        qDebug() << "Error: Tests engine, failed to find questions";
        return true;
    }

    if (docObject["questions"].isArray() == false) {
        qDebug() << "Error: Tests engine, questions is not array";
        return true;
    }

    if (docObject.contains("rates") == false) {
        qDebug() << "Error: Tests engine, failed to find rates";
        return true;
    }

    if (docObject["rates"].isArray() == false) {
        qDebug() << "Error: Tests engine, rates is not array";
        return true;
    }

    return false;
}


void TestsEngine::parseFromFile(const QString& filename)
{
    parseJSON(readTextFile(filename));
}


QString TestsEngine::readTextFile(const QString& filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&file);
    return in.readAll();
}

