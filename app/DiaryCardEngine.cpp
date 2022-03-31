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

    auto groups = rootObject["groups"];
}
