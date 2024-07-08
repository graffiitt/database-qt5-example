#ifndef DB_WORKER_H
#define DB_WORKER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <qDebug>
#include "tsquene.h"

class DbWorker
{
private:
    TSQueue<QSqlDatabase>*  _queue;
   // QSqlDatabase _database;
    QString _dbName = "test.sqlite3";
    QString _nameConnection;
 

public:
    DbWorker();
    ~DbWorker();

    bool connect();
    bool send(qint64 time);
};

#endif