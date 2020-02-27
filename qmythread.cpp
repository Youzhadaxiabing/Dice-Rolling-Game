#include "qmythread.h"
#include <QWaitCondition>
#include <QMutex>
#include <QTime>
#include <QDebug>

QMutex mutex;
QWaitCondition newdataAvailable;

int seq = 0;
int diceValue1, diceValue2;

QThreadProducer::QThreadProducer()
{

}

QThreadConsumer::QThreadConsumer()
{

}

void QThreadProducer::run()
{
    m_stop=false;//启动线程时令m_stop=false
    seq=0;
    qsrand(QTime::currentTime().msec());//随机数初始化，qsrand是线程安全的

    while(!m_stop)//循环主体
    {
        mutex.lock();
        diceValue1 = qrand();
        //qDebug()<<diceValue1<<" ";
        diceValue1 = (diceValue1 % 6) + 1;

        diceValue2 = qrand();
        //qDebug()<<diceValue2<<" ";
        diceValue2 = (diceValue2 % 6) + 1;
        //qDebug()<<diceValue1<<" "<<diceValue2;
        mutex.unlock();
        if(is_roll)
        {
            seq++;
            newdataAvailable.wakeAll();//唤醒所有线程，有新数据了
        }
        is_roll = false;
        //msleep(450);
     }
}

void QThreadConsumer::run()
{
    m_stop = false;
    while(!m_stop)
    {
        mutex.lock();
        newdataAvailable.wait(&mutex);
        emit newValue(seq, diceValue1, diceValue2);
        mutex.unlock();
    }

}

void QThreadProducer::stopThread()
{
    QMutexLocker locker(&mutex);
    m_stop = true;
}

void QThreadProducer::rollDice()
{
    is_roll= true;
}
