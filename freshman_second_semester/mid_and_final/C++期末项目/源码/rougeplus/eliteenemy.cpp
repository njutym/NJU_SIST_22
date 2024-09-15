#include "eliteenemy.h"

#include <ctime>

#include "myconfig.h"

EliteEnemy::EliteEnemy()//构造函数
{
    m_timer.setInterval(3000);
    startTimer(3000);
    connect(&m_timer,&QTimer::timeout,this,[=]()
    {
        update();
    });
    srand((unsigned int)time(NULL));//时间种子随机数
    m_object.load(ELITEENEMY_LEFT_PATH);//加载图片资源
    m_object = m_object.scaled(50,50*0.618);//设置大小
    m_free = true;//设置初始状态
    isdefeated = false;//设置初始状态
    m_Rect.setWidth(m_object.width());//初始化矩形框
    m_Rect.setHeight(m_object.height());//初始化矩形框
    hplabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);//设置血量标签对齐方式
    hplabel->setFixedSize(80,50);//设置血量标签大小
    //初始化坐标
    m_X = -300;
    m_Y = -300;
}

void EliteEnemy::round()//绕障碍物走
{
    if(rounddirection == 1) m_Y -= m_speed;
    else if(rounddirection == 2) m_Y += m_speed;
    else if(rounddirection == 3) m_X -= m_speed;
    else if(rounddirection == 4) m_X += m_speed;
    m_Rect.moveTo(m_X,m_Y);
}

void EliteEnemy::updatePosition(double playerx,double playery,QList<MyObstacle*>obstaclestore,int num)//更新精英敌人位置
{
    if(m_free)//状态free直接return
    {
        return;
    }

    if(num < 170)//打Boss前精英怪
    {
        double x = xrand + playerx;
        double y = yrand + playery;
        //移动策略，精英敌人左右图片不同，需要更新
        //限制位置绝对值差大于等于1才操作，避免敌人与玩家垂直或者水平时高频抖动

        //碰到障碍物情况
        if(isrounding)
        {
            if(rounddirection == 4)
            {
                if(m_X >= destination)
                {
                    rounddirection = 0;
                    isrounding = false;
                }
            }
            else if(rounddirection == 3)
            {
                if(m_X <= destination)
                {
                    rounddirection = 0;
                    isrounding = false;
                }
            }
            else if(rounddirection == 2)
            {
                if(m_Y >= destination)
                {
                    rounddirection = 0;
                    isrounding = false;
                }
            }
            else if(rounddirection == 1)
            {
                if(m_Y <= destination)
                {
                    rounddirection = 0;
                    isrounding = false;
                }
            }
            round();
            return;
        }

        for(int i = 0;i < obstaclestore.size();i++)
        {
            if(((abs(m_Y + m_speed + m_Rect.height() - obstaclestore[i]->m_Y ) <= 3)
                 ||(abs(m_Y - m_speed - (obstaclestore[i]->m_Y + obstaclestore[i]->m_Rect.height()) ) <= 3))
                &&(m_X >= (obstaclestore[i]->m_X - m_Rect.width()-3))
                &&(m_X <= (obstaclestore[i]->m_X + obstaclestore[i]->m_Rect.width()+3)))
            {
                if(m_X <= x)
                {
                    if(isrounding == false)
                    {
                        rounddirection = 4;
                        isrounding = true;
                        destination = obstaclestore[i]->m_X + obstaclestore[i]->m_Rect.width()+4;
                        return;
                    }
                }
                else
                {
                    if(isrounding == false)
                    {
                        rounddirection = 3;
                        isrounding = true;
                        destination = obstaclestore[i]->m_X - m_Rect.width()-4;
                        return;
                    }
                }
            }

            else if(((abs(m_X + m_speed + m_Rect.width() - obstaclestore[i]->m_X ) <= 3)
                      ||(abs(m_X - m_speed - (obstaclestore[i]->m_X + obstaclestore[i]->m_Rect.width()) ) <= 3))
                     &&(m_Y >= ((obstaclestore[i]->m_Y - m_Rect.height())-3))
                     &&(m_Y <= ((obstaclestore[i]->m_Y + obstaclestore[i]->m_Rect.height())+3)))
            {
                if(m_Y <= y)
                {
                    if(isrounding == false)
                    {
                        rounddirection = 2;
                        isrounding = true;
                        destination = (obstaclestore[i]->m_Y + obstaclestore[i]->m_Rect.height())+4;
                        return;
                    }
                }
                else
                {
                    if(isrounding == false)
                    {
                        rounddirection = 1;
                        isrounding = true;
                        destination = (obstaclestore[i]->m_Y - m_Rect.height())-4;
                        return;
                    }
                }
            }
        }

        if(sqrt((m_X - x) * (m_X - x) + (m_Y - y) * (m_Y - y)) <= 300) return;

        if(abs(m_X - x) < 1 && abs(m_Y - y) >= 1)
        {
            if(m_Y >= y) m_Y -= m_speed;
            else m_Y += m_speed;
        }
        else if(abs(m_X - x) >= 1 && abs(m_Y - y) < 1)
        {
            if(m_X >= x) m_X -= m_speed;
            else m_X += m_speed;
        }
        else
        {
            if(m_X < x && abs(m_X - x) >= 1)
            {
                m_X += m_speed/sqrt(2);
                m_object.load(ELITEENEMY_RIGHT_PATH);
            }
            else if(m_X >= x && abs(m_X - x) >= 1)
            {
                m_X -= m_speed/sqrt(2);
                m_object.load(ELITEENEMY_LEFT_PATH);
            }
            if(m_Y < y && abs(m_Y - y) >= 1) m_Y += m_speed/sqrt(2);
            else if(m_Y >= y && abs(m_Y - y) >= 1) m_Y -= m_speed/sqrt(2);
        }
        m_Rect.moveTo(m_X,m_Y);
    }

    else//打Boss时精英怪
    {
        if(m_X >= playerx) m_object.load(ELITEENEMY_LEFT_PATH2);
        else if(m_X < playerx) m_object.load(ELITEENEMY_RIGHT_PATH2);
        if(randdirection == 1 && m_X > 50) m_X -= m_speed;
        else if(randdirection == 2 && m_X < GAME_WIDTH - 50) m_X += m_speed;
        else if(randdirection == 3 && m_Y > 50) m_Y -= m_speed;
        else if(randdirection == 4 && m_Y < GAME_HEIGHT - 50) m_Y += m_speed;
        m_Rect.moveTo(m_X,m_Y);
    }
}

