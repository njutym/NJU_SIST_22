//数据类配置
#ifndef MYCONFIG_H
#define MYCONFIG_H

//游戏配置
#define GAME_WIDTH 1280
#define GAME_HEIGHT (1280*0.618)
#define GAME_RATE 2

//敌人配置
#define ENEMY_NUM 999
#define ENEMY_PATH ":/resource/enemy1.png"
#define ENEMY_SPEED 0.5

//精英敌人配置
#define ELITEENEMY_NUM 555
#define ELITEENEMY_LEFT_PATH ":/resource/enemy3left.png"
#define ELITEENEMY_RIGHT_PATH ":/resource/enemy3right.png"
#define ELITEENEMY_LEFT_PATH2 ":/resource/enemy4left.png"
#define ELITEENEMY_RIGHT_PATH2 ":/resource/enemy4right.png"

//精英敌人子弹配置
#define ENEMY_BULLET_PATH ":/resource/bullet7.png"
#define ENEMY_BULLET_SPEED 1

class MyConfig
{
public:
    MyConfig();
};

#endif // MYCONFIG_H
