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

    /*if (tables.contains("audio", Qt::CaseInsensitive)
        && tables.contains("text", Qt::CaseInsensitive))
        return QSqlError();*/

    return QSqlError();
}


QSqlQuery SQLBase::executeRequest(const QString& requestBody)
{
    //lastRequestBody = requestBody;
    QSqlQuery request;
    if (request.exec(requestBody) == false)
        qDebug() << "Request error: " << requestBody;  //notifySqlError(request);
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

    executeRequest(audioTableCreate); //QSqlQuery audioTableRequest =

    const QString textTableCreate("CREATE TABLE IF NOT EXISTS texts ("
                           "textId integer primary key autoincrement NOT NULL,"
                           "datePart text NOT NULL,"
                           "timePart text NOT NULL,"
                           "localId integer NOT NULL,"
                           "textName text NOT NULL,"
                           "textValue text NOT NULL,"
                           "tags text,"
                           "description text);");

    executeRequest(textTableCreate); //QSqlQuery textTableRequest =
}



