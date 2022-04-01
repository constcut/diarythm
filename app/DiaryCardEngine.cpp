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

    clear();
    auto rootObject = doc.object();

    _cardName = rootObject["name"].toString();

    if (rootObject.contains("description"))
        _cardDescription = rootObject["descrition"].toString();

    loadFromJsonObject(rootObject);

    qDebug() << "Diary card json loaded";
}



void DiaryCardEngine::mergeJSON(const QString& json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (hasRootErros(doc))
        return;

    loadFromJsonObject(doc.object());
    //Группы и поля не должны пересекаться, на это пока нет проверки
}


void DiaryCardEngine::loadFromJsonObject(const QJsonObject& rootObject)
{
    if (rootObject.contains("enums") && rootObject["enums"].isArray())
        addEnums(rootObject["enums"].toArray());

    addGroups(rootObject["groups"].toArray());
}


void DiaryCardEngine::clear()
{
    _cardName = "";
    _cardDescription = "";

    _enums.clear();
    _groups.clear();
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


void DiaryCardEngine::fillFieldProperties(CardField& cardField, const QJsonObject& fieldObject) const
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


void DiaryCardEngine::fillGroupProperties(CardGroup& cardGroup, const QJsonObject& groupObject) const
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



bool DiaryCardEngine::hasRootErros(const QJsonDocument& doc) const
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

    if (rootObject.contains("name") == false) {
        qDebug() << "Failed to parse: JSON name field not found.";
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



QStringList DiaryCardEngine::getAllEnumsNames() const
{
    QStringList allNames;

    for (const auto& [enumName, _]: _enums)
        allNames.append(enumName);

    return allNames;
}


QStringList DiaryCardEngine::getEnumNames(const QString &name) const
{
    if (_enums.count(name) == 0)
        return {};

    return _enums.at(name).valuesNames;
}


QStringList DiaryCardEngine::getEnumDisplayNames(const QString &name) const
{
    if (_enums.count(name) == 0)
        return {};

    return _enums.at(name).displayNames;
}


QList<int> DiaryCardEngine::getEnumValues(const QString &name) const
{
    if (_enums.count(name) == 0)
        return {};

    return _enums.at(name).values;
}


QString DiaryCardEngine::getEnumDescription(const QString &name) const
{
    if (_enums.count(name) == 0)
        return {};

    return _enums.at(name).description;
}


QStringList DiaryCardEngine::getAllGroupsNames() const //Refact generalize
{
    QStringList allNames;

    for (const auto& [groupName, _]: _groups)
        allNames.append(groupName);

    return allNames;
}


QString DiaryCardEngine::getGroupDescription(const QString &name) const
{
    if (_groups.count(name) == 0)
        return {};

    return _groups.at(name).description;
}


bool DiaryCardEngine::isGroupMandatory(const QString &name) const
{
    if (_groups.count(name) == 0)
        return false;

    return _groups.at(name).mandatory;
}


int DiaryCardEngine::getGroupDaysFrequency(const QString &name) const
{
    if (_groups.count(name) == 0)
        return 0;

    return _groups.at(name).daysFrequency;
}


QList<int> DiaryCardEngine::getGroupWeekDays(const QString &name) const
{
    if (_groups.count(name) == 0)
        return {};

    return _groups.at(name).onWeekDays;
}


QList<int> DiaryCardEngine::getGroupMonthDays(const QString &name) const
{
    if (_groups.count(name) == 0)
        return {};

    return _groups.at(name).onMonthDays;
}


bool DiaryCardEngine::isItGroupDay(const QString &name) const
{
    QDate date = QDate::currentDate();
    return isItGroupDay(date, name);
}


bool DiaryCardEngine::isItGroupDay(const QString &date, const QString &name) const
{
    QDate dateObject = QDate::fromString(date); //возможно тут нужна маска
    return isItGroupDay(dateObject, name);
}


bool DiaryCardEngine::isItGroupDay(const QDate& date, const QString &name) const
{
    if (_groups.count(name) == 0)
        return false;

     const auto& group = _groups.at(name);

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


QStringList DiaryCardEngine::getAllGroupFields(const QString &name) const
{
    if (_groups.count(name) == 0)
        return {};

    QStringList allFields;
    for (const auto& [fieldName, _]: _groups.at(name).fields)
        allFields.append(fieldName);

    return allFields;
}


QString DiaryCardEngine::getFieldType(const QString &group, const QString &field) const
{
    if (isFieldMissing(group, field))
        return "";

    return _groups.at(group).fields.at(field).type;
}


QString DiaryCardEngine::getFieldDescription(const QString &group, const QString &field) const
{
    if (isFieldMissing(group, field))
        return "";

    return _groups.at(group).fields.at(field).description;
}


QString DiaryCardEngine::getFieldEnum(const QString &group, const QString &field) const
{
    if (isFieldMissing(group, field))
        return "";

    return _groups.at(group).fields.at(field).enumName;
}

int DiaryCardEngine::getFieldRangeMin(const QString &group, const QString &field) const
{
    if (isFieldMissing(group, field))
        return -1;

    return _groups.at(group).fields.at(field).rangeMin;
}


int DiaryCardEngine::getFieldRangeMax(const QString &group, const QString &field) const
{
    if (isFieldMissing(group, field))
        return -1;

    return _groups.at(group).fields.at(field).rangeMax;
}


bool DiaryCardEngine::isFieldMissing(const QString &group, const QString &field) const
{
    if (_groups.count(group) == 0)
        return true;

    const auto& groupObj = _groups.at(group);
    if (groupObj.fields.count(field) == false)
        return true;

    return false;
}



void DiaryCardEngine::addNewEnum(QString name)
{
    if (_enums.count(name)) {
        qDebug() << "Warning: attemp to create enum with existing name";
        return;
    }

    CardEnum cardEnum;
    cardEnum.name = name;
    _enums[name] = cardEnum;
}


void DiaryCardEngine::addNewGroup(QString name)
{
    if (_groups.count(name)) {
        qDebug() << "Warning: attemp to create enum with existing name";
        return;
    }

    CardGroup cardGroup;
    cardGroup.name = name;
    _groups[name] = cardGroup;
}


void DiaryCardEngine::changeEnumDescription(QString enumName, QString description)
{
    if (_enums.count(enumName) == 0) {
        qDebug() << "Failed to find enum to change description: " << enumName;
        return;
    }

    _enums[enumName].description = description;
}


void DiaryCardEngine::changeGroupDescription(QString groupName, QString description)
{
    if (_groups.count(groupName) == 0) {
        qDebug() << "Failed to find group to change description: " << groupName;
        return;
    }

    _groups[groupName].description = description;
}


void DiaryCardEngine::changeGroupMandatory(QString groupName, bool value)
{
    if (_groups.count(groupName) == 0) {
        qDebug() << "Failed to find group to change mandatory status" << groupName;
        return;
    }

    _groups[groupName].mandatory = value;
}
