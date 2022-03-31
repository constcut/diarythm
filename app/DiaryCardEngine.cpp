#include "DiaryCardEngine.hpp"

#include <QJsonDocument>
#include <QDebug>


using namespace diaryth;


void DiaryCardEngine::parseJSON(const QString& json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());

    if (hasRootErros(doc))
        return;

    auto rootObject = doc.object();

    if (rootObject.contains("enums") && rootObject["enums"].isArray())
        addEnums(rootObject["enums"].toArray());

    addGroups(rootObject["groups"].toArray());

    qDebug() << "Diary card json loaded";
}


void DiaryCardEngine::addGroups(const QJsonArray& groupsArray)
{
    for (const auto& group: groupsArray)
    {
        const auto& groupObject = group.toObject();
        CardGroup cardGroup;
        fillGroupProperties(cardGroup, groupObject);

        //TODO fields

        _groups[cardGroup.name] = cardGroup;
    }

}


void DiaryCardEngine::fillGroupProperties(CardGroup& cardGroup, const QJsonObject& groupObject)
{
    cardGroup.name = groupObject["name"].toString();
    if (groupObject.contains("description"))
        cardGroup.description = groupObject["description"].toString();

    if (groupObject.contains("mandatory"))
        cardGroup.mandatory = groupObject["mandatory"].toBool();
    else
        cardGroup.mandatory = true;

    if (groupObject.contains("onWeekDays"))
    {
        const auto& weekDaysArray = groupObject["onWeekDays"].toArray();
        for (const auto& weekDay: weekDaysArray)
            cardGroup.onWeekDays.append(weekDay.toInt());
    }

    if (groupObject.contains("onMonthDays"))
    {
        const auto& monthDaysArray = groupObject["onMonthDays"].toArray();
        for (const auto& monthDay: monthDaysArray)
            cardGroup.onMonthDays.append(monthDay.toInt());
    }

    if (cardGroup.onMonthDays.empty() && cardGroup.onWeekDays.empty())
    {
        if (groupObject.contains("daysFrequency"))
            cardGroup.daysFrequency = groupObject["daysFrequency"].toInt();
        else
            cardGroup.daysFrequency = 1;
    }
}



bool DiaryCardEngine::hasRootErros(const QJsonDocument& doc)
{
    if (doc.isObject() == false) {
        qDebug() << "Failed to parse: JSON root is not an object.";
        return true;
    }

    auto rootObject = doc.object();

    if (rootObject.contains("groups") == false) {
        qDebug() << "Failed to prase: JSON root doesn't contain groups field.";
        return true;
    }

    if (rootObject["groups"].isArray() == false) {
        qDebug() << "Failed to parse: JSON groups field isn't array";
        return true;
    }

    return false;
}


void DiaryCardEngine::addEnums(const QJsonArray &enumsArray)
{
    for (const auto& singleEnum: enumsArray)
    {
        const auto& enumObj = singleEnum.toObject();
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


QString DiaryCardEngine::getEnumDescription(QString name)
{
    if (_enums.count(name) == false)
        return {};

    return _enums[name].description;
}


QStringList DiaryCardEngine::getAllGroupsNames() //Refact generalize
{
    QStringList allNames;

    for (const auto& [groupName, _]: _groups)
        allNames.append(groupName);

    return allNames;
}


QString DiaryCardEngine::getGroupDescription(QString name)
{
    if (_groups.count(name) == false)
        return {};

    return _groups[name].description;
}


bool DiaryCardEngine::isGroupMandatory(QString name)
{
    if (_groups.count(name) == false)
        return false;

    return _groups[name].mandatory;
}


int DiaryCardEngine::getGroupDaysFrequency(QString name)
{
    if (_groups.count(name) == false)
        return 0;

    return _groups[name].daysFrequency;
}


QList<int> DiaryCardEngine::getGroupWeekDays(QString name)
{
    if (_groups.count(name) == false)
        return {};

    return _groups[name].onWeekDays;
}


QList<int> DiaryCardEngine::getGroupMonthDays(QString name)
{
    if (_groups.count(name) == false)
        return {};

    return _groups[name].onMonthDays;
}

