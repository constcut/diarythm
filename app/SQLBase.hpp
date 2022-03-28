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

        //TODO const& for QString

        void addAudioRecord(QString date, QString time, int localId,
                            QString name, quint64 durationMs);

        void editAudioRecord(QString date, int localId,
                             QString tags, QString description);

        void removeAudioRecord(QString date, int localId);

        int recordsMaxLocalId(QString date);
        int getTotalRecords();

        QStringList findSingleRecord(QString date, int localId);
        QVariantList findRecords(QString date);

    private:

        QSqlError initBase();
        void createTablesIfNeeded();

    };

}

#endif // SQLBASE_HPP
