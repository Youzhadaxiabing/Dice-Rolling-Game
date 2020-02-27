#include "dialog.h"
#include "ui_dialog.h"

double banker_money = 0;
double gambler_money = 0;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setFixedSize(486, 610);
    connect(&threadConsumer,SIGNAL(newValue(int,int,int)),this,SLOT(onthreadB_newValue(int,int,int)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::closeEvent(QCloseEvent *event)
{//关闭窗口
    if (threadProducer.isRunning())
    {
        threadProducer.stopThread();
        threadProducer.wait();
    }

    if (threadConsumer.isRunning())
    {
        threadConsumer.terminate(); //因为threadB可能处于等待状态，所以用terminate强制结束
        threadConsumer.wait();//
    }
    event->accept();
}


void Dialog::on_btnStart_clicked()
{
//启动线程
    threadConsumer.start();
    threadProducer.start();

    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);

}


void Dialog::on_btnStop_clicked()
{
//结束线程
    threadProducer.stopThread();//结束线程的run()函数执行
    threadProducer.wait();//

    threadConsumer.terminate(); //因为threadB可能处于等待状态，所以用terminate强制结束
    threadConsumer.wait();//

    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
}


void Dialog::on_btnClear_clicked()
{
//清空文本
    ui->plainTextEdit->clear();
}

void Dialog::onthreadB_newValue(int seq,int diceValue1, int diceValue2)
{
    QString  str, str_banker, str_gambler;
    str=QString::asprintf("第 %d 次掷骰子，点数为：%d %d", seq, diceValue1, diceValue2);
    ui->plainTextEdit->appendPlainText(str);
    banker_money += 2.99;
    gambler_money -= 2.99;
    switch (diceValue1) {
    case 1:
            banker_money -= 0.88;
            gambler_money += 0.88;
            break;
    case 2:
        if(diceValue2 != 6)
        {
            banker_money -= 1.88;
            gambler_money += 1.88;
            break;
        }
        else break;
    case 3:
        if(diceValue2 != 5 && diceValue2 != 6)
        {
            banker_money -= 3.88;
            gambler_money += 3.88;
            break;
        }
        else break;
    case 4:
        if(diceValue2 == 1 || diceValue2 == 2 || diceValue2 == 3)
        {
            banker_money -= 8.88;
            gambler_money += 8.88;
            break;
        }
        else break;
    case 5:
        if(diceValue2 == 1 || diceValue2 == 2)
        {
            banker_money -= 12.88;
            gambler_money += 12.88;
            break;
        }
        else break;
    case 6:
        if(diceValue2 == 1)
        {
            banker_money -= 28.88;
            gambler_money += 28.88;
            break;
        }
        else break;
    default:
        break;
    }

    str_banker = QString::asprintf("Banker's money: ￥%.2f", banker_money);
    str_gambler = QString::asprintf("Gambler's money: ￥%.2f", gambler_money);
    ui->LabA->setText(str_banker);
    ui->LabB->setText(str_gambler);

    QPixmap pic1, pic2;
    QString filename1=QString::asprintf(":/images/d%d.jpg",diceValue1);
    QString filename2=QString::asprintf(":/images/d%d.jpg", diceValue2);
    pic1.load(filename1);
    pic2.load(filename2);

    ui->LabPic1->setPixmap(pic1);
    ui->LabPic2->setPixmap(pic2);
}

void Dialog::on_pushButton_clicked()
{
    ui->LabA->setText("Banker's money: ￥0");
    ui->LabB->setText("Gambler's money: ￥0");
    banker_money = 0;
    gambler_money = 0;
}

void Dialog::on_btnRoll_clicked()
{
    threadProducer.rollDice();
}
