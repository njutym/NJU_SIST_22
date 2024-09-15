//商店类
#ifndef MYSTORE_H
#define MYSTORE_H

#include <QLabel>

#include "scene.h"

class MyStore : public Scene//商店类继承scene类
{
    Q_OBJECT
    friend class MainWindow;//友元申明
private:
    //道具参数标签
    QLabel *label1 = new QLabel;
    QLabel *label2 = new QLabel;
    QLabel *label3 = new QLabel;
    QLabel *label4 = new QLabel;
    QLabel *label5 = new QLabel;
    QLabel *label6 = new QLabel;
    //道具已购买数量
    int buytotal1;
    int buytotal2;
    int buytotal3;
    int buytotal4;
    int buytotal5;
    int buytotal6;
    //金币数量
    QLabel *moneylabel = new QLabel;
    int moneynum;
public:
    MyStore();
    void paintEvent(QPaintEvent *);//重写绘图事件
signals:
    void choose1();
    void choose2();
    void choose3();
    void choose4();
    void choose5();
    void choose6();
};

#endif // MYSTORE_H