void EliteEnemy::shoot(double x,double y,int interval,int num)//精英敌人发射子弹
{
    m_recorder++;
    //判断如果记录数字 未达到发射间隔，直接return
    if(m_recorder < interval) return;

    if(num < 170)//打Boss之前精英怪
    {
        Bullet *m_bullet = new Bullet(x,y);//申明子弹并加入bulletstore
        bulletstore.append(m_bullet);
        if(m_bullet->m_Free)
        {
            m_bullet->m_Free = false;//将改子弹空闲状态改为假
            //设置发射的子弹坐标
            m_bullet->m_X = m_X + m_Rect.width()*0.5;
            m_bullet->m_Y = m_Y + m_Rect.height()*0.5;
            m_bullet->m_Rect.moveTo(m_bullet->m_X,m_bullet->m_Y);
        }
    }

    else//打boss时精英怪
    {
        for(int i = 0;i <= 2;i++)
        {
            for(int j = 0;j <= 2;j++)
            {
                if(!i && !j) continue;
                Bullet *m_bullet2 = new Bullet(i,j);//申明子弹并加入bulletstore
                bulletstore.append(m_bullet2);
                if(m_bullet2->m_Free)
                {
                    m_bullet2->m_Free = false;//将改子弹空闲状态改为假
                    //设置发射的子弹坐标
                    m_bullet2->m_X = m_X + m_Rect.width()*0.5;
                    m_bullet2->m_Y = m_Y + m_Rect.height()*0.5;
                    m_bullet2->m_Rect.moveTo(m_bullet2->m_X,m_bullet2->m_Y);
                }
            }
        }
        for(int i = 3;i <= 4;i++)
        {
            for(int j = 0;j <= 3;j++)
            {
                Bullet *m_bullet3 = new Bullet(i,j);//申明子弹并加入bulletstore
                bulletstore.append(m_bullet3);
                if(m_bullet3->m_Free)
                {
                    m_bullet3->m_Free = false;//将改子弹空闲状态改为假
                    //设置发射的子弹坐标
                    m_bullet3->m_X = m_X + m_Rect.width()*0.5;
                    m_bullet3->m_Y = m_Y + m_Rect.height()*0.5;
                    m_bullet3->m_Rect.moveTo(m_bullet3->m_X,m_bullet3->m_Y);
                }
            }
        }
    }

    m_recorder = 0;    //重置发射时间间隔记录  
}

void EliteEnemy::timerEvent(QTimerEvent*)//计时器事件
{
   randdirection = rand()%5+1;
}

