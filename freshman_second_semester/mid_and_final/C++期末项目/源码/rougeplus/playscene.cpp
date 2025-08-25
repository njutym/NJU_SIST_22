#include "playscene.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>
#include <ctime>
#include <QUrl>
#include <QtMultimedia>

PlayScene::PlayScene(int chooseindex,int hp,double speed,int attack,int bulletinterval,double bulletspeed,int Bombnum,int Resurrectionnum,double Coindropprobability,int Expincreasspeed,int Skillinterval)//构造函数
{
    //添加音乐
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.5);

    //前奏
    player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc:/resource/Prelude.m4a"));

    srand((unsigned int)time(NULL));//时间种子随机数
    timer.setInterval(GAME_RATE);//设置游戏帧率
    timer2.setInterval(300);//穿透弹作用间隔
    startTimer(1000);//启动计时器
    unbeatabletime = 100;//一开始禁用无敌时间
    suckhptime = 100;//一开始禁用吸血
    shoottime = 100;//一开始禁用加速攻击
    attacktime = 100;//一开始禁用伤害爆炸攻击

    //初始化计时器
    timebutton->setParent(this);
    timebutton->setFixedSize(100,40);
    timebutton->move(this->width()-timebutton->width(),20);

    //初始化操作
    this->setWindowTitle("坚持生存吧！");
    set_Win_Lose_Label();
    setEnemyIndex();
    pix.load(":/resource/WindowBackground.gif");

    //设置障碍物
    setobstacle();

    //加载玩家参数
    m_player.loadchooseindex(chooseindex,hp,speed,attack,bulletinterval,bulletspeed);
    hpmax = hp;
    PLAYER_SPEED = speed;
    atk = attack;
    bombnum = Bombnum;
    if(m_player.chooseindex == 6) bombnum = 999;//玩家6拥有999初始炸弹
    resurrectionnum = Resurrectionnum;
    if(m_player.chooseindex == 6) resurrectionnum = 999;//玩家6可以复活999次
    if(m_player.chooseindex == 3) resurrectionnum++;//玩家3可以额外复活一次
    coindropprobability = Coindropprobability;
    expincreasspeed = Expincreasspeed;
    skillinterval = Skillinterval;
    skilltime = skillinterval;
    intervalsave = bulletinterval;

    //玩家6实现999金币、999经验
    if(m_player.chooseindex == 6)
    {
        coinnum = 999;
        expmax = 999;
        expnum = 999;
    }

    //设置Boss护盾
    boss_defend = new MyObstacle(8,boss.m_X,boss.m_Y);
    boss_defend->m_X = -300;
    boss_defend->m_Y = -300;
    boss_defend->m_Rect.moveTo(-300,-300);

    connect(&timer,&QTimer::timeout,this,&PlayScene::timerupdate);

    //不断更新事件
    connect(&timer,&QTimer::timeout,this,[=]()
    {
        //更新玩家血量条
        player_hp_str = QString("血量:%1").arg(m_player.hp);
        hplabel->setText(player_hp_str);

        //更新获得的金币数量
        coinstr = QString("获得金币:%1").arg(coinnum);
        coinlabel->setText(coinstr);

        //更新获得的经验数量
        expstr = QString("经验值:%1/%2").arg(expnum).arg(expmax);
        explabel->setText(expstr);

        //更新玩家炸弹的数量
        bombstr = QString("炸弹数量:%1").arg(bombnum);
        bomblabel->setText(bombstr);

        //更新玩家技能冷却
        skillstr = QString("技能冷却:%1/%2").arg(skilltime).arg(skillinterval);//玩家技能字符串
        skilllabel->setText(skillstr);

        //玩家6实现无敌
        if(chooseindex == 6) unbeatabletime = 0;

        enemyToScene();//敌人登场
        eliteenemyTOScene();//精英敌人登场
        boss_generate_defend();//Boss生成护盾
        updatePosition();//更新位置
        collisionDetection();//碰撞判断
        upgrade();//强化

        if(num - numstore == 1) isbosscollideplayer = false;//重置Boss碰撞玩家时间

        //胜利失败判断
        if(m_player.hp <= 0)
        {
            if(resurrectionnum > 0)//复活,但属性削弱
            {
                resurrectionnum = 0;
                m_player.hp = hpmax;
                m_player.m_speed -= 0.5;
                PLAYER_SPEED -= 0.5;
                for(int i = 0;i < 3;i++)
                {
                    if(m_player.m_attack >= 2)
                    {
                        m_player.m_attack -= 1;
                        atk -= 1;
                    }
                }
                for(int i = 0;i < 20;i++)
                {
                    m_player.bulletinterval++;
                }
            }
            else
            {
                player->stop();
                Win_Lose_Judge(0);
                timer.stop();
            }
        }
        else if(boss.hp <= 0)
        {
            player->stop();
            coinnum+=300;
            Win_Lose_Judge(1);
            timer.stop();
        }
    });

    connect(&timer2,&QTimer::timeout,this,[=]()
    {
        update();
        for(int i = 0;i < m_player.bulletstore.size();i++)
        {
            if(m_player.bulletstore[i]->isthrough) m_player.bulletstore[i]->isthrough = false;
        }
    });

    //为实现丝滑移动建立信号
    keyRespondTimer = new QTimer(this);
    connect(keyRespondTimer,&QTimer::timeout,this,&PlayScene::mykeyevent);
}

void PlayScene::timerupdate()
{
    QWidget :: update();
}

void PlayScene::playgame()//开始游戏
{
    timer.start();
    timer2.start();
}

void PlayScene::stopgame()//结束游戏
{
    timer2.stop();
    //析构无用子弹
    for(int i = 0;i < m_player.bulletstore.size();i++)
    {
        Bullet * p = m_player.bulletstore[i];
        delete p;
        p = nullptr;
    }
    m_player.bulletstore.clear();
    m_player.bulletstore.~QList();
    for(int i = 0;i < ELITEENEMY_NUM;i++)
    {
        for(int j = 0;j < eliteenemystore[i].bulletstore.size();j++)
        {
            Bullet * p = eliteenemystore[i].bulletstore[j];
            delete p;
            p = nullptr;
        }
        eliteenemystore[i].bulletstore.clear();
        eliteenemystore[i].bulletstore.~QList();
    }
    for(int i = 0;i < boss.bulletstore.size();i++)
    {
        if(boss.bulletstore[i]->m_Free)
        {
            Bullet * p = boss.bulletstore[i];
            delete p;
            p = nullptr;
            boss.bulletstore.removeAt(i);
        }
    }
    //析构障碍物
    for(int i = 0;i < obstaclestore.size();i++)
    {
        MyObstacle * p = obstaclestore[i];
        delete p;
        p = nullptr;
        obstaclestore.removeAt(i);
    }
    //更新全局金币
    emit this->increasemoney();
}

void PlayScene::timerEvent(QTimerEvent *)//计时器事件
{
    num++;
    timebutton->setText(QString::number(PlayScene::num));
    timebutton->show();
    unbeatabletime++;
    if(m_player.chooseindex!=6) coinnum++;
    totalattack = 0;
    if(skilltime < skillinterval) skilltime++;
    suckhptime++;
    shoottime++;
    if(shoottime == 10)//玩家2攻速恢复
    {
        m_player.bulletinterval = intervalsave;
    }
    attacktime++;
    if(num == 5)//第5秒开始放前奏
    {
        player->play();
    }
    if(num == 27)//第27秒开始放山魔王的宫殿里
    {
        player->stop();
        player->setSource(QUrl("qrc:/resource/BackgroundMusic.ogg"));
        player->play();
        audioOutput->setVolume(0.8);
    }
    if(num == 115)//第115秒降低音量
    {
        audioOutput->setVolume(0.15);
    }
    if(num == 176)//第176秒开始放让风告诉你
    {
        player->stop();
        player->setSource(QUrl("qrc:/resource/BossMusic.ogg"));
        player->setLoops(-1);
        player->play();
        audioOutput->setVolume(0.35);
    }
}

void PlayScene::reset()//初始化
{
    num = 0;//计时器清零
    //设置玩家初始状态
    m_player.m_X = GAME_WIDTH * 0.5 - m_player.m_object.width() * 0.5;
    m_player.m_Y = GAME_HEIGHT * 0.5 - m_player.m_object.height() * 0.5;
    m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
    m_player.load_player(4);
    keypressflag = false;//设置键盘按键初始状态

    //玩家血量标签
    QFont font;
    hplabel->setParent(this);
    font.setFamily("华文新魏");
    font.setPointSize(20);
    hplabel->setFont(font);
    player_hp_str = QString("血量:%1").arg(m_player.hp);
    hplabel->setText(player_hp_str);
    hplabel->move(GAME_WIDTH/2-400,-200);
    if(m_player.chooseindex == 6) extendhp = 92;

    //局内获得金币数量显示
    coinlabel->setParent(this);
    font.setFamily("华文新魏");
    font.setPointSize(20);
    coinlabel->setFont(font);
    coinstr = QString("获得金币:%1").arg(coinnum);
    coinlabel->setText(coinstr);
    coinlabel->move(GAME_WIDTH/2-600,-200);
    coinlabel->show();

    //玩家经验标签
    explabel->setParent(this);
    font.setFamily("华文新魏");
    font.setPointSize(20);
    explabel->setFont(font);
    expstr = QString("经验值:%1/%2").arg(expnum).arg(expmax);
    explabel->setText(coinstr);
    explabel->move(GAME_WIDTH/2-175,-200);
    explabel->show();

    //玩家炸弹数量
    bomblabel->setParent(this);
    font.setFamily("华文新魏");
    font.setPointSize(20);
    bomblabel->setFont(font);
    bombstr = QString("炸弹数量:%1").arg(bombnum);
    bomblabel->setText(bombstr);
    bomblabel->move(GAME_WIDTH/2+100,-200);
    bomblabel->show();

    //技能冷却时间
    skilllabel->setParent(this);
    font.setFamily("华文新魏");
    font.setPointSize(20);
    skilllabel->setFont(font);
    skillstr = QString("技能冷却:%1/%2").arg(skilltime).arg(skillinterval);//玩家技能字符串
    skilllabel->setText(skillstr);
    skilllabel->move(GAME_WIDTH/2+300,-200);
    skilllabel->show();

    //无敌资源图像
    unbeatable_object.load(":/resource/unbeatable.png");
    unbeatable_object = unbeatable_object.scaled(30,30);
    unbeatableRect.setWidth(unbeatable_object.width());
    unbeatableRect.setHeight(unbeatable_object.height());
    unbeatableRect.moveTo(-200,-200);

    //技能资源图像
    skill_object.load(":/resource/good10.png");
    skill_object = skill_object.scaled(30,30);
    skillRect.setWidth(skill_object.width());
    skillRect.setHeight(skill_object.height());
    skillRect.moveTo(-200,-200);
}

