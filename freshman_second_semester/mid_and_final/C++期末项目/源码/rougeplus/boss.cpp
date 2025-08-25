#include "boss.h"

#include <ctime>

#include "myconfig.h"

Boss::Boss()
{
    srand((unsigned int)time(NULL));//时间种子随机数
    m_object.load(":/resource/boss.png");//加载图片资源
    m_object = m_object.scaled(81,132);//设置图片大小
    m_free = true;//设置存在状态
    m_Rect.setWidth(m_object.width());//初始化矩形框
    m_Rect.setHeight(m_object.height());//初始化矩形框
    hplabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);//设置血量标签对齐方式
    hplabel->setFixedSize(80,50);//设置血量标签大小
    //初始化坐标
    m_X = -800;
    m_Y = -800;
    hp = 999999;
}

void Boss::updatePosition(double x,double y)//更新Boss位置
{
    if(m_free)//状态free直接return
    {
        return;
    }

    //距离玩家一定距离则停止
    if(sqrt((m_X - x) * (m_X - x) + (m_Y - y) * (m_Y - y)) <= 500) return;

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

void Boss::getangle(double x,double y)//获取向玩家角度
{
    double dx = abs(x - m_X);
    double dy = abs(y - m_Y);
    if(x != m_X && y != m_Y)
    {
        if(m_X < x)
        {
            if(m_Y < y) angle = qCeil(-atan(dy/dx)*180/M_PI);
            else if(m_Y > y) angle = qCeil(atan(dy/dx)*180/M_PI);
        }
        else
        {
            if(m_Y < y) angle = qCeil((M_PI + atan(dy/dx))*180/M_PI);
            else if(m_Y > y) angle = qCeil((M_PI - atan(dy/dx))*180/M_PI);
        }
    }
    else if(y == m_Y && x != m_X)
    {
        if(x > m_X) angle = 0;
        else if(x <= m_X) angle = 180;
    }
    else if(x == m_X && y != m_Y)
    {
        if(y > m_Y) angle = (0.5*180);
        else if(y <= m_Y) angle = (1.5*180);
    }
    else if(x == m_X && y == m_Y)
    {
        angle = 0;
    }
}

void Boss::shoot(double x,double y,int kind)//Boss发射子弹
{
    m_recorder++;
    //判断如果记录数字 未达到发射间隔，直接return
    if(m_recorder < interval) return;
    switch(kind)
    {
    case 1://向四周乱射
        for(int i = 0;i < 60;i++)
        {
            Bullet *m_bullet = new Bullet(x,y,1,rand()%360,m_X + m_Rect.width()*0.5,m_Rect.height()*0.5);//申明子弹并加入bulletstore
            bulletout(m_bullet,1);
            bulletstore.append(m_bullet);
        }
        m_recorder = 0;
        break;
    case 2://大范围扫射
        getangle(x,y);
        angle -= 40;
        for(int i = 0;i < 20;i++)
        {
            Bullet *m_bullet = new Bullet(x,y,2,angle+i*4,m_X + m_Rect.width()*0.5,m_Rect.height()*0.5);//申明子弹并加入bulletstore
            bulletout(m_bullet,2);
            bulletstore.append(m_bullet);
        }
        m_recorder = 0;
        break;
    case 3://追踪弹,同时可以减少玩家速度
        for(int i = 0;i < 3;i++)
        {
            Bullet *m_bullet = new Bullet(x,y,3,angle,m_X + m_Rect.width()*0.1 + randbullet3_1,m_Y + m_Rect.height()*0.1 + randbullet3_2);//申明子弹并加入bulletstore
            bulletout(m_bullet,3);
            bulletstore.append(m_bullet);
        }
        m_recorder = 0;
        break;
    case 4://随机乱射子弹
        center_x = rand()%(GAME_WIDTH-300) + 150;
        center_y = rand()%qFloor(GAME_HEIGHT-300) + 150;
        while(sqrt((center_x - x)*(center_x - x) + (center_y - y)*(center_y - y)) <= 200)
        {
            center_x = rand()%(GAME_WIDTH-300) + 150;
            center_y = rand()%qFloor(GAME_HEIGHT-300) + 150;
        }
        for(int i = 0;i < 75;i++)
        {
            Bullet *m_bullet = new Bullet(x,y,4,rand()%360,m_X + m_Rect.width()*0.5,m_Y + m_Rect.height()*0.5);//申明子弹并加入bulletstore
            bulletout(m_bullet,4);
            bulletstore.append(m_bullet);
        }
        m_recorder = 0;
        break;
    case 5://大型子弹
        getangle(x,y);
        for(int i = 0;i < 1;i++)
        {
            Bullet *m_bullet = new Bullet(x,y,5,angle,m_X + m_Rect.width()*0.5,m_Rect.height()*0.5);//申明子弹并加入bulletstore
            bulletout(m_bullet,5);
            bulletstore.append(m_bullet);
        }
        m_recorder = 0;
        break;
    default:
        break;
    }
}

void Boss::bulletout(Bullet *m_bullet,int kind)//子弹发出处理
{
    if(m_bullet->m_Free)
    {
        m_bullet->m_Free = false;//将改子弹空闲状态改为假
        //设置发射的子弹坐标
        if(kind == 4)
        {
            m_bullet->m_X = center_x + rand()%200 - 100;
            m_bullet->m_Y = center_y + rand()%200 - 100;
        }
        else if(kind != 3)
        {
            m_bullet->m_X = m_X + m_Rect.width()*0.5;
            m_bullet->m_Y = m_Y + m_Rect.height()*0.5;
        }
        else
        {
            randbullet3_1 = rand()%80;
            randbullet3_2 = rand()%80;
            m_bullet->m_X = m_X + m_Rect.width()*0.1 + randbullet3_1;
            m_bullet->m_Y = m_Y + m_Rect.height()*0.1 + randbullet3_2;
        }
        m_bullet->m_Rect.moveTo(m_bullet->m_X,m_bullet->m_Y);
    }
}
