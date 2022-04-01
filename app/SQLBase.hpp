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

        QSqlQuery executeRequest(const QString& requestBody) const;

        Q_INVOKABLE void addAudioRecord(const QString& date, const QString& time, int localId,
                                        const QString& name, quint64 durationMs) const;

        Q_INVOKABLE void editAudioRecord(const QString& date, int localId, const QString& name,
                                         const QString& tags, const QString& description) const;

        Q_INVOKABLE void removeAudioRecord(const QString& date, int localId) const;

        Q_INVOKABLE int getRecordsMaxLocalId(const QString& date) const;
        Q_INVOKABLE int getTotalRecords() const;

        Q_INVOKABLE QStringList findSingleRecord(const QString& date, int localId) const;
        Q_INVOKABLE QVariantList findRecords(const QString& date) const;

        Q_INVOKABLE QVariantList findRecordsByNameMask(const QString& nameMask) const;
        Q_INVOKABLE QVariantList findRecordsByTagMask(const QString& tagMask) const;

        Q_INVOKABLE QVariantList findRecordsByNameMaskAndDate(const QString& date,
                                                              const QString& nameMask) const;

        Q_INVOKABLE QVariantList findRecordsByTagMaskAndDate(const QString& date,
                                                             const QString& tagMask) const;


        Q_INVOKABLE void addText(const QString& name, const QString& text,
                                 const QString& tags, const QString& description) const;

        Q_INVOKABLE void editText(const QString& date, int localId, const QString& name,
                                  const QString& tags, const QString& description) const;

        Q_INVOKABLE void removeText(const QString& date, int localId) const;

        Q_INVOKABLE int getTextsMaxLocalId(const QString& date) const;
        Q_INVOKABLE int getTotalTexts() const;

        Q_INVOKABLE QStringList findSingleText(const QString& date, int localId) const;
        Q_INVOKABLE QVariantList findTexts(const QString& date) const;

        Q_INVOKABLE QVariantList findTextsByNameMask(const QString& nameMask) const;
        Q_INVOKABLE QVariantList findTextsByTagMask(const QString& tagMask) const;

        Q_INVOKABLE QVariantList findTextsByNameMaskAndDate(const QString& date,
                                                            const QString& nameMask) const;

        Q_INVOKABLE QVariantList findTextsByTagMaskAndDate(const QString& date,
                                                           const QString& tagMask) const;


        Q_INVOKABLE bool checkCardNameExists(QString name) const;
        Q_INVOKABLE QStringList getAllCardsNames() const;
        Q_INVOKABLE QString getCardDescription(QString name) const;


    private:

        QSqlError initBase() const;
        void createTablesIfNeeded() const;

        QVariantList fillRecordsSearchResults(QSqlQuery& query) const;

        bool logIfError(QSqlQuery& query, const QString& request) const;

        QVariantList findByFieldMaskAndDate(const QString& table, const QString& field,
                                            const QString& date, const QString& mask) const;

        QVariantList findByFieldMask(const QString& table, const QString& field,
                                     const QString& mask) const;

        QVariantList findByDate(const QString& table, const QString& date) const;

        QStringList findSingle(const QString& table, int fieldsCount,
                               const QString& date, int localId) const;

        int getTotalRows(const QString& keyField, const QString& table) const;

        int getMaxLocalId(const QString& table, const QString& date) const;

        void removeRow(const QString& table, const QString& date, int localId) const;

        void editRow(const QString& table, const QString& nameField, const QString& date,
                     int localId, const QString& name, const QString& tags,
                     const QString& description) const;


        const int audioFieldsCount = 7; //Later use walkaround with query.last() and query.at() + 1
        const int textFieldsCount = 7;
    };

}

#endif // SQLBASE_HPP
