//选择人物类
#ifndef CHOOSEPLAYER_H
#define CHOOSEPLAYER_H

#include <QLabel>
#include <QPushButton>

#include "scene.h"

class ChoosePlayer : public Scene
{
    Q_OBJECT//不能删！！！不删才能用信号和槽
    friend class MainWindow;//友元申明
private:
    int chooseindex = 1;
    //选择选中标签
    QString choosestr;
    QLabel *chooselabel = new QLabel;

    //玩家参数标签
    QLabel *label1 = new QLabel;
    QLabel *label2 = new QLabel;
    QLabel *label3 = new QLabel;
    QLabel *label4 = new QLabel;
    QLabel *label5 = new QLabel;
    QLabel *label6 = new QLabel;

    //玩家1属性
    int hp1 = 60;
    double speed1 = 1;
    int attack1 = 3;
    int bulletinterval1 = 220;
    double bulletspeed1 = 5;

    //玩家2属性
    int hp2 = 40;
    double speed2 = 0.8;
    int attack2 = 2;
    int bulletinterval2 = 70;
    double bulletspeed2 = 5;

    //玩家3属性
    int hp3 = 30;
    double speed3 = 0.8;
    int attack3 = 2;
    int bulletinterval3 = 150;
    double bulletspeed3 = 5;

    //玩家4属性
    int hp4 = 50;
    double speed4 = 0.8;
    int attack4 = 5;
    int bulletinterval4 = 125;
    double bulletspeed4 = 2;

    //玩家5属性
    int hp5 = 40;
    double speed5 = 0.8;
    int attack5 = 1;
    int bulletinterval5 = 160;
    double bulletspeed5 = 2;

    //玩家6属性
    int hp6 = 5201314;
    double speed6 = 3;
    int attack6 = 999;
    int bulletinterval6 = 50;
    double bulletspeed6 = 3.5;

    QLabel *label6_1;
    QPushButton *btn6_2;

public:
    explicit ChoosePlayer(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *);//重写绘图事件
signals:
    void choose1();
    void choose2();
    void choose3();
    void choose4();
    void choose5();
    void choose6();
};

#endif // CHOOSEPLAYER_H
