#include <QCoreApplication>
#include <QtConcurrent>
#include <QTimer>
#include <QDateTime>

#include "dbWorker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTimer timer;
    static std::atomic_int count{0};

    QObject::connect(&timer, &QTimer::timeout, []()
                     { 
                        static std::atomic_int lastCount{0};
                        qDebug() << "inserts: " << count - lastCount << "all: " << count;
                        lastCount = count.load(); });
    timer.start(1000);

    qDebug() << "auto thread count detect:" << QThread::idealThreadCount();

    for (size_t i = 0; i < QThread::idealThreadCount(); i++)
    {
        QtConcurrent::run([=]()
                          {
                              DbWorker dbWorker(QString::number(i), &count);
                              bool state = dbWorker.connect();
                              qDebug() << "connect " << i << " is " << state;
                              if (!state)
                                  return;

                              for (size_t numInserts = 0; numInserts < 50000; numInserts++)
                              {
                                  dbWorker.send(QDateTime::currentDateTime().toSecsSinceEpoch());
                              } });
    }
    return a.exec();
}
