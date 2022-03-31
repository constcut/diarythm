#include "DiaryCardEngine.hpp"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

void DiaryCardEngine::parseJSON(const QString& json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());

    if (doc.isObject() == false) {
        qDebug() << "Failed to parse: JSON root is not an object.";
        return;
    }

    auto rootObject = doc.object();

    if (rootObject.contains("groups") == false) {
        qDebug() << "Failed to prase: JSON root doesn't contain groups field.";
        return;
    }

    if (rootObject["groups"].isArray() == false) {
        qDebug() << "Failed to parse: JSON groups field isn't array";
        return;
    }

    if (rootObject.contains("enums") && rootObject["enums"].isArray())
    {
        auto enumsArray = rootObject["enums"].toArray();

        for (const auto& singleEnum: enumsArray)
        {
            auto enumObj = singleEnum.toObject();

            auto enumName = enumObj["name"].toString();
            auto description = enumObj["description"].toString();
            bool showValues = enumObj["showValues"].toBool();

            auto valuesArray = enumObj["values"].toArray();
            auto vNamesArray = enumObj["names"].toArray();
        }
    }

    auto groups = rootObject["groups"];
}
