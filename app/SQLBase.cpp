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
}


void SQLBase::editAudioRecord(QString date, int localId,
                              QString tags, QString description)
{
    QString updateAudioRequest =
            QString("UPDATE audio SET tags='%1', description='%2' "
                    "WHERE datePart='%4' AND localId='%5';")
            .arg(tags).arg(description).arg(date).arg(localId);

    QSqlQuery updateQuery = executeRequest(updateAudioRequest);

    if (updateQuery.lastError().text().isEmpty() == false)
        qDebug() << "Update request failed " << updateAudioRequest
                 << "\n Error: " << updateQuery.lastError();
}



void SQLBase::removeAudioRecord(QString date, int localId)
{
    QString deleteAudioRequest =
            QString("DELETE FROM audio WHERE datePart='%1' AND localId='%2';")
            .arg(date).arg(localId);

    QSqlQuery deleteQuery = executeRequest(deleteAudioRequest);

    if (deleteQuery.lastError().text().isEmpty() == false)
        qDebug() << "Delete request failed " << deleteAudioRequest
                 << "\n Error: " << deleteQuery.lastError();
}
