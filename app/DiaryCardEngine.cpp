#include "DiaryCardEngine.hpp"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>


using namespace diaryth;


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
            auto valuesArray = enumObj["values"].toArray();
            auto vNamesArray = enumObj["names"].toArray();

            if (valuesArray.size() != vNamesArray.size())
            {
                qDebug() << "Parse error: values and names arrays has different sizes "
                         << valuesArray.size() << " and " << vNamesArray.size();
                continue;
            }

            CardEnum cardEnum;
            cardEnum.name = enumObj["name"].toString();

            if (enumObj.contains("description"))
                cardEnum.description = enumObj["description"].toString(); //optional

            bool showValues = enumObj["showValues"].toBool();

            for (int i = 0; i < valuesArray.size(); ++i)
            {
                const auto value = valuesArray[i].toInt();
                const auto valueName = vNamesArray[i].toString();

                cardEnum.valuesNames.append(valueName);
                cardEnum.values.append(value);

                if (showValues) {
                    const QString fullName = QString::number(value) + " " + vNamesArray[i].toString();
                    cardEnum.displayNames.append(fullName);
                }
                else
                    cardEnum.displayNames.append(valueName);
            }

            _enums[cardEnum.name] = cardEnum;

        }
    }

    auto groups = rootObject["groups"];
}


QStringList DiaryCardEngine::getAllEnumsNames()
{
    QStringList allNames;

    for (const auto& [enumName, _]: _enums)
        allNames.append(enumName);

    return allNames;
}


QStringList DiaryCardEngine::getEnumNames(QString name)
{
    if (_enums.count(name) == false)
        return {};

    return _enums[name].valuesNames;
}


QStringList DiaryCardEngine::getEnumDisplayNames(QString name)
{
    if (_enums.count(name) == false)
        return {};

    return _enums[name].displayNames;
}


QList<int> DiaryCardEngine::getEnumValues(QString name)
{
    if (_enums.count(name) == false)
        return {};

    return _enums[name].values;
}






