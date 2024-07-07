#include "dbWorker.h"

DbWorker::DbWorker(QString nameConnection, std::atomic_int *counter)
{
    _database = QSqlDatabase::addDatabase("QSQLITE", nameConnection);
    _database.setDatabaseName(_dbName);
    _counter = counter;
    _nameConnection = nameConnection;
}

DbWorker::~DbWorker()
{
}

bool DbWorker::connect()
{
    return _database.open();
}

void DbWorker::send(qint64 time)
{
    QSqlQuery _query(QSqlDatabase::database(_nameConnection));
    _query.prepare("INSERT INTO raw_data (tm)"
                   " VALUES (:time)");
    _query.bindValue(":time", time);
    std::mutex mutex;
    mutex.lock();
    if (!_query.exec())
    {
        qDebug() << "db error: " << _query.lastError();
    }
    else{
        *_counter = *_counter + 1;
     }
    mutex.unlock();
}
