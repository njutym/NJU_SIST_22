//人物类，作为玩家类、敌人类、精英敌人类、Boss类继承的父类
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QWidget>

#include "bullet.h"

class MyObject : QWidget
{
    //友元申明
    friend class Player;
    friend class Enemy;
    friend class EliteEnemy;
    friend class Boss;
    friend class PlayScene;
private:
    double m_X;//x坐标
    double m_Y;//y坐标

    int hp;//血量
    int m_attack;//攻击力
    double m_speed;//速度
    int bulletinterval;//子弹冷却
    double bulletspeed;//子弹速度
    int m_recorder = 0;//子弹发射冷却限制

    QPixmap m_object;//资源对象
    QRect m_Rect;//图片矩形边框

    QList<Bullet*>bulletstore;//子弹存储器
public:
    MyObject();
};

#endif // MYOBJECT_H
