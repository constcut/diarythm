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

        void parseJSON(const QString& json);

        QStringList getAllEnumsNames() const;
        QString getEnumDescription(QString name) const;

        QStringList getEnumNames(QString name) const;
        QStringList getEnumDisplayNames(QString name) const;
        QList<int> getEnumValues(QString name) const;

        QStringList getAllGroupsNames();
        QString getGroupDescription(QString name);
        bool isGroupMandatory(QString name);

        int getGroupDaysFrequency(QString name);
        QList<int> getGroupWeekDays(QString name);
        QList<int> getGroupMonthDays(QString name);

        bool isItGroupDay(QString name);
        bool isItGroupDay(QString date, QString name);

        QStringList getAllGroupFields(QString name);
        QString getFieldType(QString group, QString field);
        QString getFieldDescription(QString group, QString field);
        QString getFieldEnum(QString group, QString field);

        int getFieldRangeMin(QString group, QString field);
        int getFieldRangeMax(QString group, QString field);

    private:

        bool isItGroupDay(const QDate& date, QString name) const;

        void addEnums(const QJsonArray& enumsArray);
        void addGroups(const QJsonArray& groupsArray);

        void fillGroupProperties(CardGroup& cardGroup,
                                 const QJsonObject& groupObject) const;

        void fillFieldProperties(CardField& cardField,
                                 const QJsonObject& fieldObject) const;

        bool hasRootErros(const QJsonDocument& doc) const;

        bool isFieldMissing(QString group, QString field) const;


        std::unordered_map<QString, CardEnum> _enums;
        std::unordered_map<QString, CardGroup> _groups;

    };

}


#endif // DIARYCARDENGINE_HPP