void PlayScene::setobstacle()//设置障碍物
{
    MyObstacle *m_obstacle = new MyObstacle(5,175,150);
    obstaclestore.append(m_obstacle);
    MyObstacle *m_obstacle2 = new MyObstacle(5,195,GAME_HEIGHT - 350);
    m_obstacle2->m_object = m_obstacle2->m_object.scaled(88,88);
    m_obstacle2->m_Rect.setWidth(m_obstacle2->m_object.width());
    m_obstacle2->m_Rect.setHeight(m_obstacle2->m_object.height());
    obstaclestore.append(m_obstacle2);
    MyObstacle *m_obstacle3 = new MyObstacle(5,GAME_WIDTH - 250,400);
    obstaclestore.append(m_obstacle3);
    MyObstacle *m_obstacle5 = new MyObstacle(7,GAME_WIDTH/2-100,200);
    obstaclestore.append(m_obstacle5);
    for(int i = 0;i < 4;i++)
    {
        MyObstacle *m_obstacle6 = new MyObstacle(6,GAME_WIDTH/2+50+i%2*200,GAME_HEIGHT-350+i/2*200);
        obstaclestore.append(m_obstacle6);
    }
}

void PlayScene::setEnemyIndex()//初始化敌人参数
{
    //普通敌人
    for(int i = 0;i < ENEMY_NUM;i++)
    {
        enemystore[i].hp = 1;
        enemystore[i].m_speed = 0.3;
    }
    for(int i = 0;i < ENEMY_NUM;i++)
    {
        enemystore[i].hplabel->setParent(this);//血量标签
        //随机追踪玩家的方向
        enemystore[i].xrand = rand()%m_player.m_Rect.width();
        enemystore[i].yrand = rand()%m_player.m_Rect.height();
    }

    //精英敌人
    for(int i = 0;i < ELITEENEMY_NUM;i++)
    {
        eliteenemystore[i].hp = 2;
        eliteenemystore[i].m_speed = 0.4;
        eliteenemystore[i].hplabel->setParent(this);//血量标签
        //随机追踪玩家的方向
        eliteenemystore[i].xrand = rand()%m_player.m_Rect.width();
        eliteenemystore[i].yrand = rand()%m_player.m_Rect.height();
    }

    //Boss
    boss.hplabel->setParent(this);
    boss.hp = 500;
    boss.m_speed = 0.4;
}

