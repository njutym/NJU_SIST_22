//普通敌人类
#ifndef ENEMY_H
#define ENEMY_H

#include <QLabel>

#include "myobject.h"
#include "myobstacle.h"

class Enemy : public MyObject//继承MyObject类
{
    friend class PlayScene;//声明友元
private:
    QLabel *hplabel = new QLabel;//血量标签
    QString enemy_hp_str;//血量字符串
    bool m_free;//存在状态
    bool isdefeated;//是否已经被消灭
    //优化追踪玩家策略，加一点小变量，免得生硬
    int xrand = 0;
    int yrand = 0;
    bool isrounding = false;//1-up,2-down,3-left,4-right
    int rounddirection = 0;
    double destination = 0;//绕障碍物终点
public:
    Enemy();
    void updatePosition(double playerx, double playery, QList<MyObstacle*>obstaclestore);
    void round();//绕障碍物
};

#endif // ENEMY_H
