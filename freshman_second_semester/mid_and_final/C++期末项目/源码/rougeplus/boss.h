//Boss类
#ifndef BOSS_H
#define BOSS_H

#include <QLabel>

#include "myobject.h"

class Boss : public MyObject//boss类继承Myobject类
{
    friend class PlayScene;//声明友元
private:
    QLabel *hplabel = new QLabel;//血量标签
    QString enemy_hp_str;//血量字符串
    bool m_free;//存在状态
    int interval = 800;//子弹冷却
    double angle = 0;
    int randbullet3_1 = 0;
    int randbullet3_2 = 0;
    double center_x = 0;
    double center_y = 0;
public:
    Boss();
    void updatePosition(double x, double y);
    void shoot(double x,double y,int kind);
    void getangle(double x,double y);//子弹角度设置
    void bulletout(Bullet* m_bullet,int kind);//子弹发出处理
signals:

};

#endif // BOSS_H