void PlayScene::set_Win_Lose_Label()//设置胜利失败标签
{
    //胜利标签
    QPixmap tmpPix;
    tmpPix.load(":/resource/Win.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap((tmpPix));
    winLabel->setParent(this);
    winLabel->move((this->width()-tmpPix.width())*0.5,-tmpPix.height());
    //失败标签
    QPixmap tmpPix2;
    tmpPix2.load(":/resource/Lose.jpg");
    loseLabel->setGeometry(0,0,tmpPix2.width(),tmpPix2.height());
    loseLabel->setPixmap((tmpPix2));
    loseLabel->setParent(this);
    loseLabel->move((this->width()-tmpPix2.width())*0.5,-tmpPix2.height());
}

void PlayScene::Win_Lose_Judge(int Win_Lose_Flag)//判断是否胜利失败，并展示动画
{
    if(Win_Lose_Flag == 0)//失败
    {
        QPropertyAnimation * animation = new QPropertyAnimation(loseLabel,"geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(loseLabel->x(),loseLabel->y(),loseLabel->width(),loseLabel->height()));
        animation->setEndValue(QRect(loseLabel->x(),loseLabel->y()+300,loseLabel->width(),loseLabel->height()));
        animation->setEasingCurve(QEasingCurve::OutBounce);
        animation->start();
    }
    else if(Win_Lose_Flag == 1)//胜利
    {
        QPropertyAnimation * animation = new QPropertyAnimation(winLabel,"geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+300,winLabel->width(),winLabel->height()));
        animation->setEasingCurve(QEasingCurve::OutBounce);
        animation->start();
    }
}

void PlayScene::enemyToScene()//普通敌人登场
{
    //根据敌人冷却器选择是否登场敌人
    enemyrecorder++;
    if(enemyrecorder < ENEMY_INTERVAL)
    {
        return;
    }
    enemyrecorder = 0;

    //敌人登场
    for(int i = 0;i < ENEMY_NUM;i++)
    {
        if(enemystore[i].m_free && !enemystore[i].isdefeated)
        {
            enemystore[i].m_free = false;//登场后该敌人空闲状态改为false
            appeardirection = rand() % 4;//设置随机出现位置(均在周围)
            switch(appeardirection)
            {
            case 0://上
                enemystore[i].m_X = rand() % (GAME_WIDTH - enemystore[i].m_Rect.width());
                enemystore[i].m_Y = -enemystore[i].m_Rect.height();
                enemystore[i].m_Rect.moveTo(enemystore[i].m_X,enemystore[i].m_Y);
                break;
            case 1://下
                enemystore[i].m_X = rand() % (GAME_WIDTH - enemystore[i].m_Rect.width());
                enemystore[i].m_Y = GAME_HEIGHT+enemystore[i].m_Rect.height();
                enemystore[i].m_Rect.moveTo(enemystore[i].m_X,enemystore[i].m_Y);
                break;
            case 2://左
                enemystore[i].m_X = -enemystore[i].m_Rect.width();
                enemystore[i].m_Y = rand() % (qFloor(GAME_HEIGHT) - qFloor(enemystore[i].m_Rect.height()));
                enemystore[i].m_Rect.moveTo(enemystore[i].m_X,enemystore[i].m_Y);
                break;
            case 3://右
                enemystore[i].m_X = GAME_WIDTH + enemystore[i].m_Rect.width();
                enemystore[i].m_Y = rand() % (qFloor(GAME_HEIGHT) - qFloor(enemystore[i].m_Rect.height()));
                enemystore[i].m_Rect.moveTo(enemystore[i].m_X,enemystore[i].m_Y);
                break;
            }
            break;//一次只出一个敌人，因此需要break
        }
    }
}

void PlayScene::eliteenemyTOScene()//精英敌人登场
{
    //根据敌人冷却器选择是否登场敌人
    eliteenemyrecorder++;
    if(eliteenemyrecorder < ELITEENEMY_INTERVAL)
    {
        return;
    }
    eliteenemyrecorder = 0;

    //精英敌人登场
    for(int i = 0;i < ELITEENEMY_NUM;i++)
    {
        if(eliteenemystore[i].m_free && !eliteenemystore[i].isdefeated)
        {
            eliteenemystore[i].m_free = false;//登场后该精英敌人空闲状态改为false
            appeardirection = rand() % 4;//设置随机出现位置(均在周围)
            switch(appeardirection)
            {
            case 0://上
                eliteenemystore[i].m_X = rand() % (GAME_WIDTH - eliteenemystore[i].m_Rect.width());
                eliteenemystore[i].m_Y = -eliteenemystore[i].m_Rect.height();
                eliteenemystore[i].m_Rect.moveTo(eliteenemystore[i].m_X,eliteenemystore[i].m_Y);
                break;
            case 1://下
                eliteenemystore[i].m_X = rand() % (GAME_WIDTH - eliteenemystore[i].m_Rect.width());
                eliteenemystore[i].m_Y = GAME_HEIGHT+eliteenemystore[i].m_Rect.height();
                eliteenemystore[i].m_Rect.moveTo(eliteenemystore[i].m_X,eliteenemystore[i].m_Y);
                break;
            case 2://左
                eliteenemystore[i].m_X = -eliteenemystore[i].m_Rect.width();
                eliteenemystore[i].m_Y = rand() % (qFloor(GAME_HEIGHT) - qFloor(eliteenemystore[i].m_Rect.height()));
                eliteenemystore[i].m_Rect.moveTo(eliteenemystore[i].m_X,eliteenemystore[i].m_Y);
                break;
            case 3://右
                eliteenemystore[i].m_X = GAME_WIDTH + eliteenemystore[i].m_Rect.width();
                eliteenemystore[i].m_Y = rand() % (qFloor(GAME_HEIGHT) - qFloor(eliteenemystore[i].m_Rect.height()));
                eliteenemystore[i].m_Rect.moveTo(eliteenemystore[i].m_X,eliteenemystore[i].m_Y);
                break;
            }
            break;//一次只出一个敌人，因此需要break
        }
    }
}

void PlayScene::bossToScene()//Boss登场
{
    boss.hp = 500;
    boss.m_X = GAME_WIDTH/2 - boss.m_Rect.width()/2;
    boss.m_Y = boss.m_Rect.height();
    boss.m_Rect.moveTo(boss.m_X,boss.m_Y);
}

void PlayScene::boss_generate_defend()//Boss生成护盾
{
    for(int i = 0;i < 30*10;i+=30)
    {
        if(num >= 200+i && num < 210+i && !boss.m_free)
        {
            boss_defend->m_X = boss.m_X+((boss_defend->m_Rect.width()-boss.m_Rect.width())/2)-20;
            boss_defend->m_Y = boss.m_Y+((boss_defend->m_Rect.height()-boss.m_Rect.height())/2)+20;
            boss_defend->m_Rect.moveTo(boss_defend->m_X,boss_defend->m_Y);
        }
        if(num == 210+i)
        {
            boss_defend->m_X = -300;
            boss_defend->m_Y = -300;
            boss_defend->m_Rect.moveTo(boss_defend->m_X,boss_defend->m_Y);
        }
    }
}

void PlayScene::upgrade()//强化
{
    //是否开始打Boss
    if(num == 170)
    {
        //消灭障碍物
        for(int i = 0;i < obstaclestore.size();i++)
        {
            MyObstacle * p = obstaclestore[i];
            delete p;
            p = nullptr;
            obstaclestore.removeAt(i);
        }
        //消灭场上剩余敌人、子弹
        for(int i = 0;i < ENEMY_NUM;i++)
        {
            if(enemystore[i].m_free == false)
            {
                enemystore[i].m_free = true;
                enemystore[i].isdefeated = true;
                enemystore[i].hplabel->hide();
            }
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            for(int j = 0;j < eliteenemystore[i].bulletstore.size();j++)
            {
                eliteenemystore[i].bulletstore[j]->m_Free = true;
            }
            if(eliteenemystore[i].m_free == false)
            {
                eliteenemystore[i].m_free = true;
                eliteenemystore[i].isdefeated = true;
                eliteenemystore[i].hplabel->hide();
            }
        }
        //Boss登场
        bossToScene();
        boss.m_free = false;
    }

    //玩家升级强化

    //初级变中级
    if(isupgrade1 == false && expnum == expmax && m_player.chooseindex != 6)
    {
        expmax+=200;
        m_player.hp+=20;
        hpmax+=20;
        m_player.m_speed+=0.3;
        PLAYER_SPEED+=0.3;
        m_player.m_attack+=1;
        atk+=1;
        for(int i = 0;i < 10;i++)
        {
            if(m_player.bulletinterval > 40) m_player.bulletinterval--;
            else break;
        }
        bombnum+=1;
        isupgrade1 = true;
    }
    //中级变高级
    if(isupgrade2 == false && expnum == expmax && m_player.chooseindex != 6)
    {
        m_player.hp+=20;
        hpmax+=20;
        m_player.m_speed+=0.3;
        PLAYER_SPEED+=0.3;
        m_player.m_attack+=2;
        atk+=2;
        for(int i = 0;i < 10;i++)
        {
            if(m_player.bulletinterval > 35) m_player.bulletinterval--;
            else break;
        }
        bombnum+=1;
        isupgrade2 = true;
    }

    //敌人强化
    if(num == 30) ENEMY_INTERVAL = 175;
    if(num == 60) ENEMY_INTERVAL = 155;
    if(num == 90) ENEMY_INTERVAL = 125;
    if(num == 120) ENEMY_INTERVAL = 100;
    if(num == 150) ENEMY_INTERVAL = 80;
    if(num == 30)
    {
        ELITEENEMY_INTERVAL = 450;
        ENEMY_BULLET_INTERVAL = 450;
    }
    if(num == 60)
    {
        ELITEENEMY_INTERVAL = 400;
        ENEMY_BULLET_INTERVAL = 400;
    }
    if(num == 90)
    {
        ELITEENEMY_INTERVAL = 350;
        ENEMY_BULLET_INTERVAL = 320;
    }
    if(num == 120)
    {
        ELITEENEMY_INTERVAL = 300;
        ENEMY_BULLET_INTERVAL = 250;
    }
    if(num == 150)
    {
        ELITEENEMY_INTERVAL = 250;
        ENEMY_BULLET_INTERVAL = 200;
    }
    if(num == 170)
    {
        ENEMY_INTERVAL = 900;
        ELITEENEMY_INTERVAL = 1200;
        ENEMY_BULLET_INTERVAL = 600;
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(eliteenemystore[i].m_free) continue;
            eliteenemystore[i].m_object.load(ELITEENEMY_RIGHT_PATH2);//加载图片资源
            eliteenemystore[i].m_object = eliteenemystore[i].m_object.scaled(50,50*0.618);//设置大小
            eliteenemystore[i].m_Rect.setWidth(eliteenemystore[i].m_object.width());//初始化矩形框
            eliteenemystore[i].m_Rect.setHeight(eliteenemystore[i].m_object.height());//初始化矩形框
        }
    }
    if(num >= 170 && boss.hp <= 100)
    {
        ENEMY_INTERVAL = 500;
        ELITEENEMY_INTERVAL = 700;
        ENEMY_BULLET_INTERVAL = 350;
    }

    if(num >= 0 && num < 30)
    {
        for(int i = 0; i < ENEMY_NUM;i++)
        {
            if(!enemystore[i].m_free && !enemystore[i].isdefeated) continue;
            enemystore[i].hp = 1;
            enemystore[i].m_speed = 0.3;
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(!eliteenemystore[i].m_free && !eliteenemystore[i].isdefeated) continue;
            eliteenemystore[i].hp = 2;
            eliteenemystore[i].m_speed = 0.35;
        }
    }
    if(num >= 30 && num < 60)
    {
        for(int i = 0; i < ENEMY_NUM;i++)
        {
            if(!enemystore[i].m_free && !enemystore[i].isdefeated) continue;
            enemystore[i].hp = 2;
            enemystore[i].m_speed = 0.3;
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(!eliteenemystore[i].m_free && !eliteenemystore[i].isdefeated) continue;
            eliteenemystore[i].hp = 3;
            eliteenemystore[i].m_speed = 0.35;
        }
    }
    if(num >= 60 && num < 90)
    {
        for(int i = 0; i < ENEMY_NUM;i++)
        {
            if(!enemystore[i].m_free && !enemystore[i].isdefeated) continue;
            enemystore[i].hp = 3;
            enemystore[i].m_speed = 0.4;
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(!eliteenemystore[i].m_free && !eliteenemystore[i].isdefeated) continue;
            eliteenemystore[i].hp = 5;
            eliteenemystore[i].m_speed = 0.4;
        }
    }
    if(num >= 60 && num < 90)
    {
        for(int i = 0; i < ENEMY_NUM;i++)
        {
            if(!enemystore[i].m_free && !enemystore[i].isdefeated) continue;
            enemystore[i].hp = 4;
            enemystore[i].m_speed = 0.45;
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(!eliteenemystore[i].m_free && !eliteenemystore[i].isdefeated) continue;
            eliteenemystore[i].hp = 6;
            eliteenemystore[i].m_speed = 0.45;
        }
    }
    if(num >= 90 && num < 120)
    {
        for(int i = 0; i < ENEMY_NUM;i++)
        {
            if(!enemystore[i].m_free && !enemystore[i].isdefeated) continue;
            enemystore[i].hp = 5;
            enemystore[i].m_speed = 0.5;
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(!eliteenemystore[i].m_free && !eliteenemystore[i].isdefeated) continue;
            eliteenemystore[i].hp = 8;
            eliteenemystore[i].m_speed = 0.5;
        }
    }
    if(num >= 120 && num < 150)
    {
        for(int i = 0; i < ENEMY_NUM;i++)
        {
            if(!enemystore[i].m_free && !enemystore[i].isdefeated) continue;
            enemystore[i].hp = 8;
            enemystore[i].m_speed = 0.5;
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(!eliteenemystore[i].m_free && !eliteenemystore[i].isdefeated) continue;
            eliteenemystore[i].hp = 10;
            eliteenemystore[i].m_speed = 0.5;
        }
    }
    if(num >= 150 && num < 170)
    {
        for(int i = 0; i < ENEMY_NUM;i++)
        {
            if(!enemystore[i].m_free && !enemystore[i].isdefeated) continue;
            enemystore[i].hp = 10;
            enemystore[i].m_speed = 0.6;
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(!eliteenemystore[i].m_free && !eliteenemystore[i].isdefeated) continue;
            eliteenemystore[i].hp = 10;
            eliteenemystore[i].m_speed = 0.6;
        }
    }
    if(num == 170)
    {
        for(int i = 0; i < ENEMY_NUM;i++)
        {
            if(!enemystore[i].m_free && !enemystore[i].isdefeated) continue;
            enemystore[i].hp = 10;
            enemystore[i].m_speed = 0.55;
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(!eliteenemystore[i].m_free && !eliteenemystore[i].isdefeated) continue;
            eliteenemystore[i].hp = 10;
            eliteenemystore[i].m_speed = 0.55;
        }
    }

    //掉落物削减概率
    if(num == 60) dropgooddecrease = 1;
    if(num == 120) dropgooddecrease = 2;
    if(num == 170) dropgooddecrease = 3;
}

void PlayScene::keyPressEvent(QKeyEvent *event)//键盘事件 WASD或者5213移动
{
    //判断如果不是长按时自动触发的按下,就将key值加入容器
    if(!event->isAutoRepeat())  keys.append(event->key());
    //如果定时器不在运行，就启动一下
    if(!keyRespondTimer->isActive()) keyRespondTimer->start(4);
}

void PlayScene::keyReleaseEvent(QKeyEvent * event)//重写键盘释放事件
{
    //判断如果不是长按时自动触发的释放,就将key值从容器中删除
    if(!event->isAutoRepeat())
    {
        switch(event->key())
        {
        case Qt::Key_W: case Qt::Key_5: case Qt::Key_S: case Qt::Key_2:
            verticalkey = 0;
            break;
        case Qt::Key_A: case Qt::Key_1: case Qt::Key_D: case Qt::Key_3:
            horizontalkey = 0;
            break;
        default:
            break;
        }
        keys.removeAll(event->key());
    }
    //容器空了，关闭定时器
    if(keys.isEmpty()) keyRespondTimer->stop();
}

void PlayScene::mykeyevent()//自定义键盘事件解决卡键
{
    for(int i = 0;i < keys.size();i++)
    {
        switch (keys[i])
        {
        case Qt::Key_W: case Qt::Key_5:
            verticalkey = 1;
            break;
        case Qt::Key_S: case Qt::Key_2:
            verticalkey = 2;
            break;
        case Qt::Key_A: case Qt::Key_1:
            horizontalkey = 1;
            break;
        case Qt::Key_D: case Qt::Key_3:
            horizontalkey = 2;
            break;
        case Qt::Key_J:
            bomb();
            keys.removeAll(Qt::Key_J);
            break;
        case Qt::Key_K:
            if(skilltime == skillinterval) skill();
            keys.removeAll(Qt::Key_K);
        default:
            break;
        }
    }
}

void PlayScene::judgekey()//根据按键移动玩家并更新玩家图片
{
    QPainter painter2(this);

    //撞到障碍物情况
    for(int i = 0;i < obstaclestore.size();i++)
    {
        if(m_player.chooseindex == 6) break;
        if(abs(m_player.m_Y + m_player.m_speed + m_player.m_Rect.height() - obstaclestore[i]->m_Y ) <= 1.5
            &&(m_player.m_X >= ((obstaclestore[i]->m_X - m_player.m_Rect.width())))
            &&(m_player.m_X <= ((obstaclestore[i]->m_X + obstaclestore[i]->m_Rect.width()))))
        {
            if(verticalkey == 2)
            {
                painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
                return;
            }
        }
        else if(abs(m_player.m_Y - m_player.m_speed - (obstaclestore[i]->m_Y + obstaclestore[i]->m_Rect.height()) ) <= 1.5
                 &&(m_player.m_X >= ((obstaclestore[i]->m_X - m_player.m_Rect.width())))
                 &&(m_player.m_X <= ((obstaclestore[i]->m_X + obstaclestore[i]->m_Rect.width()))))
        {
            if(verticalkey == 1)
            {
                painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
                return;
            }
        }
        else if(abs(m_player.m_X + m_player.m_speed + m_player.m_Rect.width() - obstaclestore[i]->m_X ) <= 1.5
                 &&(m_player.m_Y >= ((obstaclestore[i]->m_Y - m_player.m_Rect.height())))
                 &&(m_player.m_Y <= ((obstaclestore[i]->m_Y + obstaclestore[i]->m_Rect.height()))))
        {
            if(horizontalkey == 2)
            {
                painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
                return;
            }
        }
        else if(abs(m_player.m_X - m_player.m_speed - (obstaclestore[i]->m_X + obstaclestore[i]->m_Rect.width()) ) <= 1.5
                 &&(m_player.m_Y >= ((obstaclestore[i]->m_Y - m_player.m_Rect.height())))
                 &&(m_player.m_Y <= ((obstaclestore[i]->m_Y + obstaclestore[i]->m_Rect.height()))))
        {
            if(horizontalkey == 1)
            {
                painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
                return;
            }
        }
    }

    //未碰到障碍物情况或者撞到障碍物但是移动方向仍可行情况
    switch(verticalkey)
    {
    case 0:
        switch(horizontalkey)
        {
        case 0:
            m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
            painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
            return;
        case 1://左
            m_player.m_X = (m_player.m_X - 5 < 0) ? m_player.m_X : m_player.m_X - PLAYER_SPEED;
            m_player.load_player(3);
            m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
            painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
            return;
        case 2://右
            m_player.m_X = (m_player.m_X + 5 + m_player.m_object.width() > GAME_WIDTH) ? m_player.m_X : m_player.m_X + PLAYER_SPEED;
            m_player.load_player(4);
            m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
            painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
            return;
        }
    case 1:
        switch(horizontalkey)
        {
        case 0://上
            m_player.m_Y = (m_player.m_Y - 5 < 0) ? m_player.m_Y : m_player.m_Y - PLAYER_SPEED;
            m_player.load_player(1);
            m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
            painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
            return;
        case 1://左上
            m_player.m_X = (m_player.m_X - 5 < 0) ? m_player.m_X : m_player.m_X - PLAYER_HYPOTENUSE_SPEED;
            m_player.m_Y = (m_player.m_Y - 5 < 0) ? m_player.m_Y : m_player.m_Y - PLAYER_HYPOTENUSE_SPEED;
            m_player.load_player(3);
            m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
            painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
            return;
        case 2://右上
            m_player.m_X = (m_player.m_X + 5 + m_player.m_object.width() > GAME_WIDTH) ? m_player.m_X : m_player.m_X + PLAYER_HYPOTENUSE_SPEED;
            m_player.m_Y = (m_player.m_Y - 5 < 0) ? m_player.m_Y : m_player.m_Y - PLAYER_HYPOTENUSE_SPEED;
            m_player.load_player(4);
            m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
            painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
            return;
        }
    case 2:
        switch(horizontalkey)
        {
        case 0://下
            m_player.m_Y = (m_player.m_Y + 5 + m_player.m_object.height() > GAME_HEIGHT) ? m_player.m_Y : m_player.m_Y + PLAYER_SPEED;
            m_player.load_player(2);
            m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
            painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
            return;
        case 1://左下
            m_player.m_X = (m_player.m_X - 5 < 0) ? m_player.m_X : m_player.m_X - PLAYER_HYPOTENUSE_SPEED;
            m_player.m_Y = (m_player.m_Y + 5 + m_player.m_object.height() > GAME_HEIGHT) ? m_player.m_Y : m_player.m_Y + PLAYER_HYPOTENUSE_SPEED;
            m_player.load_player(3);
            m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
            painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
            return;
        case 2://右下
            m_player.m_X = (m_player.m_X + 5 + m_player.m_object.width() > GAME_WIDTH) ? m_player.m_X : m_player.m_X + PLAYER_HYPOTENUSE_SPEED;
            m_player.m_Y = (m_player.m_Y + 5 + m_player.m_object.height() > GAME_HEIGHT) ? m_player.m_Y : m_player.m_Y + PLAYER_HYPOTENUSE_SPEED;
            m_player.load_player(4);
            m_player.m_Rect.moveTo(m_player.m_X,m_player.m_Y);
            painter2.drawPixmap(m_player.m_X,m_player.m_Y,m_player.m_object);
            return;
        }
    default:
        return;
    }
}

void PlayScene::bomb()//使用炸弹
{
    if(bombnum == 0) return;
    else
    {
        bombnum--;
        for(int i = 0;i < ENEMY_NUM;i++)
        {
            if(enemystore[i].m_free) continue;
            if(sqrt((m_player.m_X - enemystore[i].m_X) * (m_player.m_X - enemystore[i].m_X)
              +(m_player.m_Y - enemystore[i].m_Y) * (m_player.m_Y - enemystore[i].m_Y))
              <= 600)
            {
                for(int i = 0;i <= expincreasspeed;i++)
                {
                    if(expnum<expmax)expnum++;
                }
                for(int k = 0 ; k < BOMB_NUM;k++)
                {
                    if(m_bombs[k].m_Free)
                    {
                        //爆炸状态设置为非空闲
                        m_bombs[k].m_Free = false;
                        //更新坐标
                        m_bombs[k].m_X = enemystore[i].m_X;
                        m_bombs[k].m_Y = enemystore[i].m_Y;
                        break;
                    }
                }
                enemystore[i].m_free = true;
                enemystore[i].isdefeated = true;
                enemystore[i].hplabel->hide();
                dropgood(enemystore[i].m_X,enemystore[i].m_Y);
            }
        }
        for(int i = 0;i < ELITEENEMY_NUM;i++)
        {
            if(eliteenemystore[i].m_free) continue;
            if(sqrt((m_player.m_X - eliteenemystore[i].m_X) * (m_player.m_X - eliteenemystore[i].m_X)
                     +(m_player.m_Y - eliteenemystore[i].m_Y) * (m_player.m_Y - eliteenemystore[i].m_Y))
                <= 600)
            {
                for(int i = 0;i <= expincreasspeed;i++)
                {
                    if(expnum<expmax)expnum++;
                    if(expnum<expmax)expnum++;
                }
                for(int k = 0 ; k < BOMB_NUM;k++)
                {
                    if(m_bombs[k].m_Free)
                    {
                        //爆炸状态设置为非空闲
                        m_bombs[k].m_Free = false;
                        //更新坐标
                        m_bombs[k].m_X = eliteenemystore[i].m_X;
                        m_bombs[k].m_Y = eliteenemystore[i].m_Y;
                        break;
                    }
                }
                eliteenemystore[i].m_free = true;
                eliteenemystore[i].isdefeated = true;
                eliteenemystore[i].hplabel->hide();
                dropgood(eliteenemystore[i].m_X,eliteenemystore[i].m_Y);
            }
        }
    }
}

void PlayScene::skill()//使用技能
{
    skilltime = 0;
    switch(m_player.get_chooseindex())
    {
    case 1:
        suckhptime = 0;
        break;
    case 2:
        shoottime = 0;
        for(int i = 0;i < 2;i++)
        {
            if(m_player.bulletinterval > 30)
            {
                m_player.bulletinterval -= 15;
            }
        }
        break;
    case 3:
        coinnum+=100;
        break;
    case 4:
        unbeatabletime = 0;
        break;
    case 5:
        attacktime = 0;
        break;
    case 6:
        coinnum+=999;
        break;
    default:
        break;
    }
}

void PlayScene::updatePosition()//更新坐标
{
    //普通敌人位置
    for(int i = 0 ; i < ENEMY_NUM;i++)
    {
        //非空闲敌人 更新坐标
        if(enemystore[i].m_free == false)
        {
            enemystore[i].updatePosition(m_player.m_X,m_player.m_Y,obstaclestore);
        }
    }

    //精英敌人位置
    for(int i = 0 ; i < ELITEENEMY_NUM;i++)
    {
        //非空闲敌人 更新坐标
        if(eliteenemystore[i].m_free == false)
        {
            eliteenemystore[i].updatePosition(m_player.m_X,m_player.m_Y,obstaclestore,num);
        }
    }

    //Boss位置
    if(boss.m_free == false) boss.updatePosition(m_player.m_X,m_player.m_Y);

    //玩家子弹
    m_player.shoot(verticalkey,horizontalkey);
    for(int i = 0;i < m_player.bulletstore.size();i++)
    {
        m_player.bulletstore[i]->updatePosition();
    }

    if(m_player.get_chooseindex() == 1)//近战子弹一旦离人物较远就析构
    {
        for(int i = 0;i < m_player.bulletstore.size();i++)
        {
            if(sqrt((m_player.bulletstore[i]->m_X - m_player.m_X - m_player.m_Rect.width()*0.5 - 18)
              *(m_player.bulletstore[i]->m_X - m_player.m_X - m_player.m_Rect.width()*0.5 + 18)
              +(m_player.bulletstore[i]->m_Y - m_player.m_Y - m_player.m_Rect.height()*0.5 + 25)
              *(m_player.bulletstore[i]->m_Y - m_player.m_Y - m_player.m_Rect.height()*0.5 + 25))
               >= 200)
            m_player.bulletstore[i]->m_Free = true;
        }
    }

    //精英敌人子弹
    for(int i = 0;i < ELITEENEMY_NUM;i++)
    {
        if(!eliteenemystore[i].m_free)
        {
            double dx = m_player.m_X+m_player.m_Rect.width()/2 - (eliteenemystore[i].m_X+eliteenemystore[i].m_Rect.width()/2);
            double dy = m_player.m_Y+m_player.m_Rect.height()/2 - (eliteenemystore[i].m_Y+eliteenemystore[i].m_Rect.height()/2);
            double extend_x = (m_player.m_X+m_player.m_Rect.width()/2 + GAME_WIDTH * dx);
            double extend_y = (m_player.m_Y+m_player.m_Rect.height()/2 + GAME_WIDTH * dy);
            eliteenemystore[i].shoot(extend_x,extend_y,ENEMY_BULLET_INTERVAL,num);
        }
        for(int j = 0;j < eliteenemystore[i].bulletstore.size();j++)
        {
            eliteenemystore[i].bulletstore[j]->eliteenemyupdatePosition(num);
        }
    }

    //Boss子弹
    if(!boss.m_free)
    {
        int randkind = rand()%5+1;
        boss.shoot(m_player.m_X,m_player.m_Y,randkind);
    }
    for(int i = 0;i < boss.bulletstore.size();i++)
    {
        boss.bulletstore[i]->bossbulletupdatePosition(m_player.m_X,m_player.m_Y);
    }

    //爆炸特效位置
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            m_bombs[i].updateInfo();
        }
    }
}

