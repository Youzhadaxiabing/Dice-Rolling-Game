#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "qmythread.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QThreadProducer threadProducer;
    QThreadConsumer threadConsumer;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnClear_clicked();
    //void gamble_started();
    //void gamble_finished();
    void onthreadB_newValue(int seq,int diceValue1, int diceValue2);
    void on_pushButton_clicked();
    void on_btnRoll_clicked();
};

#endif // DIALOG_H
