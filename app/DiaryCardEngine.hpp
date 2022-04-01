#ifndef DIARYCARDENGINE_HPP
#define DIARYCARDENGINE_HPP

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <unordered_map>


namespace diaryth {


    struct CardEnum
    {
        QString name;
        QString description;

        QStringList valuesNames;
        QStringList displayNames;
        QList<int> values;
    };


    struct CardField
    {
        QString name;
        QString type;
        QString description;

        QString enumName;
        int rangeMin = 0;
        int rangeMax = 10;
    };


    struct CardGroup
    {
        QString name;
        QString description;
        bool mandatory = true;

        int daysFrequency = 1;
        QList<int> onWeekDays;
        QList<int> onMonthDays;

        std::unordered_map<QString, CardField> fields;
    };



    class DiaryCardEngine : public QObject
    {
        Q_OBJECT
    public:

        DiaryCardEngine() = default;

        Q_INVOKABLE void parseJSON(const QString& json);

        Q_INVOKABLE QString getCardName() const { return _cardName; }
        Q_INVOKABLE QString getCardDescription() const { return _cardDescription; }

        Q_INVOKABLE QStringList getAllEnumsNames() const;
        Q_INVOKABLE QString getEnumDescription(const QString& name) const;

        Q_INVOKABLE QStringList getEnumNames(const QString& name) const;
        Q_INVOKABLE QStringList getEnumDisplayNames(const QString& name) const;
        Q_INVOKABLE QList<int> getEnumValues(const QString& name) const;

        Q_INVOKABLE QStringList getAllGroupsNames() const;
        Q_INVOKABLE QString getGroupDescription(const QString& name) const;
        Q_INVOKABLE bool isGroupMandatory(const QString& name) const;

        Q_INVOKABLE int getGroupDaysFrequency(const QString& name) const;
        Q_INVOKABLE QList<int> getGroupWeekDays(const QString& name) const;
        Q_INVOKABLE QList<int> getGroupMonthDays(const QString& name) const;

        Q_INVOKABLE bool isItGroupDay(const QString& name) const;
        Q_INVOKABLE bool isItGroupDay(const QString& date, const QString& name) const;

        Q_INVOKABLE QStringList getAllGroupFields(const QString& name) const;
        Q_INVOKABLE QString getFieldType(const QString& group, const QString& field) const;
        Q_INVOKABLE QString getFieldDescription(const QString& group, const QString& field) const;
        Q_INVOKABLE QString getFieldEnum(const QString& group, const QString& field) const;

        Q_INVOKABLE int getFieldRangeMin(const QString& group, const QString& field) const;
        Q_INVOKABLE int getFieldRangeMax(const QString& group, const QString& field) const;


    private:

        bool isItGroupDay(const QDate& date, const QString& name) const;

        void addEnums(const QJsonArray& enumsArray);
        void addGroups(const QJsonArray& groupsArray);

        void fillGroupProperties(CardGroup& cardGroup,
                                 const QJsonObject& groupObject) const;

        void fillFieldProperties(CardField& cardField,
                                 const QJsonObject& fieldObject) const;

        bool hasRootErros(const QJsonDocument& doc) const;

        bool isFieldMissing(const QString& group, const QString& field) const;


        std::unordered_map<QString, CardEnum> _enums;
        std::unordered_map<QString, CardGroup> _groups;

        QString _cardName;
        QString _cardDescription;

    };

}


#endif // DIARYCARDENGINE_HPP
