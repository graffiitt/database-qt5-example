#ifndef DB_WORKER_H
#define DB_WORKER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <qDebug>

class DbWorker
{
private:
    QSqlDatabase _database;
    QString _dbName = "test.sqlite3";
    QString _nameConnection;
    std::atomic_int *_counter;

public:
    DbWorker(QString nameConnection, std::atomic_int *counter);
    ~DbWorker();

    bool connect();
    void send(qint64 time);
};

#endif