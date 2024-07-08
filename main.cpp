#include <QCoreApplication>
#include <QtConcurrent>
#include <QTimer>
#include <QDateTime>

#include "dbWorker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
   static  DbWorker dbWorker;
    bool state = dbWorker.connect();

    static std::atomic_int count{0};
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, []()
                     { 
                        static std::atomic_int lastCount{0};
                        qDebug() << "inserts: " << count - lastCount << "all: " << count;
                        lastCount =  count.load() - lastCount; });
    timer.start(1000);

    qDebug() << "auto thread count detect:" << QThread::idealThreadCount();

    for (size_t i = 0; i < 10; i++)
    {
        QtConcurrent::run([=]()
                          {
                              qDebug() << "connect " << i << " is " << state;
                              if (!state)
                                  return;

                                while (1)                             
                            {
                                  if(dbWorker.send(QDateTime::currentDateTime().toSecsSinceEpoch()))
                                  count++;
                            } });
    }
    return a.exec();
}
