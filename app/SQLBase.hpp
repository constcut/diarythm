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

        Q_INVOKABLE void addAudioRecord(const QString& date, const QString& time, int localId,
                                        const QString& name, quint64 durationMs);

        Q_INVOKABLE void editAudioRecord(const QString& date, int localId, const QString& name,
                                         const QString& tags, const QString& description);

        Q_INVOKABLE void removeAudioRecord(const QString& date, int localId);

        Q_INVOKABLE int getRecordsMaxLocalId(const QString& date);
        Q_INVOKABLE int getTotalRecords();

        Q_INVOKABLE QStringList findSingleRecord(const QString& date, int localId);
        Q_INVOKABLE QVariantList findRecords(const QString& date);

        Q_INVOKABLE QVariantList findRecordsByNameMask(const QString& nameMask);
        Q_INVOKABLE QVariantList findRecordsByTagMask(const QString& tagMask);

        Q_INVOKABLE QVariantList findRecordsByNameMaskAndDate(const QString& date,
                                                              const QString& nameMask);

        Q_INVOKABLE QVariantList findRecordsByTagMaskAndDate(const QString& date,
                                                             const QString& tagMask);


        Q_INVOKABLE void addText(const QString& name, const QString& text,
                                 const QString& tags, const QString& description);

        Q_INVOKABLE void editText(const QString& date, int localId, const QString& name,
                                  const QString& tags, const QString& description);

        Q_INVOKABLE void removeText(const QString& date, int localId);

        Q_INVOKABLE int getTextsMaxLocalId(const QString& date);
        Q_INVOKABLE int getTotalTexts();

        Q_INVOKABLE QStringList findSingleText(const QString& date, int localId);
        Q_INVOKABLE QVariantList findTexts(const QString& date);

        Q_INVOKABLE QVariantList findTextsByNameMask(const QString& nameMask);
        Q_INVOKABLE QVariantList findTextsByTagMask(const QString& tagMask);

        Q_INVOKABLE QVariantList findTextsByNameMaskAndDate(const QString& date,
                                                            const QString& nameMask);

        Q_INVOKABLE QVariantList findTextsByTagMaskAndDate(const QString& date,
                                                           const QString& tagMask);


    private:

        QSqlError initBase();
        void createTablesIfNeeded();

        QVariantList fillRecordsSearchResults(QSqlQuery& query);

        bool logIfError(QSqlQuery& query, const QString& request);

        QVariantList findByFieldMaskAndDate(const QString& table, const QString& field,
                                            const QString& date, const QString& mask);

        QVariantList findByFieldMask(const QString& table, const QString& field,
                                     const QString& mask);

        QVariantList findByDate(const QString& table, const QString& date);

        QStringList findSingle(const QString& table, int fieldsCount,
                               const QString& date, int localId);

        int getTotalRows(const QString& keyField, const QString& table);

        int getMaxLocalId(const QString& table, const QString& date);

        void removeRow(const QString& table, const QString& date, int localId);

        void editRow(const QString& table, const QString& nameField, const QString& date,
                     int localId, const QString& name, const QString& tags, const QString& description);


        const int audioFieldsCount = 7; //Later use walkaround with query.last() and query.at() + 1
        const int textFieldsCount = 7;
    };

}

#endif // SQLBASE_HPP
