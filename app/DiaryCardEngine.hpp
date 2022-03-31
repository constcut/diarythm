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


    struct CardGroup
    {
        QString name;
        QString description;
        bool mandatory;

        int daysFrequency;
        QList<int> onWeekDays;
        QList<int> onMonthDays;

    };



    class DiaryCardEngine : public QObject
    {
        Q_OBJECT
    public:

        DiaryCardEngine() = default;

        void parseJSON(const QString& json);

        QStringList getAllEnumsNames();
        QString getEnumDescription(QString name);

        QStringList getEnumNames(QString name);
        QStringList getEnumDisplayNames(QString name);
        QList<int> getEnumValues(QString name);

        QStringList getAllGroupsNames();
        QString getGroupDescription(QString name);
        bool isGroupMandatory(QString name);


    private:

        void addEnums(const QJsonArray& enumsArray);
        void addGroups(const QJsonArray& groupsArray);

        bool hasRootErros(const QJsonDocument& doc);

        std::unordered_map<QString, CardEnum> _enums;
        std::unordered_map<QString, CardGroup> _groups;

    };

}


#endif // DIARYCARDENGINE_HPP
