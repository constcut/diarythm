#include "SQLBase.hpp"

#include <QDebug>
#include <QSqlError>

#include <QDate>
#include <QTime>

using namespace diaryth;


SQLBase::SQLBase() {
    createTablesIfNeeded();
}


QSqlError SQLBase::initBase() const
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("base.sqlite");

    if (!db.open())
        return db.lastError();

    qDebug() << "List of existing sql tables";
    const QStringList tables = db.tables();
    for (const auto& table: tables)
        qDebug() << ":" << table;

    return QSqlError();
}


QSqlQuery SQLBase::executeRequest(const QString& requestBody) const
{
    QSqlQuery request;
    if (request.exec(requestBody) == false)
        qDebug() << "Request error: " << request.lastError() << " for " << requestBody;
    return request;
}


void SQLBase::createTablesIfNeeded() const {

    auto initResult = initBase();
    if (initResult.text().isEmpty() == false)
        qDebug() << "Init base error: " << initResult;

    const QString audioTableCreate("CREATE TABLE IF NOT EXISTS audio ("
                           "audioId integer primary key autoincrement NOT NULL,"
                           "datePart text NOT NULL,"
                           "timePart text NOT NULL,"
                           "localId integer NOT NULL,"
                           "audioName text NOT NULL,"
                           "durationMs integer,"
                           "tags text,"
                           "description text);");

    executeRequest(audioTableCreate); //QSqlQuery audioTableQuery =

    const QString textTableCreate("CREATE TABLE IF NOT EXISTS texts ("
                           "textId integer primary key autoincrement NOT NULL,"
                           "datePart text NOT NULL,"
                           "timePart text NOT NULL,"
                           "localId integer NOT NULL,"
                           "textName text NOT NULL,"
                           "textValue text NOT NULL,"
                           "tags text,"
                           "description text);");

    //Если разрешить редактирование текста, возможно стоит завести таблицу, где хранить несколько последних версий
    executeRequest(textTableCreate); //QSqlQuery textTableQuery =


    const QString cardRecordsTableCreate("CREATE TABLE IF NOT EXISTS diaryCardRecords ("
                           "diaryRecordId integer primary key autoincrement NOT NULL,"
                           "datePart text NOT NULL,"
                           "timePart text NOT NULL,"
                           "localId integer NOT NULL,"
                           "cardId integer NOT NULL,"
                           "cardDate text NOT NULL,"
                           "groupName text NOT NULL,"
                           "fieldName text NOT NULL,"
                           "fieldValue text);");

    executeRequest(cardRecordsTableCreate);

    const QString diaryCardsTableCreate("CREATE TABLE IF NOT EXISTS diaryCards ("
                           "diaryCardId integer primary key autoincrement NOT NULL,"
                           "cardName text NOT NULL UNIQUE,"
                           "cardDescription text NOT NULL,"
                           "jsonText text NOT NULL);");

    executeRequest(diaryCardsTableCreate);

}


void SQLBase::addAudioRecord(const QString& date, const QString& time,
                             int localId, const QString& name, quint64 durationMs) const
{
    QString addAudioRequest =
            QString("INSERT INTO audio (datePart, timePart, localId, audioName, durationMs) "
            "VALUES('%1','%2','%3','%4','%5');")
            .arg(date,time).arg(localId).arg(name).arg(durationMs);

    executeRequest(addAudioRequest);  //Вероятно в начале была бы полезна проверка, что пары date + localId ещё нет
}


void SQLBase::editAudioRecord(const QString& date, int localId, const QString& name,
                              const QString& tags, const QString& description) const
{
    editRow("audio", "audioName", date, localId, name, tags, description);
}


void SQLBase::removeAudioRecord(const QString& date, int localId) const
{
    removeRow("audio", date, localId);
}


int SQLBase::getRecordsMaxLocalId(const QString& date) const
{
    return getMaxLocalId("audio", date);
}


bool SQLBase::logIfError(QSqlQuery& query, const QString &request) const
{
    if (query.lastError().text().isEmpty() == false) {
        qDebug() << "Update request failed " << request
                 << "\n Error: " << query.lastError();
        return true;
    }
    return false;
}


int SQLBase::getTotalRecords() const
{
    return getTotalRows("audioId", "audio");
}


QStringList SQLBase::findSingleRecord(const QString& date, int localId) const
{
    return findSingle("audio", audioFieldsCount, date, localId);
}


QVariantList SQLBase::findRecords(const QString& date) const
{
    return findByDate("audio", date);
}