void PlayScene::collisionDetection()//碰撞判断
{
    bool isbreak = false;//停止标志(防止一些极端情况)

    //玩家子弹撞到障碍物
    if(m_player.chooseindex != 5 && m_player.get_chooseindex() != 6)
    {
        for(int i = 0;i < m_player.bulletstore.size();i++)
        {
            for(int j = 0;j < obstaclestore.size();j++)
            {
                if(m_player.bulletstore[i]->m_Rect.intersects(obstaclestore[j]->m_Rect))
                {
                    m_player.bulletstore[i]->m_Free = true;//子弹失效
                    if(m_player.chooseindex == 4)//爆炸弹碰到障碍物爆炸
                    {
                        for(int k = 0;k < ENEMY_NUM; k++)
                        {
                            if(enemystore[k].m_free) continue;
                            if(abs(enemystore[k].m_X - m_player.bulletstore[i]->m_X) <= 200
                                && abs(enemystore[k].m_Y - m_player.bulletstore[i]->m_Y) <= 200)
                            {
                                enemystore[k].hp -= atk;//敌人扣血
                                if(enemystore[k].hp <= 0)//敌人没血则被消灭
                                {
                                    for(int i = 0;i <= expincreasspeed;i++)
                                    {
                                        if(expnum<expmax)expnum++;
                                    }
                                    enemystore[k].m_free = true;
                                    enemystore[k].isdefeated = true;
                                    enemystore[k].hplabel->hide();
                                    dropgood(enemystore[k].m_X,enemystore[k].m_Y);
                                }
                            }
                        }
                        for(int k = 0;k < ELITEENEMY_NUM; k++)
                        {
                            if(eliteenemystore[k].m_free) continue;
                            if(abs(eliteenemystore[k].m_X - m_player.bulletstore[i]->m_X) <= 200
                                && abs(eliteenemystore[k].m_Y - m_player.bulletstore[i]->m_Y) <= 200)
                            {
                                eliteenemystore[k].hp -= atk;//敌人扣血
                                if(eliteenemystore[k].hp <= 0)//敌人没血则被消灭
                                {
                                    for(int i = 0;i <= expincreasspeed;i++)
                                    {
                                        if(expnum<expmax)expnum++;
                                        if(expnum<expmax)expnum++;
                                    }
                                    eliteenemystore[k].m_free = true;
                                    eliteenemystore[k].isdefeated = true;
                                    eliteenemystore[k].hplabel->hide();
                                    dropgood(enemystore[k].m_X,enemystore[k].m_Y);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //敌人子弹碰到障碍物
    for(int i = 0;i < ELITEENEMY_NUM;i++)
    {
        if(eliteenemystore[i].m_free) continue;
        for(int j = 0;j < obstaclestore.size();j++)
        {
            for(int k = 0;k < eliteenemystore[i].bulletstore.size();k++)
            {
                if(eliteenemystore[i].bulletstore[k]->m_Rect.intersects(obstaclestore[j]->m_Rect))
                {
                    eliteenemystore[i].bulletstore[k]->m_Free = true;
                }
            }
        }
    }

    //普通敌人
    for(int i = 0 ;i < ENEMY_NUM;i++)
    {
        isbreak = false;
        if(enemystore[i].m_free) continue;//空闲敌人 跳转下一次循环

        //敌人血量标签
        enemystore[i].enemy_hp_str = QString("  血量:%1").arg(enemystore[i].hp);
        enemystore[i].hplabel->setText(enemystore[i].enemy_hp_str);
        enemystore[i].hplabel->move(enemystore[i].m_X,enemystore[i].m_Y + enemystore[i].m_Rect.height()+1);

        //玩家碰撞普通敌人
        if(m_player.m_Rect.intersects(enemystore[i].m_Rect))
        {
            if(unbeatabletime >= 10) m_player.hp--;//玩家扣血
            //敌人被消灭
            for(int i = 0;i <= expincreasspeed;i++)
            {
                if(expnum<expmax)expnum++;
            }
            enemystore[i].m_free = true;
            enemystore[i].isdefeated = true;
            enemystore[i].hplabel->hide();
            dropgood(enemystore[i].m_X,enemystore[i].m_Y);
            continue;
        }

        //玩家子弹碰撞普通敌人
        if(m_player.get_chooseindex() == 1 || m_player.get_chooseindex() == 2 || m_player.get_chooseindex() == 3)
        {
            for(int j = 0 ; j < m_player.bulletstore.size();j++)
            {
                if(m_player.bulletstore[j]->m_Free || isbreak == true) continue;//空闲子弹 跳转下一次循环
                if(enemystore[i].m_Rect.intersects(m_player.bulletstore[j]->m_Rect))
                {
                    m_player.bulletstore[j]->m_Free = true;//子弹失效
                    if(suckhptime < 5 && m_player.hp < hpmax) m_player.hp++;//玩家吸血
                    enemystore[i].hp -= atk;//敌人扣血
                    if(enemystore[i].hp <= 0)//敌人没血则被消灭
                    {
                        for(int k = 0 ; k < BOMB_NUM;k++)
                        {
                            if(m_bombs[k].m_Free)
                            {
                                //爆炸状态设置为非空闲
                                m_bombs[k].m_Free = false;
                                //更新坐标
                                m_bombs[k].m_X = enemystore[i].m_X;
                                m_bombs[k].m_Y = enemystore[i].m_Y;
                                break;
                            }
                        }
                        for(int i = 0;i <= expincreasspeed;i++)
                        {
                            if(expnum<expmax)expnum++;
                        }
                        enemystore[i].m_free = true;
                        enemystore[i].isdefeated = true;
                        enemystore[i].hplabel->hide();
                        dropgood(enemystore[i].m_X,enemystore[i].m_Y);
                        isbreak = true;
                    }
                }
            }
        }
        else if(m_player.get_chooseindex() == 4)//爆炸子弹
        {
            for(int j = 0 ; j < m_player.bulletstore.size();j++)
            {
                if(m_player.bulletstore[j]->m_Free || isbreak == true) continue;//空闲子弹 跳转下一次循环
                if(enemystore[i].m_Rect.intersects(m_player.bulletstore[j]->m_Rect))
                {
                    m_player.bulletstore[j]->m_Free = true;//子弹失效
                    for(int k = 0;k < ENEMY_NUM; k++)
                    {
                        if(enemystore[k].m_free) continue;
                        if(abs(enemystore[k].m_X - m_player.bulletstore[j]->m_X) <= 200
                        && abs(enemystore[k].m_Y - m_player.bulletstore[j]->m_Y) <= 200)
                        {
                            enemystore[k].hp -= atk;//敌人扣血
                            if(enemystore[k].hp <= 0)//敌人没血则被消灭
                            {
                                for(int l = 0 ; l < BOMB_NUM;l++)
                                {
                                    if(m_bombs[l].m_Free)
                                    {
                                        //爆炸状态设置为非空闲
                                        m_bombs[l].m_Free = false;
                                        //更新坐标
                                        m_bombs[l].m_X = enemystore[k].m_X;
                                        m_bombs[l].m_Y = enemystore[k].m_Y;
                                        break;
                                    }
                                }
                                for(int i = 0;i <= expincreasspeed;i++)
                                {
                                    if(expnum<expmax)expnum++;
                                }
                                enemystore[k].m_free = true;
                                enemystore[k].isdefeated = true;
                                enemystore[k].hplabel->hide();
                                dropgood(enemystore[k].m_X,enemystore[k].m_Y);
                                isbreak = true;
                            }
                        }
                    }
                    for(int k = 0;k < ELITEENEMY_NUM; k++)
                    {
                        if(eliteenemystore[k].m_free) continue;
                        if(abs(eliteenemystore[k].m_X - m_player.bulletstore[j]->m_X) <= 200
                            && abs(eliteenemystore[k].m_Y - m_player.bulletstore[j]->m_Y) <= 200)
                        {
                            eliteenemystore[k].hp -= atk;//敌人扣血
                            if(eliteenemystore[k].hp <= 0)//敌人没血则被消灭
                            {
                                for(int l = 0 ; l < BOMB_NUM;l++)
                                {
                                    if(m_bombs[l].m_Free)
                                    {
                                        //爆炸状态设置为非空闲
                                        m_bombs[l].m_Free = false;
                                        //更新坐标
                                        m_bombs[l].m_X = eliteenemystore[k].m_X;
                                        m_bombs[l].m_Y = eliteenemystore[k].m_Y;
                                        break;
                                    }
                                }
                                for(int i = 0;i <= expincreasspeed;i++)
                                {
                                    if(expnum<expmax)expnum++;
                                    if(expnum<expmax)expnum++;
                                }

                                eliteenemystore[k].m_free = true;
                                eliteenemystore[k].isdefeated = true;
                                eliteenemystore[k].hplabel->hide();
                                dropgood(enemystore[k].m_X,enemystore[k].m_Y);
                                isbreak = true;
                            }
                        }
                    }
                }
            }
        }
        else if(m_player.get_chooseindex() == 5)//穿透弹
        {
            for(int j = 0 ; j < m_player.bulletstore.size();j++)
            {
                if(m_player.bulletstore[j]->m_Free || isbreak == true) continue;//空闲子弹 跳转下一次循环
                if(enemystore[i].m_Rect.intersects(m_player.bulletstore[j]->m_Rect))
                {
                    if(!m_player.bulletstore[j]->isthrough)
                    {
                        if(attacktime <= 5)//技能:强化伤害
                        {
                            enemystore[i].hp -= 10;//敌人扣血
                            m_player.bulletstore[j]->isthrough = 1;//是否被穿透弹击中
                        }
                        else
                        {
                            enemystore[i].hp -= atk;//敌人扣血
                            m_player.bulletstore[j]->isthrough = 1;//是否被穿透弹击中
                        }
                    }
                    if(enemystore[i].hp <= 0)//敌人没血则被消灭
                    {
                        for(int k = 0 ; k < BOMB_NUM;k++)
                        {
                            if(m_bombs[k].m_Free)
                            {
                                //爆炸状态设置为非空闲
                                m_bombs[k].m_Free = false;
                                //更新坐标
                                m_bombs[k].m_X = enemystore[i].m_X;
                                m_bombs[k].m_Y = enemystore[i].m_Y;
                                break;
                            }
                        }
                        for(int i = 0;i <= expincreasspeed;i++)
                        {
                            if(expnum<expmax)expnum++;
                        }
                        enemystore[i].m_free = true;
                        enemystore[i].isdefeated = true;
                        enemystore[i].hplabel->hide();
                        dropgood(enemystore[i].m_X,enemystore[i].m_Y);
                        isbreak = true;
                    }
                }
            }
        }
        else if(m_player.get_chooseindex() == 6)//爆炸加穿透子弹
        {
            for(int j = 0 ; j < m_player.bulletstore.size();j++)
            {
                if(m_player.bulletstore[j]->m_Free || isbreak == true) continue;//空闲子弹 跳转下一次循环
                if(enemystore[i].m_Rect.intersects(m_player.bulletstore[j]->m_Rect))
                {
                    for(int k = 0;k < ENEMY_NUM; k++)
                    {
                        if(enemystore[k].m_free) continue;
                        if(abs(enemystore[k].m_X - m_player.bulletstore[j]->m_X) <= 200
                            && abs(enemystore[k].m_Y - m_player.bulletstore[j]->m_Y) <= 200)
                        {
                            enemystore[k].hp -= atk;//敌人扣血
                            if(enemystore[k].hp <= 0)//敌人没血则被消灭
                            {
                                for(int l = 0 ; l < BOMB_NUM;l++)
                                {
                                    if(m_bombs[l].m_Free)
                                    {
                                        //爆炸状态设置为非空闲
                                        m_bombs[l].m_Free = false;
                                        //更新坐标
                                        m_bombs[l].m_X = enemystore[k].m_X;
                                        m_bombs[l].m_Y = enemystore[k].m_Y;
                                        break;
                                    }
                                }
                                for(int i = 0;i <= expincreasspeed;i++)
                                {
                                    if(expnum<expmax)expnum++;
                                }
                                enemystore[k].m_free = true;
                                enemystore[k].isdefeated = true;
                                enemystore[k].hplabel->hide();
                                dropgood(enemystore[k].m_X,enemystore[k].m_Y);
                                isbreak = true;
                            }
                        }
                    }
                    for(int k = 0;k < ELITEENEMY_NUM; k++)
                    {
                        if(eliteenemystore[k].m_free) continue;
                        if(abs(eliteenemystore[k].m_X - m_player.bulletstore[j]->m_X) <= 200
                            && abs(eliteenemystore[k].m_Y - m_player.bulletstore[j]->m_Y) <= 200)
                        {
                            eliteenemystore[k].hp -= atk;//敌人扣血
                            if(eliteenemystore[k].hp <= 0)//敌人没血则被消灭
                            {
                                for(int l = 0 ; l < BOMB_NUM;l++)
                                {
                                    if(m_bombs[l].m_Free)
                                    {
                                        //爆炸状态设置为非空闲
                                        m_bombs[l].m_Free = false;
                                        //更新坐标
                                        m_bombs[l].m_X = eliteenemystore[k].m_X;
                                        m_bombs[l].m_Y = eliteenemystore[k].m_Y;
                                        break;
                                    }
                                }
                                for(int i = 0;i <= expincreasspeed;i++)
                                {
                                    if(expnum<expmax)expnum++;
                                    if(expnum<expmax)expnum++;
                                }
                                eliteenemystore[k].m_free = true;
                                eliteenemystore[k].isdefeated = true;
                                eliteenemystore[k].hplabel->hide();
                                dropgood(eliteenemystore[k].m_X,eliteenemystore[k].m_Y);
                                isbreak = true;
                            }
                        }
                    }
                }
            }
        } 
    }

    //精英敌人
    for(int i = 0;i < ELITEENEMY_NUM;i++)
    {
        isbreak = false;
        if(!eliteenemystore[i].m_free)//还存在着的精英敌人
        {
            //精英敌人血量标签
            eliteenemystore[i].enemy_hp_str = QString("  血量:%1").arg(eliteenemystore[i].hp);
            eliteenemystore[i].hplabel->setText(eliteenemystore[i].enemy_hp_str);
            eliteenemystore[i].hplabel->move(eliteenemystore[i].m_X,eliteenemystore[i].m_Y + enemystore[i].m_Rect.height()+1);

            //玩家碰撞精英敌人
            if(m_player.m_Rect.intersects(eliteenemystore[i].m_Rect))
            {
                if(unbeatabletime >= 10) m_player.hp--;//玩家扣血
                //精英敌人被消灭
                for(int i = 0;i <= expincreasspeed;i++)
                {
                    if(expnum<expmax)expnum++;
                    if(expnum<expmax)expnum++;
                }
                eliteenemystore[i].m_free = true;
                eliteenemystore[i].isdefeated = true;
                eliteenemystore[i].hplabel->hide();
                dropgood(eliteenemystore[i].m_X,eliteenemystore[i].m_Y);
            }

            //玩家子弹碰撞精英敌人
            if(m_player.get_chooseindex() == 1 || m_player.get_chooseindex() == 2 || m_player.get_chooseindex() == 3)
            {
                for(int j = 0 ; j < m_player.bulletstore.size();j++)
                {
                    if(m_player.bulletstore[j]->m_Free || isbreak == true) continue;//空闲子弹 跳转下一次循环
                    if(eliteenemystore[i].m_Rect.intersects(m_player.bulletstore[j]->m_Rect))
                    {
                        m_player.bulletstore[j]->m_Free = true;//子弹失效
                        if(suckhptime < 5 && m_player.hp < hpmax) m_player.hp++;
                        eliteenemystore[i].hp -= atk;//精英敌人扣血
                        if(eliteenemystore[i].hp <= 0)//精英敌人没血则被消灭
                        {
                            for(int k = 0 ; k < BOMB_NUM;k++)
                            {
                                if(m_bombs[k].m_Free)
                                {
                                    //爆炸状态设置为非空闲
                                    m_bombs[k].m_Free = false;
                                    //更新坐标
                                    m_bombs[k].m_X = eliteenemystore[i].m_X;
                                    m_bombs[k].m_Y = eliteenemystore[i].m_Y;
                                    break;
                                }
                            }
                            for(int i = 0;i <= expincreasspeed;i++)
                            {
                                if(expnum<expmax)expnum++;
                                if(expnum<expmax)expnum++;
                            }
                            eliteenemystore[i].m_free = true;
                            eliteenemystore[i].isdefeated = true;
                            eliteenemystore[i].hplabel->hide();
                            dropgood(eliteenemystore[i].m_X,eliteenemystore[i].m_Y);
                        }
                    }
                }
            }
            else if(m_player.get_chooseindex() == 4)//爆炸子弹
            {
                for(int j = 0 ; j < m_player.bulletstore.size();j++)
                {
                    if(m_player.bulletstore[j]->m_Free || isbreak == true) continue;//空闲子弹 跳转下一次循环
                    if(eliteenemystore[i].m_Rect.intersects(m_player.bulletstore[j]->m_Rect))
                    {
                        m_player.bulletstore[j]->m_Free = true;//子弹失效
                        for(int k = 0;k < ENEMY_NUM; k++)
                        {
                            if(enemystore[k].m_free) continue;
                            if(abs(enemystore[k].m_X - m_player.bulletstore[j]->m_X) <= 200
                                && abs(enemystore[k].m_Y - m_player.bulletstore[j]->m_Y) <= 200)
                            {
                                enemystore[k].hp -= atk;//敌人扣血
                                if(enemystore[k].hp <= 0)//敌人没血则被消灭
                                {
                                    for(int l = 0 ; l < BOMB_NUM;l++)
                                    {
                                        if(m_bombs[l].m_Free)
                                        {
                                            //爆炸状态设置为非空闲
                                            m_bombs[l].m_Free = false;
                                            //更新坐标
                                            m_bombs[l].m_X = enemystore[k].m_X;
                                            m_bombs[l].m_Y = enemystore[k].m_Y;
                                            break;
                                        }
                                    }
                                    for(int i = 0;i <= expincreasspeed;i++)
                                    {
                                        if(expnum<expmax)expnum++;
                                    }
                                    enemystore[k].m_free = true;
                                    enemystore[k].isdefeated = true;
                                    enemystore[k].hplabel->hide();
                                    dropgood(eliteenemystore[k].m_X,eliteenemystore[k].m_Y);
                                    isbreak = true;
                                }
                            }
                        }
                        for(int k = 0;k < ELITEENEMY_NUM; k++)
                        {
                            if(eliteenemystore[k].m_free) continue;
                            if(abs(eliteenemystore[k].m_X - m_player.bulletstore[j]->m_X) <= 200
                                && abs(eliteenemystore[k].m_Y - m_player.bulletstore[j]->m_Y) <= 200)
                            {
                                eliteenemystore[k].hp -= atk;//敌人扣血
                                if(eliteenemystore[k].hp <= 0)//敌人没血则被消灭
                                {
                                    for(int l = 0 ; l < BOMB_NUM;l++)
                                    {
                                        if(m_bombs[l].m_Free)
                                        {
                                            //爆炸状态设置为非空闲
                                            m_bombs[l].m_Free = false;
                                            //更新坐标
                                            m_bombs[l].m_X = eliteenemystore[k].m_X;
                                            m_bombs[l].m_Y = eliteenemystore[k].m_Y;
                                            break;
                                        }
                                    }
                                    for(int i = 0;i <= expincreasspeed;i++)
                                    {
                                        if(expnum<expmax)expnum++;
                                        if(expnum<expmax)expnum++;
                                    }
                                    eliteenemystore[k].m_free = true;
                                    eliteenemystore[k].isdefeated = true;
                                    eliteenemystore[k].hplabel->hide();
                                    dropgood(eliteenemystore[k].m_X,eliteenemystore[k].m_Y);
                                    isbreak = true;
                                }
                            }
                        }
                    }
                }
            }
            else if(m_player.get_chooseindex() == 5)//穿透弹
            {
                for(int j = 0 ; j < m_player.bulletstore.size();j++)
                {
                    if(m_player.bulletstore[j]->m_Free || isbreak == true) continue;//空闲子弹 跳转下一次循环
                    if(eliteenemystore[i].m_Rect.intersects(m_player.bulletstore[j]->m_Rect))
                    {
                        if(!m_player.bulletstore[j]->isthrough)
                        {
                            if(attacktime <= 5)//技能:强化伤害
                            {
                                eliteenemystore[i].hp -= 10;//敌人扣血
                                m_player.bulletstore[j]->isthrough = 1;//是否被穿透弹击中
                            }
                            else
                            {
                                eliteenemystore[i].hp -= atk;//敌人扣血
                                m_player.bulletstore[j]->isthrough = 1;//是否被穿透弹击中
                            }
                        }
                        if(eliteenemystore[i].hp <= 0)//敌人没血则被消灭
                        {
                            for(int k = 0 ; k < BOMB_NUM;k++)
                            {
                                if(m_bombs[k].m_Free)
                                {
                                    //爆炸状态设置为非空闲
                                    m_bombs[k].m_Free = false;
                                    //更新坐标
                                    m_bombs[k].m_X = eliteenemystore[i].m_X;
                                    m_bombs[k].m_Y = eliteenemystore[i].m_Y;
                                    break;
                                }
                            }
                            for(int i = 0;i <= expincreasspeed;i++)
                            {
                                if(expnum<expmax)expnum++;
                                if(expnum<expmax)expnum++;
                            }
                            eliteenemystore[i].m_free = true;
                            eliteenemystore[i].isdefeated = true;
                            eliteenemystore[i].hplabel->hide();
                            dropgood(eliteenemystore[i].m_X,eliteenemystore[i].m_Y);
                            isbreak = true;
                        }
                    }
                }
            }
            else if(m_player.get_chooseindex() == 6)//爆炸加穿透子弹
            {
                for(int j = 0 ; j < m_player.bulletstore.size();j++)
                {
                    if(m_player.bulletstore[j]->m_Free || isbreak == true) continue;//空闲子弹 跳转下一次循环
                    if(eliteenemystore[i].m_Rect.intersects(m_player.bulletstore[j]->m_Rect))
                    {
                        for(int k = 0;k < ENEMY_NUM; k++)
                        {
                            if(enemystore[k].m_free) continue;
                            if(abs(enemystore[k].m_X - m_player.bulletstore[j]->m_X) <= 200
                                && abs(enemystore[k].m_Y - m_player.bulletstore[j]->m_Y) <= 200)
                            {
                                enemystore[k].hp -= atk;//敌人扣血
                                if(enemystore[k].hp <= 0)//敌人没血则被消灭
                                {
                                    for(int l = 0 ; l < BOMB_NUM;l++)
                                    {
                                        if(m_bombs[l].m_Free)
                                        {
                                            //爆炸状态设置为非空闲
                                            m_bombs[l].m_Free = false;
                                            //更新坐标
                                            m_bombs[l].m_X = enemystore[k].m_X;
                                            m_bombs[l].m_Y = enemystore[k].m_Y;
                                            break;
                                        }
                                    }
                                    for(int i = 0;i <= expincreasspeed;i++)
                                    {
                                        if(expnum<expmax)expnum++;
                                    }
                                    enemystore[k].m_free = true;
                                    enemystore[k].isdefeated = true;
                                    enemystore[k].hplabel->hide();
                                    dropgood(enemystore[k].m_X,enemystore[k].m_Y);
                                    isbreak = true;
                                }
                            }
                        }
                        for(int k = 0;k < ELITEENEMY_NUM; k++)
                        {
                            if(eliteenemystore[k].m_free) continue;
                            if(abs(eliteenemystore[k].m_X - m_player.bulletstore[j]->m_X) <= 200
                                && abs(eliteenemystore[k].m_Y - m_player.bulletstore[j]->m_Y) <= 200)
                            {
                                eliteenemystore[k].hp -= atk;//敌人扣血
                                if(eliteenemystore[k].hp <= 0)//敌人没血则被消灭
                                {
                                    for(int l = 0 ; l < BOMB_NUM;l++)
                                    {
                                        if(m_bombs[l].m_Free)
                                        {
                                            //爆炸状态设置为非空闲
                                            m_bombs[l].m_Free = false;
                                            //更新坐标
                                            m_bombs[l].m_X = eliteenemystore[k].m_X;
                                            m_bombs[l].m_Y = eliteenemystore[k].m_Y;
                                            break;
                                        }
                                    }
                                    for(int i = 0;i <= expincreasspeed;i++)
                                    {
                                        if(expnum<expmax)expnum++;
                                        if(expnum<expmax)expnum++;
                                    }
                                    eliteenemystore[k].m_free = true;
                                    eliteenemystore[k].isdefeated = true;
                                    eliteenemystore[k].hplabel->hide();
                                    dropgood(eliteenemystore[k].m_X,eliteenemystore[k].m_Y);
                                    isbreak = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        //精英敌人子弹碰撞玩家(若敌人已经被消灭，子弹仍能攻击玩家)
        for(int j = 0 ; j < eliteenemystore[i].bulletstore.size();j++)
        {
            if(eliteenemystore[i].bulletstore[j]->m_Free || isbreak == true) continue;//空闲子弹 跳转下一次循环
            if(m_player.m_Rect.intersects(eliteenemystore[i].bulletstore[j]->m_Rect))
            {
                eliteenemystore[i].bulletstore[j]->m_Free = true;//子弹失效
                isbreak = true;
                if(unbeatabletime >= 10) m_player.hp--;//玩家扣血
            }
        }
    }

    //Boss碰撞判断
    Bosscollision();

    //掉落物
    for(int i = 0;i < goodstore.size();i++)
    {
        //玩家6自动拾取掉落物
        if((m_player.chooseindex == 6 && num % 3 == 0) || m_player.m_Rect.intersects(goodstore[i]->m_Rect))
        {
            switch(goodstore[i]->goodindex)
            {
            case 1://加血道具
                for(int j = 0;j < 10;j++)
                {
                    if(m_player.hp == hpmax) break;
                    m_player.hp++;
                }
                break;
            case 2://金币
                if(m_player.chooseindex!=6)coinnum+=50;
                break;
            case 3://人物速度
                m_player.m_speed = m_player.m_speed < 2 ? m_player.m_speed + 0.05 : m_player.m_speed;
                PLAYER_SPEED = PLAYER_SPEED < 2 ? PLAYER_SPEED + 0.05 : PLAYER_SPEED;
                break;
            case 4://人物攻击力
                m_player.m_attack = m_player.m_attack < 10 ? (m_player.m_attack + 1) : m_player.m_attack;
                atk = atk < 10 ? (atk + 1) : atk;
                break;
            case 5://人物子弹冷却间隔
                m_player.bulletinterval = m_player.bulletinterval > 50 ? (m_player.bulletinterval - 10) : m_player.bulletinterval;
                break;
            case 6://炸弹
                bombnum++;
                break;
            case 7://无敌五角星
                unbeatabletime = 0;
                break;
            case 8://经验
                for(int i = 0;i < 30;i++)
                {
                    if(expnum<expmax)expnum++;
                    else break;
                }
                break;
            default:
                break;
            }
            MyGood *p = goodstore[i];
            delete p;
            p = nullptr;
            goodstore.removeAt(i);
        }
    }
}

void PlayScene::Bosscollision()//Boss碰撞判断
{
    //Boss血量标签
    boss.enemy_hp_str = QString("  血量:%1").arg(boss.hp);
    boss.hplabel->setText(boss.enemy_hp_str);
    boss.hplabel->move(boss.m_X,boss.m_Y + boss.m_Rect.height()+1);

    //Boss碰撞玩家
    if(boss.m_Rect.intersects(m_player.m_Rect))
    {
        if(m_player.chooseindex != 6)
        {
            if(unbeatabletime >= 10)
            {
                if(isbosscollideplayer == false)
                {
                    isbosscollideplayer = true;
                    bosscollisionplayertime = 0;
                    numstore = num;
                }
                if(bosscollisionplayertime == 0)
                {
                    bosscollisionplayertime++;
                    m_player.hp -= 10;
                }
            }
        }
    }

    //Boss子弹碰撞玩家
    for(int i = 0;i < boss.bulletstore.size();i++)
    {
        if(m_player.chooseindex == 6) break;
        if(boss.bulletstore[i]->m_Free) continue;
        if(boss.bulletstore[i]->m_Rect.intersects(m_player.m_Rect))
        {
            boss.bulletstore[i]->m_Free = true;
            if(unbeatabletime >= 10)//玩家扣血
            {
                if(boss.bulletstore[i]->bulletkind == 1) m_player.hp--;
                else if(boss.bulletstore[i]->bulletkind == 2) m_player.hp-=2;
                else if(boss.bulletstore[i]->bulletkind == 3)
                {
                    m_player.hp--;
                    if(m_player.m_speed > 1.2)
                    {
                        m_player.m_speed-=0.05;
                        PLAYER_SPEED-=0.05;
                    }
                }
                else if(boss.bulletstore[i]->bulletkind == 4) m_player.hp--;
                else if(boss.bulletstore[i]->bulletkind == 5) m_player.hp-=10;
            }
        }
    }

    //玩家子弹碰撞Boss
    bool isbossunbeatable = false;
    for(int i = 0 ; i< m_player.bulletstore.size();i++)
    {
        if(boss.m_free) break;
        isbossunbeatable = false;
        if(m_player.bulletstore[i]->m_Free) continue;//空闲子弹 跳转下一次循环
        if(boss.m_Rect.intersects(m_player.bulletstore[i]->m_Rect))
        {
            if(m_player.chooseindex != 5 && m_player.chooseindex != 6) m_player.bulletstore[i]->m_Free = true;//子弹失效
            if(suckhptime < 5 && m_player.hp < hpmax) m_player.hp++;
            for(int j = 0;j < 30*10;j+=30)//护盾期间Boss无敌
            {
                if(num >= 200+j && num < 210+j)
                {
                    isbossunbeatable = true;
                }
            }
            if(isbossunbeatable == true) continue;
            for(int i = 0;i < 3;i++)
            {
                if(atk > i && totalattack < 10)//Boss扣血,最多一个子弹掉3滴血,且做多一下掉10滴血
                {
                    boss.hp--;
                    totalattack++;
                }
            }
            if(boss.hp <= 0)//敌人没血则被消灭
            {
                for(int i = 0;i <= 100;i++)
                {
                    if(expnum<expmax)expnum++;
                }
                boss.m_X = -300;
                boss.m_Y = -300;
                boss.m_Rect.moveTo(boss.m_X,boss.m_Y);
                boss.m_free = true;
                boss.hplabel->hide();
            }
        }
        if(totalattack == 10) break;
    }
}

void PlayScene::dropgood(int x,int y)
{
    int randdrop = 0;
    if(dropgooddecrease == 0) randdrop = rand()%100;
    else if(dropgooddecrease == 1) randdrop = rand()%200;
    else if(dropgooddecrease == 2) randdrop = rand()%300;
    else if(dropgooddecrease == 3) randdrop = rand()%400;
    if(randdrop >= 10) return;
    if(coindropprobability != 0)
    {
        int randcoin = rand()%100 + 1;
        if(randcoin <= 100*coindropprobability)
        {
            MyGood * m_good = new MyGood(2,x,y);
            goodstore.append(m_good);
            return;
        }
    }
    int randdrop2 = rand()%8 + 1;
    if(dropgooddecrease >= 20)//一分钟后减少炸弹和无敌掉落概率
    {
        if(randdrop == 6 || randdrop == 7)
        randdrop2 = rand()%8 + 1;
    }
    while(randdrop2 == 2) randdrop2 = rand()%8 + 1;
    MyGood * m_good = new MyGood(randdrop2,x,y);
    goodstore.append(m_good);
}

void PlayScene::paintEvent(QPaintEvent *event)//绘图事件
{
    //画游戏背景
    QPainter painter(this);
    painter.drawPixmap(0,0,GAME_WIDTH,GAME_HEIGHT,pix);

    //画障碍物
    QPainter painter2(this);
    for(int i = 0;i < obstaclestore.size();i++)
    {
        painter.drawPixmap(obstaclestore[i]->m_X,obstaclestore[i]->m_Y,obstaclestore[i]->m_object);
    }

    //画玩家
    judgekey();

    //画玩家血量条
    QBrush red_brush(QColor(255,106,106));//把刷子设置为红色
    QPainter painter_hp(this);
    painter_hp.setBrush(red_brush);//应用刷子
    float rate = 1.0*m_player.hp/hpmax;//计算比例
    painter_hp.drawRect(m_player.m_X,m_player.m_Y-20,rate*m_player.m_Rect.width()+extendhp,m_player.m_Rect.height()*0.2);//绘制矩形
    QPainter painter_hp2(this);
    painter_hp2.drawRect(m_player.m_X,m_player.m_Y-20,m_player.m_Rect.width()+extendhp,m_player.m_Rect.height()*0.2);//绘制矩形

    //画玩家子弹
    QPainter painter3(this);
    for(int i = 0;i < m_player.bulletstore.size();i++)
    {
        if(m_player.bulletstore[i]->m_Free)//失效子弹随时析构
        {
            Bullet * p = m_player.bulletstore[i];
            delete p;
            p = nullptr;
            m_player.bulletstore.removeAt(i);
            continue;
        }
        painter3.drawPixmap(m_player.bulletstore[i]->m_X,m_player.bulletstore[i]->m_Y,m_player.bulletstore[i]->m_Bullet);
    }

    //画普通敌人
    QPainter painter4(this);
    for(int i = 0 ; i < ENEMY_NUM;i++)
    {
        if(enemystore[i].m_free == false)
        {
            painter4.drawPixmap(enemystore[i].m_X,enemystore[i].m_Y,enemystore[i].m_object); 
        }
    }

    //画精英敌人
    QPainter painter5(this);
    for(int i = 0 ; i < ELITEENEMY_NUM;i++)
    {
        if(eliteenemystore[i].m_free == false)
        {
            painter5.drawPixmap(eliteenemystore[i].m_X,eliteenemystore[i].m_Y,eliteenemystore[i].m_object);
        }
    }

    //画精英敌人子弹
    QPainter painter6(this);
    for(int i = 0;i < ELITEENEMY_NUM;i++)
    {
        for(int j = 0;j < eliteenemystore[i].bulletstore.size();j++)
        {
            if(eliteenemystore[i].bulletstore[j]->m_Free)//失效子弹随时析构
            {
                Bullet * p = eliteenemystore[i].bulletstore[j];
                delete p;
                p = nullptr;
                eliteenemystore[i].bulletstore.removeAt(j);
                continue;
            }
            painter6.drawPixmap(eliteenemystore[i].bulletstore[j]->m_X,eliteenemystore[i].bulletstore[j]->m_Y,eliteenemystore[i].bulletstore[j]->m_Bullet);
        }
    }

    //画掉落物
    QPainter painter7(this);
    for(int i = 0;i < goodstore.size();i++)
    {
        painter7.drawPixmap(goodstore[i]->m_X,goodstore[i]->m_Y,goodstore[i]->m_object);
    }

    //画无敌
    if(unbeatabletime < 10)
    {
        QPainter painter8(this);
        painter8.drawPixmap(m_player.m_X,m_player.m_Y - 50,unbeatable_object);
    }

    //画使用技能状态
    QPainter painter9(this);
    if(m_player.get_chooseindex() == 1 || m_player.get_chooseindex() == 5)
    {
        if(skilltime < 5)
        {
            painter9.drawPixmap(m_player.m_X + m_player.m_Rect.width()/2+13,m_player.m_Y - 50,skill_object);
        }
    }
    else if(m_player.get_chooseindex() == 2)
    {
        if(skilltime < 10)
        {
            painter9.drawPixmap(m_player.m_X + m_player.m_Rect.width()/2+13,m_player.m_Y - 50,skill_object);
        }
    }

    //画Boss
    QPainter painter10(this);
    if(boss.m_free == false)
    {
        painter10.drawPixmap(boss.m_X,boss.m_Y,boss.m_object);
    }

    //画Boss子弹
    QPainter painter11(this);
    for(int i = 0;i < boss.bulletstore.size();i++)
    {
        if(boss.bulletstore[i]->m_Free)//失效子弹随时析构
        {
            Bullet * p = boss.bulletstore[i];
            delete p;
            p = nullptr;
            boss.bulletstore.removeAt(i);
            continue;
        }
        painter11.drawPixmap(boss.bulletstore[i]->m_X,boss.bulletstore[i]->m_Y,boss.bulletstore[i]->m_Bullet);
    }

    //画Boss护盾
    QPainter painter12(this);
    painter12.drawPixmap(boss_defend->m_X,boss_defend->m_Y,boss_defend->m_object);

    //画爆炸特效
    QPainter painter13(this);
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            painter13.drawPixmap(m_bombs[i].m_X,m_bombs[i].m_Y,m_bombs[i].m_pixArr[m_bombs[i].m_index]);
        }
    }
}
