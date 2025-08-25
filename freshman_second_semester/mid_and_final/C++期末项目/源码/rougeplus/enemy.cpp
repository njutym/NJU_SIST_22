#include "enemy.h"

#include "myconfig.h"

Enemy::Enemy()//构造函数
{
    m_object.load(ENEMY_PATH);//加载图片资源
    m_object = m_object.scaled(50,50*0.618);//设置图片大小
    m_free = true;//设置存在状态
    isdefeated = false;//设置存在状态
    m_Rect.setWidth(m_object.width());//初始化矩形框
    m_Rect.setHeight(m_object.height());//初始化矩形框
    hplabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);//设置血量标签对齐方式
    hplabel->setFixedSize(80,50);//设置血量标签大小
    //初始化坐标
    m_X = -300;
    m_Y = -300;
}

void Enemy::round()//绕障碍物走
{
    if(rounddirection == 1) m_Y -= m_speed;
    else if(rounddirection == 2) m_Y += m_speed;
    else if(rounddirection == 3) m_X -= m_speed;
    else if(rounddirection == 4) m_X += m_speed;
    m_Rect.moveTo(m_X,m_Y);
}


void Enemy::updatePosition(double playerx,double playery,QList<MyObstacle*>obstaclestore)//更新敌人位置
{
    if(m_free)//状态free直接return
    {
        return;
    }
    //优化追偿策略，避免生硬
    double x = xrand + playerx;
    double y = yrand + playery;

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

    //未碰到障碍物情况
    //限制位置绝对值差大于等于1才操作，避免敌人与玩家垂直或者水平时高频抖动
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
        if(m_X < x && abs(m_X - x) >= 1) m_X += m_speed/sqrt(2);
        else if(m_X >= x && abs(m_X - x) >= 1) m_X -= m_speed/sqrt(2);
        if(m_Y < y && abs(m_Y - y) >= 1) m_Y += m_speed/sqrt(2);
        else if(m_Y >= y && abs(m_Y - y) >= 1) m_Y -= m_speed/sqrt(2);
    }
    m_Rect.moveTo(m_X,m_Y);
}


