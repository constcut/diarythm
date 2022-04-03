#ifndef TESTSENGINE_HPP
#define TESTSENGINE_HPP

#include <QObject>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


namespace diaryth {



    class TestsEngine : public QObject
    {
        Q_OBJECT

    public:
        TestsEngine() = default;

        void parseJSON(const QString& json);
        void parseFromFile(const QString& filename);

    private:


        bool hasRootErros(const QJsonDocument& root);

        QString readTextFile(const QString& filename);



        QString _testName;
        QString _testDescription;

    };


}


#endif // TESTSENGINE_HPP