QVariantList SQLBase::findRecordsByNameMask(const QString& nameMask) const
{
    return findByFieldMask("audio", "audioName", nameMask);
}


QVariantList SQLBase::findRecordsByTagMask(const QString& tagMask) const
{
    return findByFieldMask("audio", "tags", tagMask);
}


QVariantList SQLBase::findRecordsByNameMaskAndDate(const QString& date,
                                                   const QString& nameMask) const
{
    return findByFieldMaskAndDate("audio", "audioName", date, nameMask);
}


QVariantList SQLBase::findRecordsByTagMaskAndDate(const QString& date,
                                                  const QString& tagMask) const
{
    return findByFieldMaskAndDate("audio", "tags", date, tagMask);
}


QVariantList SQLBase::fillRecordsSearchResults(QSqlQuery& query) const
{
     QVariantList records;

     while(query.next())
     {
         QStringList singleRecord;
         for (int i = 1; i <= audioFieldsCount; ++i)
             singleRecord << query.value(i).toString();

         records << singleRecord;
     }

     return records;
}


void SQLBase::addText(const QString& name, const QString& text,
                      const QString& tags, const QString& description) const
{
    auto date = QDate::currentDate().toString("yyyy-MM-dd");
    auto time = QTime::currentTime().toString("HH:mm:ss");

    int localId = getTextsMaxLocalId(date) + 1;

    QString addTextRequest =
            QString("INSERT INTO texts (datePart, timePart, localId, textName, textValue, tags, description) "
            "VALUES('%1','%2','%3','%4','%5','%6','%7');")
            .arg(date,time).arg(localId).arg(name, text, tags, description);

    executeRequest(addTextRequest);
}


void SQLBase::editText(const QString& date, int localId, const QString& name,
                       const QString& tags, const QString& description) const
{
    editRow("texts", "textName", date, localId, name, tags, description);
}


void SQLBase::removeText(const QString& date, int localId) const
{
    removeRow("texts", date, localId);
}


int SQLBase::getTextsMaxLocalId(const QString& date) const
{
    return getMaxLocalId("texts", date);
}


int SQLBase::getTotalTexts() const
{
    return getTotalRows("textId", "texts");
}


QStringList SQLBase::findSingleText(const QString& date, int localId) const
{
    return findSingle("texts", textFieldsCount, date, localId);
}


QVariantList SQLBase::findTexts(const QString& date) const
{
    return findByDate("texts", date);
}


QVariantList SQLBase::findTextsByNameMask(const QString& nameMask) const
{
    return findByFieldMask("texts", "textName", nameMask);
}


QVariantList SQLBase::findTextsByTagMask(const QString& tagMask) const
{
    return findByFieldMask("texts", "tags", tagMask);
}


QVariantList SQLBase::findTextsByNameMaskAndDate(const QString& date,
                                                 const QString& nameMask) const
{
    return findByFieldMaskAndDate("texts", "textName", date, nameMask);
}


QVariantList SQLBase::findTextsByTagMaskAndDate(const QString& date,
                                                const QString& tagMask) const
{
    return findByFieldMaskAndDate("texts", "tags", date, tagMask);
}


QVariantList SQLBase::findByFieldMaskAndDate(const QString& table, const QString& field,
                                             const QString& date, const QString& mask) const
{
    QString findRequest =
            QString("SELECT * FROM %1 WHERE datePart='%2' AND %3 LIKE '%%4%';")
            .arg(table, date, field, mask);

    QSqlQuery requestQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(requestQuery);
}


QVariantList SQLBase::findByFieldMask(const QString& table, const QString& field,
                                      const QString& mask) const
{
    QString findRequest =
            QString("SELECT * FROM %1 WHERE %2 LIKE '%%3%';")
            .arg(table, field, mask);

    QSqlQuery requestQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(requestQuery);
}


QVariantList SQLBase::findByDate(const QString& table, const QString& date) const
{
    QString findRequest =
            QString("SELECT * FROM %1 WHERE datePart='%2';")
            .arg(table, date);

    QSqlQuery requestQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(requestQuery);
}


QStringList SQLBase::findSingle(const QString& table, int fieldsCount,
                                const QString& date, int localId) const
{
    QString requestSingle =
            QString("SELECT * FROM %1 WHERE datePart='%2' AND localId='%3';")
            .arg(table, date).arg(localId);

    QSqlQuery singleQuery = executeRequest(requestSingle);
    QStringList single;

    if (singleQuery.next())
        for (int i = 1; i <= fieldsCount; ++i)
            single << singleQuery.value(i).toString();

    return single;
}


