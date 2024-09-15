//子弹类
#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>

class Bullet
{
    //友元申明
    friend class Player;
    friend class EliteEnemy;
    friend class Boss;
    friend class PlayScene;
private:
    QPixmap m_Bullet;//子弹资源对象
    double m_X;//子弹x坐标
    double m_Y;//子弹y坐标
    double m_Speed;//子弹移动速度
    bool m_Free;//子弹是否闲置
    QRect m_Rect;//子弹的矩形边框(用于碰撞检测)

    bool isthrough = 0;//穿透子弹射中敌人

    int vertical_direction = 0;//玩家类子弹垂直方向
    int horizontal_direction = 0;//玩家子弹水平方向

    double direction_x = 0;//精英敌人类子弹x方向
    double direction_y = 0;//精英敌人类子弹y方向

    int bulletkind = 0;//Boss子弹种类
    double angle = 0;//Boss子弹1角度 //进制转换要求必须是double类型
    //Boss子弹初始发射位置
    double shootx = 0;
    double shooty = 0;

public:
    Bullet(){};//默认构造函数，本游戏并不使用
    Bullet(int v_key,int h_key,int bulletindex,double BULLET_SPEED);//玩家类的子弹构造函数
    Bullet(double x,double y);//精英敌人类子弹构造函数
    Bullet(double x,double y,int kind,double randangle,double shoot_x,double shoot_y);//Boss类子弹构造函数

    //玩家子弹移动策略
    void left();//左
    void right();//右
    void up();//上
    void down();//下
    void left_up();//左上
    void left_down();//左下
    void right_up();//右上
    void right_down();//右下
    void rightup_right();//右上和右之间
    void rightup_up();//右上和上之间
    void leftup_up();//左上和上之间
    void leftup_left();//左上和左之间
    void leftdown_left();//左下和左之间
    void leftdown_down();//左下和下之间
    void rightdown_down();//右下和下之间
    void rightdown_right();//右下和右之间

    void updatePosition();//玩家类子弹位置更新函数
    void eliteenemyupdatePosition(int num);//精英敌人类子弹位置更新函数
    void bossbulletupdatePosition(int currentx,int currenty);//Boss类子弹更新位置
};

#endif // BULLET_H
