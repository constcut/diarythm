#ifndef TESTSENGINE_HPP
#define TESTSENGINE_HPP

#include <QObject>



namespace diaryth {



    class TestsEngine : public QObject
    {
        Q_OBJECT

    public:
        TestsEngine() = default;

        void parseJSON(const QString& json);

    private:


        QString _testName;
        QString _testDescription;

    };


}


#endif // TESTSENGINE_HPP
