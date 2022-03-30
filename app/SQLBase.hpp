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

        Q_INVOKABLE void addAudioRecord(QString date, QString time, int localId,
                            QString name, quint64 durationMs);

        Q_INVOKABLE void editAudioRecord(QString date, int localId, QString name,
                             QString tags, QString description);

        Q_INVOKABLE void removeAudioRecord(QString date, int localId);

        Q_INVOKABLE int getRecordsMaxLocalId(QString date);
        Q_INVOKABLE int getTotalRecords();

        Q_INVOKABLE QStringList findSingleRecord(QString date, int localId);
        Q_INVOKABLE QVariantList findRecords(QString date);

        Q_INVOKABLE QVariantList findRecordsByNameMask(QString nameMask);
        Q_INVOKABLE QVariantList findRecordsByTagMask(QString tagMask);

        Q_INVOKABLE QVariantList findRecordsByNameMaskAndDate(QString date, QString nameMask);
        Q_INVOKABLE QVariantList findRecordsByTagMaskAndDate(QString date, QString tagMask);


        Q_INVOKABLE void addText(QString name, QString text, QString tags, QString description);
        Q_INVOKABLE void editText(QString date, int localId, QString name, QString tags, QString description);

        Q_INVOKABLE void removeText(QString date, int localId);

        Q_INVOKABLE int getTextsMaxLocalId(QString date);
        Q_INVOKABLE int getTotalTexts();

        Q_INVOKABLE QStringList findSingleText(QString date, int localId);
        Q_INVOKABLE QVariantList findTexts(QString date);

        Q_INVOKABLE QVariantList findTextsByNameMask(QString nameMask);
        Q_INVOKABLE QVariantList findTextsByTagMask(QString tagMask);

        Q_INVOKABLE QVariantList findTextsByNameMaskAndDate(QString date, QString nameMask);
        Q_INVOKABLE QVariantList findTextsByTagMaskAndDate(QString date, QString tagMask);


    private:

        QSqlError initBase();
        void createTablesIfNeeded();

        QVariantList fillRecordsSearchResults(QSqlQuery& query);

        bool logIfError(QSqlQuery& query, const QString& request);

        QVariantList findByFieldMaskAndDate(QString table, QString field, QString date, QString mask);

        const int audioFieldsCount = 7; //Later use walkaround with query.last() and query.at() + 1
        const int textFieldsCount = 7;
    };

}

#endif // SQLBASE_HPP
