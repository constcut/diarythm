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
        SQLBase();

        QSqlQuery executeRequest(const QString& requestBody);

        void addAudioRecord(QString date, QString time, int localId,
                            QString name, quint64 durationMs);

        void editAudioRecord(QString date, int localId,
                             QString tags, QString description);

    private:

        QSqlError initBase();
        void createTablesIfNeeded();

    };

}

#endif // SQLBASE_HPP
