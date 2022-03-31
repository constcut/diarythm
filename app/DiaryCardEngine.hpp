#ifndef DIARYCARDENGINE_HPP
#define DIARYCARDENGINE_HPP

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <unordered_map>


namespace diaryth {


    struct CardEnum {
        QString name;
        QString description;

        QStringList valuesNames; //Возможно лишнее
        QStringList displayNames;
        QList<int> values;
    };



    class DiaryCardEngine : public QObject
    {
        Q_OBJECT
    public:

        DiaryCardEngine() = default;

        void parseJSON(const QString& json);

        QStringList getAllEnumsNames();

        QStringList getEnumNames(QString name);
        QStringList getEnumDisplayNames(QString name);

        QList<int> getEnumValues(QString name);

    private:

        void addEnums(const QJsonArray& enumsArray);

        bool hasRootErros(const QJsonDocument& root);

        std::unordered_map<QString, CardEnum> _enums;

    };

}


#endif // DIARYCARDENGINE_HPP
