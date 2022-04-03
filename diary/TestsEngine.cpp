#include "TestsEngine.hpp"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>



using namespace diaryth;


void TestsEngine::parseJSON(const QString& json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());

    //checks

    auto docObject = doc.object();

    _testName = docObject["name"].toString();

    if (docObject.contains("description"))
        _testDescription = docObject["description"].toString();
    else
        _testDescription = "";


}
