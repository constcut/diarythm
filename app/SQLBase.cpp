#include "SQLBase.hpp"

#include <QDebug>
#include <QSqlError>

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

    executeRequest(textTableCreate); //QSqlQuery textTableQuery =
}


void SQLBase::addAudioRecord(QString date, QString time, int localId,
                             QString name, quint64 durationMs)
{
    QString addAudioRequest = QString("INSERT INTO audio (datePart, timePart, localId, audioName, durationMs) "
    "VALUES('%1','%2','%3','%4','%5');").arg(date).arg(time).arg(localId).arg(name).arg(durationMs);

    executeRequest(addAudioRequest); //QSqlQuery addQuery =

    //Вероятно в начале была бы полезна проверка, что пары date + localId ещё нет
}


void SQLBase::editAudioRecord(QString date, int localId,
                              QString tags, QString description)
{
    QString updateAudioRequest =
            QString("UPDATE audio SET tags='%1', description='%2' "
                    "WHERE datePart='%4' AND localId='%5';")
            .arg(tags, description, date).arg(localId);

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


int SQLBase::recordsMaxLocalId(QString date)
{
    QString requestMaxId =
            QString("SELECT MAX(localId) FROM audio WHERE datePart='%1';")
            .arg(date);

    QSqlQuery maxIdQuery = executeRequest(requestMaxId);

    if (logIfError(maxIdQuery, requestMaxId) == false
            && maxIdQuery.next())
            return maxIdQuery.value(0).toInt();

    return -1;
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
    QString requestTotalRecords = "SELECT COUNT(audioId) FROM audio";
    QSqlQuery totalRecordsQuery = executeRequest(requestTotalRecords);

    if (totalRecordsQuery.next())
        return totalRecordsQuery.value(0).toInt();

    return 0;
}


QStringList SQLBase::findSingleRecord(QString date, int localId)
{
    QString requestSingleRecord =
            QString("SELECT * FROM audio WHERE datePart='%1' AND localId='%2';")
            .arg(date).arg(localId);

    QSqlQuery singleRecordQuery = executeRequest(requestSingleRecord);

    QStringList singleRecord;

    if (singleRecordQuery.next())
        for (int i = 1; i <= audioFieldsCount; ++i)
            singleRecord << singleRecordQuery.value(i).toString();

    return singleRecord;
}


QVariantList SQLBase::findRecords(QString date)
{
    QString requestRecords =
            QString("SELECT * FROM audio WHERE datePart='%1';")
            .arg(date);

    QSqlQuery recordsQuery = executeRequest(requestRecords);
    return fillRecordsSearchResults(recordsQuery);
}


QVariantList SQLBase::findByNameMask(QString nameMask)
{
    QString findRequest =
            QString("SELECT * FROM audio WHERE audioName LIKE '%1%';")
            .arg(nameMask);

    QSqlQuery recordsQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(recordsQuery);
}


QVariantList SQLBase::findByTagMask(QString tagMask)
{
    QString findRequest =
            QString("SELECT * FROM audio WHERE tags LIKE '%%1%';") //Для древовидных тэгов
            .arg(tagMask);

    QSqlQuery recordsQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(recordsQuery);
}


QVariantList SQLBase::findByNameMaskAndDate(QString date, QString nameMask)
{
    QString findRequest =
            QString("SELECT * FROM audio WHERE datePart='%1' AND audioName LIKE '%2%';")
            .arg(date, nameMask);

    QSqlQuery recordsQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(recordsQuery);
}


QVariantList SQLBase::findByTagMaskAndDate(QString date, QString tagMask)
{
    QString findRequest =
            QString("SELECT * FROM audio WHERE datePart='%1' AND tags LIKE '%%2%';")
            .arg(date, tagMask);

    QSqlQuery recordsQuery = executeRequest(findRequest);
    return fillRecordsSearchResults(recordsQuery);
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


