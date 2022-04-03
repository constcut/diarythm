#include "TestsEngine.hpp"


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