int SQLBase::getTotalRows(const QString& keyField, const QString& table) const
{
    QString requestTotal = QString("SELECT COUNT(%1) FROM %2")
                           .arg(keyField, table);

    QSqlQuery totalQuery = executeRequest(requestTotal);

    if (totalQuery.next())
        return totalQuery.value(0).toInt();

    return 0;
}


int SQLBase::getMaxLocalId(const QString& table, const QString& date) const
{
    QString requestMaxId =
            QString("SELECT MAX(localId) FROM %1 WHERE datePart='%2';")
            .arg(table, date);

    QSqlQuery maxIdQuery = executeRequest(requestMaxId);

    if (logIfError(maxIdQuery, requestMaxId) == false
            && maxIdQuery.next())
            return maxIdQuery.value(0).toInt();

    return 0;
}


void SQLBase::removeRow(const QString& table, const QString& date, int localId) const
{
    QString deleteRequest =
            QString("DELETE FROM %1 WHERE datePart='%2' AND localId='%3';")
            .arg(table, date).arg(localId);

    QSqlQuery deleteQuery = executeRequest(deleteRequest);
    logIfError(deleteQuery, deleteRequest);
}


void SQLBase::editRow(const QString& table, const QString& nameField, const QString& date, int localId,
                      const QString& name, const QString& tags, const QString& description) const
{
    QString updateRequest =
            QString("UPDATE %1 SET tags='%2', description='%3', %4='%5' "
                    "WHERE datePart='%6' AND localId='%7';")
            .arg(table, tags, description, nameField, name, date).arg(localId);

    QSqlQuery updateQuery = executeRequest(updateRequest);
    logIfError(updateQuery, updateRequest);
}



bool SQLBase::checkCardNameExists(const QString& name) const
{
    QString checkRequest =
            QString("SELECT * FROM diaryCards WHERE cardName='%1';").arg(name);

    QSqlQuery checkQuery = executeRequest(checkRequest);

    if (checkQuery.next())
        return true;

    return false;
}


QStringList SQLBase::getAllCardsNames() const
{
     QString allNamesRequest = QString("SELECT cardName FROM diaryCards;");
     QSqlQuery allNamesQuery = executeRequest(allNamesRequest);

     QStringList allNames;
     while (allNamesQuery.next())
        allNames.append(allNamesQuery.value(0).toString());

     return allNames;
}


QString SQLBase::getCardDescription(const QString& name) const
{
    QString descriptionRequest =
            QString("SELECT cardDescription FROM diaryCards WHERE cardName='%1';").arg(name);

    QSqlQuery descriptionQuery = executeRequest(descriptionRequest);

    if (descriptionQuery.next())
        return descriptionQuery.value(0).toString();

    return {};
}


QString SQLBase::getCardJSON(const QString& name) const
{
    QString descriptionRequest =
            QString("SELECT jsonText FROM diaryCards WHERE cardName='%1';").arg(name); //can be done tiny ref ^1,2

    QSqlQuery descriptionQuery = executeRequest(descriptionRequest);

    if (descriptionQuery.next())
        return descriptionQuery.value(0).toString();

    return {};
}


int SQLBase::getCardId(const QString& name) const
{
    QString descriptionRequest =
            QString("SELECT jsonText FROM diaryCards WHERE cardName='%1';").arg(name); //can be done tiny ref ^1,2

    QSqlQuery descriptionQuery = executeRequest(descriptionRequest);

    if (descriptionQuery.next())
        return descriptionQuery.value(0).toInt();

    return -1;
}



int SQLBase::getTotalCards() const
{
    return getTotalRows("cardName", "diaryCards");
}



int SQLBase::getCardRecordsMaxLocalId(const QString& date) const
{
    return getMaxLocalId("diaryCardRecords", date);
}


int SQLBase::getTotalCardRecords() const
{
    return getTotalRows("diaryCardRecords", "diaryRecordId");
}


int SQLBase::getCardRecordsCountOnDate(const QString& date) const
{
    QString countRequest =
            QString("SELECT COUNT(localId) FROM diaryCardRecords WHERE datePart='%1';")
            .arg(date);

    QSqlQuery countQuery = executeRequest(countRequest);

    if (countQuery.next())
        return countQuery.value(0).toInt();

    return 0;
}
