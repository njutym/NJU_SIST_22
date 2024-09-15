//玩家类
#ifndef PLAYER_H
#define PLAYER_H

#include "myobject.h"

class Player : public MyObject//玩家类继承MyObject类
{
    friend class PlayScene;//友元申明
private:
    int chooseindex = 1;//选择玩家参数
    int face = 0;//玩家朝向
public:
    Player();
    void loadchooseindex(int Chooseindex,int HP,double speed,int attack,int Bulletinterval,double Bulletspeed);//加载玩家参数
    int get_chooseindex();//给外界获得选择玩家参数
    void load_player(int flag);//重新加载玩家图片
    void shoot(int v_key,int h_key);//玩家类发射子弹
    void shoot1();//玩家1发动攻击
    void shoot2(int v_key,int h_key);//玩家2发射子弹
    void shoot3();//玩家3发射子弹
    void shoot4(int v_key,int h_key);//玩家4发射子弹
    void shoot5(int v_key,int h_key);//玩家5发射子弹
    void shoot6(int v_key,int h_key);//玩家6发射子弹
    void bulletout(Bullet *bullet);//玩家子弹射出后子弹的参数配置
};

#endif // PLAYER_H
