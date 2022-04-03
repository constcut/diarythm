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


    addQuestions(docObject["questions"].toArray());

    qDebug() << "Test engine loaded: " << _testName;
}


void TestsEngine::addQuestions(const QJsonArray& questions)
{
    for (const auto& q: questions)
    {
        const auto questionObject = q.toObject();


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

