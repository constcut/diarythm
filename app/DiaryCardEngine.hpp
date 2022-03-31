#ifndef DIARYCARDENGINE_HPP
#define DIARYCARDENGINE_HPP

#include <QObject>



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

};




#endif // DIARYCARDENGINE_HPP
