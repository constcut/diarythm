#ifndef SQLBASE_HPP
#define SQLBASE_HPP

#include <QObject>
#include <QVariantList>
#include <QSqlQuery>

namespace diaryth {


    class SQLBase : public QObject
    {
        Q_OBJECT

    public:
        SQLBase() = default;

        QSqlQuery executeRequest(const QString& requestBody);

        void createTablesIfNeeded();

    private:

        QSqlError initBase();

    };

}

#endif // SQLBASE_HPP
