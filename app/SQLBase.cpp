#include "SQLBase.hpp"

#include <QDebug>
#include <QSqlError>

#include <QDate>
#include <QTime>

using namespace diaryth;


SQLBase::SQLBase() {
    createTablesIfNeeded();
}


QSqlError SQLBase::initBase()
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


QSqlQuery SQLBase::executeRequest(const QString& requestBody)
{
    QSqlQuery request;
    if (request.exec(requestBody) == false)
        qDebug() << "Request error: " << request.lastError() << " for " << requestBody;
    return request;
}


void SQLBase::createTablesIfNeeded() {

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
}

//Возможно разумно убрать date & time и формировать их прямо тут да и localId
void SQLBase::addAudioRecord(QString date, QString time, int localId,
                             QString name, quint64 durationMs)
{
    QString addAudioRequest = QString("INSERT INTO audio (datePart, timePart, localId, audioName, durationMs) "
    "VALUES('%1','%2','%3','%4','%5');").arg(date,time).arg(localId).arg(name).arg(durationMs);

    executeRequest(addAudioRequest); //QSqlQuery addQuery =
    //Вероятно в начале была бы полезна проверка, что пары date + localId ещё нет
}


void SQLBase::editAudioRecord(QString date, int localId, QString name,
                              QString tags, QString description)
{
    QString updateAudioRequest =
            QString("UPDATE audio SET tags='%1', description='%2', audioName='%3' "
                    "WHERE datePart='%4' AND localId='%5';")
            .arg(tags, description, name, date).arg(localId);

    QSqlQuery updateQuery = executeRequest(updateAudioRequest);
    logIfError(updateQuery, updateAudioRequest);
}


void SQLBase::removeAudioRecord(QString date, int localId)
{
    QString deleteAudioRequest =
            QString("DELETE FROM audio WHERE datePart='%1' AND localId='%2';")
            .arg(date).arg(localId);

    QSqlQuery deleteQuery = executeRequest(deleteAudioRequest);
    logIfError(deleteQuery, deleteAudioRequest);
}


int SQLBase::getRecordsMaxLocalId(QString date)
{
    QString requestMaxId =
            QString("SELECT MAX(localId) FROM audio WHERE datePart='%1';")
            .arg(date);

    QSqlQuery maxIdQuery = executeRequest(requestMaxId);

    if (logIfError(maxIdQuery, requestMaxId) == false
            && maxIdQuery.next())
            return maxIdQuery.value(0).toInt();

    return 0;
}


bool SQLBase::logIfError(QSqlQuery& query, const QString &request)
{
    if (query.lastError().text().isEmpty() == false) {
        qDebug() << "Update request failed " << request
                 << "\n Error: " << query.lastError();
        return true;
    }
    return false;
}


int SQLBase::getTotalRecords()
{
    return getTotalRows("audioId", "audio");
}


QStringList SQLBase::findSingleRecord(QString date, int localId)
{
    return findSingle("audio", audioFieldsCount, date, localId);
}


QVariantList SQLBase::findRecords(QString date)
{
    return findByDate("audio", date);
}


QVariantList SQLBase::findRecordsByNameMask(QString nameMask)
{
    return findByFieldMask("audio", "audioName", nameMask);
}


QVariantList SQLBase::findRecordsByTagMask(QString tagMask)
{
    return findByFieldMask("audio", "tags", tagMask);
}


QVariantList SQLBase::findRecordsByNameMaskAndDate(QString date, QString nameMask)
{
    return findByFieldMaskAndDate("audio", "audioName", date, nameMask);
}


QVariantList SQLBase::findRecordsByTagMaskAndDate(QString date, QString tagMask)
{
    return findByFieldMaskAndDate("audio", "tags", date, tagMask);
}


QVariantList SQLBase::fillRecordsSearchResults(QSqlQuery& query)
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


void SQLBase::addText(QString name, QString text, QString tags, QString description)
{
    auto date = QDate::currentDate().toString("yyyy-MM-dd");
    auto time = QTime::currentTime().toString("HH:mm:ss");

    int localId = getTextsMaxLocalId(date) + 1;

    QString addTextRequest =
            QString("INSERT INTO texts (datePart, timePart, localId, textName, textValue, tags, description) "
    "VALUES('%1','%2','%3','%4','%5','%6','%7');").arg(date,time).arg(localId).arg(name, text, tags, description);

    executeRequest(addTextRequest);
}


