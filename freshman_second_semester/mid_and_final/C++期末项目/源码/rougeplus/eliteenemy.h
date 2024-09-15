//精英敌人类
#ifndef ELITEENEMY_H
#define ELITEENEMY_H

#include <QLabel>
#include <QTimer>
#include <QTimerEvent>

#include "myobject.h"
#include "myobstacle.h"

class EliteEnemy : public MyObject//继承MyObject类
{
    friend class PlayScene;//友元声明
private:
    bool m_free;//精英敌人存在状态
    bool isdefeated;//精英敌人是否已经被消灭
    QLabel *hplabel = new QLabel;//血量标签
    QString enemy_hp_str;//血量字符串
    QTimer m_timer; // 定义的定时器
    //优化追踪玩家策略，加一点小变量，免得生硬
    int xrand = 0;
    int yrand = 0;
    bool isrounding = false;//1-up,2-down,3-left,4-right
    int rounddirection = 0;
    double destination = 0;//绕障碍物终点
    int randdirection = 1;
public:
    EliteEnemy();//构造函数
    void updatePosition(double playerx,double playery,QList<MyObstacle*>obstaclestore,int num);//更新精英敌人坐标
    void round();//绕障碍物
    void shoot(double x,double y,int interval,int num);//精英敌人发射子弹
    void timerEvent(QTimerEvent *);//重写时间事件
};

#endif // ELITEENEMY_H
