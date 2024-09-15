//游玩界面类，场景布置继承scene类
#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#define PLAYER_HYPOTENUSE_SPEED (PLAYER_SPEED/(sqrt(2)))

#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QPixmap>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "myconfig.h"
#include "scene.h"
#include "enemy.h"
#include "eliteenemy.h"
#include "boss.h"
#include "player.h"
#include "mygood.h"
#include "myobstacle.h"
#include "bombeffect.h"

class PlayScene : public Scene
{
    Q_OBJECT
    friend class MainWindow;//友元申明
private:
    int num = 0;//定时器数据
    int hpmax = 0;//血量上限
    double PLAYER_SPEED;//玩家移动速度
    int atk;//玩家攻击力
    int unbeatabletime;//无敌时间
    int skilltime;//技能时间
    int suckhptime;//玩家1吸血时间
    int shoottime;//玩家2快速攻击时间
    int intervalsave = 0;//玩家2基础攻速储存
    int attacktime;//玩家5伤害爆炸时间
    int bosscollisionplayertime = 0;//防止Boss不断碰撞玩家
    bool isbosscollideplayer = false;//防止Boss不断碰撞玩家
    int numstore = 0;//防止Boss不断碰撞玩家

    QPixmap pix;//背景图片资源
    QPushButton *timebutton = new QPushButton;//计时器
    QTimer timer;//定时器1用于不断更新游戏帧
    QTimer timer2;//定时器2用于实现穿透弹

    QList<MyObstacle*>obstaclestore;//障碍物储存器

    QTimer *keyRespondTimer;//为实现按键像德芙一样丝滑，申明定时器3

    QList<int>keys;//为实现按键像德芙一样丝滑，申明按键储存器
    int verticalkey = 0;//0-未按下，1-向上，2-向下
    int horizontalkey = 0;//0-未按下，1-向左，2-向右
    bool keypressflag;//键盘按键是否按下
    int keyflag;//键盘按键

    Player m_player;//申明一个玩家
    QString player_hp_str;//玩家血量字符串
    QLabel *hplabel = new QLabel;//玩家血量标签
    int extendhp = 0;//优化玩家6血量条
    bool isupgrade1 = false;//玩家是否已经升级到中级
    bool isupgrade2 = false;//玩家是否已经升级到高级

    bool WinOrLose;//胜利或者失败判断
    QLabel *winLabel = new QLabel;//胜利标签
    QLabel *loseLabel = new QLabel;//失败标签

    Enemy enemystore[ENEMY_NUM];//敌人存储器
    EliteEnemy eliteenemystore[ELITEENEMY_NUM];//精英敌人存储器
    Boss boss;//Boss

    //爆炸数组
    Bombeffect m_bombs[BOMB_NUM];

    int enemyrecorder = 0;//敌人冷却计数器
    int eliteenemyrecorder = 0;//精英敌人冷却计数器
    int appeardirection;//敌人随机出现随机数
    int ENEMY_INTERVAL = 200;//敌人刷新间隔
    int ELITEENEMY_INTERVAL = 500;//精英敌人刷新间隔
    int ENEMY_BULLET_INTERVAL = 500;//精英敌人子弹冷却
    MyObstacle * boss_defend;//Boss护盾
    int totalattack = 0;//防止玩家靠近boss一下打很多血

    int dropgooddecrease = 0;//掉落物削减
    QList<MyGood*>goodstore;//掉落物储存器

    int coinnum = 0;//获得金币数量
    int expnum = 0;//经验数量
    int expmax = 200;//经验上限
    int bombnum = 1;//炸弹数量
    int resurrectionnum = 0;//复活次数
    double coindropprobability = 0;//掉金币概率
    int expincreasspeed = 0;//击敌经验增长速度
    int skillinterval = 60;//技能冷却

    QString coinstr;//玩家金币字符串
    QLabel *coinlabel = new QLabel;//金币数量标签
    QString expstr;//玩家经验字符串
    QLabel *explabel = new QLabel;//经验数量标签
    QString bombstr;//玩家炸弹字符串
    QLabel *bomblabel = new QLabel;//炸弹数量标签
    QString skillstr;//玩家技能字符串
    QLabel *skilllabel = new QLabel;//技能冷却标签

    QPixmap unbeatable_object;//无敌资源对象
    QRect unbeatableRect;//无敌图片矩形边框

    QPixmap skill_object;//技能资源对象
    QRect skillRect;//技能图片矩形边框

    //音乐
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
public:
    PlayScene(){};//构造函数，继承scene类
    PlayScene(int chooseindex,int hp,double speed,int attack,int bulletinterval,double bulletspeed,int Bombnum,int Resurrectionnum,double Coindropprobability,int Expincreasspeed,int Skillinterval);

    void playgame();//开始游戏
    void stopgame();//结束游戏

    void timerEvent(QTimerEvent *);//重写时间事件

    void reset();//初始化

    void setobstacle();//设置障碍物
    void setEnemyIndex();//设置敌人参数
    void set_Win_Lose_Label();//设置胜利失败的图片
    void Win_Lose_Judge(int Win_Lose_Flag);//胜利失败判断 0-win 1-lose

    void enemyToScene();//敌人登场
    void eliteenemyTOScene();//精英敌人登场
    void bossToScene();//Boss登场
    void boss_generate_defend();//Boss生成护盾

    void upgrade();//强化

    void keyPressEvent(QKeyEvent * event);//重写键盘按下事件
    void judgekey();//根据按键移动玩家并更新玩家图片
    void keyReleaseEvent(QKeyEvent * event);//重写键盘释放事件
    void mykeyevent();//自定义按键行为
    void bomb();//使用炸弹
    void skill();//使用技能

    void updatePosition();//更新玩家、敌人、子弹位置

    void collisionDetection();//碰撞判断
    void Bosscollision();//Boss碰撞判断

    void dropgood(int x,int y);//掉落物

    void paintEvent(QPaintEvent *);//重写绘图事件
signals:
    void increasemoney();//游戏结束增加全局金币数
public slots:
    void timerupdate();
};

#endif // PLAYSCENE_H
