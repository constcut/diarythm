#include "SQLBase.hpp"

#include <QDebug>
#include <QSqlError>

using namespace diaryth;


QSqlError SQLBase::initBase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("base.sqlite");

    if (!db.open())
        return db.lastError();

    const QStringList tables = db.tables();
    for (int i = 0; i < tables.size(); ++i)
        qDebug() << "Table: " << tables[i];

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


bool SQLBase::createTablesIfNeeded() {

    qDebug() << initBase(); //Только когда была ошибка

    const QString audioTableCreate("CREATE TABLE IF NOT EXISTS audio ("
                           "audioId integer primary key autoincrement NOT NULL,"
                           "datePart text NOT NULL,"
                           "timePart text NOT NULL,"
                           "localId integer NOT NULL,"
                           "audioName text NOT NULL,"
                           "durationMs integer,"
                           "tags text,"
                           "description text);");

    QSqlQuery audioTableRequest = executeRequest(audioTableCreate);

    return audioTableRequest.isValid();
}



