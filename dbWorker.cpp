#include "dbWorker.h"

DbWorker::DbWorker()
{
    _queue = new TSQueue<QSqlDatabase>;

    // _database = QSqlDatabase::addDatabase("QSQLITE", "nameConnection");
    // _database.setDatabaseName(_dbName);
}

DbWorker::~DbWorker()
{
   delete _queue;
}

bool DbWorker::connect()
{
    for (int i = 0; i < 1; i++)
    {
        QSqlDatabase _database = QSqlDatabase::addDatabase("QSQLITE", QString::number(i));
        _database.setDatabaseName(_dbName);
        _database.open();

        _queue->push(_database);
    }
    return 1;
}

bool DbWorker::send(qint64 time)
{
    bool state = 1;

    QSqlDatabase num = _queue->pop();

    QSqlQuery _query(num);
    _query.prepare("INSERT INTO raw_data (tm)"
                   " VALUES (:time)");
    _query.bindValue(":time", time);

    // std::mutex mutex;
    // mutex.lock();
    if (!_query.exec())
    {
        qDebug() << "db error: " << _query.lastError();
        state = 0;
    }
    // mutex.unlock();
    _queue->push(num);

    return state;
}
