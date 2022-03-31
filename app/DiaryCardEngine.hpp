#ifndef DIARYCARDENGINE_HPP
#define DIARYCARDENGINE_HPP

#include <QObject>

class DiaryCardEngine : public QObject
{
    Q_OBJECT
public:

    DiaryCardEngine() = default;

    void parseJSON(const QString& json);

};

#endif // DIARYCARDENGINE_HPP