void SQLBase::editText(QString date, int localId, QString name, //TODO refact (дублирование кода с audio версией)
                       QString tags, QString description)
{
    QString updateTextRequest =
            QString("UPDATE texts SET tags='%1', description='%2', textName='%3' "
                    "WHERE datePart='%4' AND localId='%5';")
            .arg(tags, description, name, date).arg(localId);

    QSqlQuery updateQuery = executeRequest(updateTextRequest);
    logIfError(updateQuery, updateTextRequest);
}


void SQLBase::removeText(QString date, int localId) //TODO refact (дублирование кода с audio версией)
{
    QString deleteAudioRequest =
            QString("DELETE FROM texts WHERE datePart='%1' AND localId='%2';")
            .arg(date).arg(localId);

    QSqlQuery deleteQuery = executeRequest(deleteAudioRequest);
    logIfError(deleteQuery, deleteAudioRequest);
}


int SQLBase::getTextsMaxLocalId(QString date) //TODO refact (дублирование кода с audio версией)
{
    QString requestMaxId =
            QString("SELECT MAX(localId) FROM texts WHERE datePart='%1';")
            .arg(date);

    QSqlQuery maxIdQuery = executeRequest(requestMaxId);

    if (logIfError(maxIdQuery, requestMaxId) == false
            && maxIdQuery.next())
            return maxIdQuery.value(0).toInt();

    return 0;
}


int SQLBase::getTotalTexts() //TODO refact (дублирование кода с audio версией)
{
    return getTotalRows("textId", "texts");
}


QStringList SQLBase::findSingleText(QString date, int localId) //TODO refact (дублирование кода с audio версией)
{
    return findSingle("texts", textFieldsCount, date, localId);
}


QVariantList SQLBase::findTexts(QString date) //TODO refact (дублирование кода с audio версией)
{
    return findByDate("texts", date);
}


QVariantList SQLBase::findTextsByNameMask(QString nameMask)
{
    return findByFieldMask("texts", "textName", nameMask);
}


QVariantList SQLBase::findTextsByTagMask(QString tagMask)
{
    return findByFieldMask("texts", "tags", tagMask);
}


QVariantList SQLBase::findTextsByNameMaskAndDate(QString date, QString nameMask)
{
    return findByFieldMaskAndDate("texts", "textName", date, nameMask);
}


QVariantList SQLBase::findTextsByTagMaskAndDate(QString date, QString tagMask)
{
    return findByFieldMaskAndDate("texts", "tags", date, tagMask);
}



QVariantList SQLBase::findByFieldMaskAndDate(QString table, QString field,
                                    QString date, QString mask)
{
    QString findRequest =
            QString("SELECT * FROM %1 WHERE datePart='%2' AND %3 LIKE '%%4%';")
            .arg(table, date, field, mask);

    QSqlQuery requestQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(requestQuery);
}


QVariantList SQLBase::findByFieldMask(QString table, QString field, QString mask)
{
    QString findRequest =
            QString("SELECT * FROM %1 WHERE %2 LIKE '%%3%';") //Для древовидных тэгов
            .arg(table, field, mask);

    QSqlQuery requestQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(requestQuery);
}


QVariantList SQLBase::findByDate(QString table, QString date)
{
    QString findRequest =
            QString("SELECT * FROM %1 WHERE datePart='%2';")
            .arg(table, date);

    QSqlQuery requestQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(requestQuery);
}


QStringList SQLBase::findSingle(QString table, int fieldsCount,
                        QString date, int localId)
{
    QString requestSingle =
            QString("SELECT * FROM %1 WHERE datePart='%2' AND localId='%2';")
            .arg(table, date).arg(localId);

    QSqlQuery singleQuery = executeRequest(requestSingle);
    QStringList single;

    if (singleQuery.next())
        for (int i = 1; i <= fieldsCount; ++i)
            single << singleQuery.value(i).toString();

    return single;
}


int SQLBase::getTotalRows(QString keyField, QString table)
{
    QString requestTotal = QString("SELECT COUNT(%1) FROM %2")
                           .arg(keyField, table);

    QSqlQuery totalQuery = executeRequest(requestTotal);

    if (totalQuery.next())
        return totalQuery.value(0).toInt();

    return 0;
}


