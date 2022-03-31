#include "DiaryCardEngine.hpp"

#include <QJsonDocument>
#include <QDebug>
#include <QDate>


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

        const auto& fieldsArray = groupObject["fields"].toArray();
        for (const auto& field: fieldsArray)
        {
            const auto& fieldObj = field.toObject();
            CardField cardField;
            fillFieldProperties(cardField, fieldObj);

            cardGroup.fields[cardField.name] = cardField;
        }

        _groups[cardGroup.name] = cardGroup;
    }
}


void DiaryCardEngine::fillFieldProperties(CardField& cardField, const QJsonObject& fieldObject)
{
    cardField.name = fieldObject["name"].toString();
    cardField.type = fieldObject["type"].toString();

    if (fieldObject.contains("description"))
        cardField.description = fieldObject["description"].toString();

    if (cardField.type == "enum")
        cardField.enumName = fieldObject["enumName"].toString();

    if (cardField.type == "range")
    {
        if (fieldObject.contains("rangeMin"))
            cardField.rangeMin = fieldObject["rangeMin"].toInt();

        if (fieldObject.contains("rangeMax"))
            cardField.rangeMax = fieldObject["rangeMax"].toInt();
    }
}


void DiaryCardEngine::fillGroupProperties(CardGroup& cardGroup, const QJsonObject& groupObject)
{
    cardGroup.name = groupObject["name"].toString();
    if (groupObject.contains("description"))
        cardGroup.description = groupObject["description"].toString();

    if (groupObject.contains("mandatory"))
        cardGroup.mandatory = groupObject["mandatory"].toBool();

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
            cardEnum.description = enumObj["description"].toString();

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
    if (_enums.count(name) == 0)
        return {};

    return _enums[name].valuesNames;
}


QStringList DiaryCardEngine::getEnumDisplayNames(QString name)
{
    if (_enums.count(name) == 0)
        return {};

    return _enums[name].displayNames;
}


QList<int> DiaryCardEngine::getEnumValues(QString name)
{
    if (_enums.count(name) == 0)
        return {};

    return _enums[name].values;
}


QString DiaryCardEngine::getEnumDescription(QString name)
{
    if (_enums.count(name) == 0)
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
    if (_groups.count(name) == 0)
        return {};

    return _groups[name].description;
}


bool DiaryCardEngine::isGroupMandatory(QString name)
{
    if (_groups.count(name) == 0)
        return false;

    return _groups[name].mandatory;
}


int DiaryCardEngine::getGroupDaysFrequency(QString name)
{
    if (_groups.count(name) == 0)
        return 0;

    return _groups[name].daysFrequency;
}


QList<int> DiaryCardEngine::getGroupWeekDays(QString name)
{
    if (_groups.count(name) == 0)
        return {};

    return _groups[name].onWeekDays;
}


QList<int> DiaryCardEngine::getGroupMonthDays(QString name)
{
    if (_groups.count(name) == 0)
        return {};

    return _groups[name].onMonthDays;
}


bool DiaryCardEngine::isItGroupDay(QString name)
{
    QDate date = QDate::currentDate();
    return isItGroupDay(date, name);
}


bool DiaryCardEngine::isItGroupDay(QString date, QString name)
{
    QDate dateObject = QDate::fromString(date); //возможно тут нужна маска
    return isItGroupDay(dateObject, name);
}


bool DiaryCardEngine::isItGroupDay(const QDate& date, QString name)
{
    if (_groups.count(name) == 0)
        return false;

     const auto& group = _groups[name];

     if (group.daysFrequency == 1) //Подумать над другими значениями
         return true;

    if (group.onMonthDays.empty() == false)
        for (int day: group.onMonthDays)
            if (day == date.day())
                return true;

    if (group.onWeekDays.empty() == false)
        for (int day: group.onWeekDays)
            if (day == date.dayOfWeek())
                return true;

    return false;
}


QStringList DiaryCardEngine::getAllGroupFields(QString name)
{
    if (_groups.count(name) == 0)
        return {};

    QStringList allFields;
    for (const auto& [fieldName, _]: _groups[name].fields)
        allFields.append(fieldName);

    return allFields;
}


QString DiaryCardEngine::getFieldType(QString group, QString field)
{
    if (_groups.count(group) == 0)
        return "";

    const auto& groupObj = _groups[group];
    if (groupObj.fields.count(field) == false)
        return "";

    return groupObj.fields.at(field).type;
}

//TODO generalize
QString DiaryCardEngine::getFieldDescription(QString group, QString field)
{
    if (_groups.count(group) == 0)
        return "";

    const auto& groupObj = _groups[group];
    if (groupObj.fields.count(field) == false)
        return "";

    return groupObj.fields.at(field).description;
}


QString DiaryCardEngine::getFieldEnum(QString group, QString field)
{
    if (_groups.count(group) == 0)
        return "";

    const auto& groupObj = _groups[group];
    if (groupObj.fields.count(field) == false)
        return "";

    return groupObj.fields.at(field).enumName;
}
