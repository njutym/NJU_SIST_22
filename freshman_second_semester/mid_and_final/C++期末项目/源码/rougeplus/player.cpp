#include "player.h"

Player::Player()
{
    //初始化玩家资源
    load_player(4);
    m_Rect.setWidth(m_object.width());
    m_Rect.setHeight(m_object.height());
    m_Rect.moveTo(m_X,m_Y);
}

void Player::loadchooseindex(int Chooseindex,int HP,double speed,int attack,int Bulletinterval,double Bulletspeed)//加载玩家选择参数
{
    chooseindex = Chooseindex;
    hp = HP;
    m_speed = speed;
    m_attack = attack;
    bulletinterval = Bulletinterval;
    bulletspeed = Bulletspeed;
}

int Player::get_chooseindex()//给外界获得选择玩家参数
{
    return chooseindex;
}

void Player::load_player(int flag)//更新玩家图片资源
{
    if(chooseindex == 2 || chooseindex == 3)
    {
        switch(flag)
        {
        case 1://上
            m_object.load(QString(":/resource/player%1up.png").arg(chooseindex));
            m_object = m_object.scaled(88,88);
            face = 1;
            return;
        case 2://下
            m_object.load(QString(":/resource/player%1down.png").arg(chooseindex));
            m_object = m_object.scaled(88,88);
            face = 2;
            return;
        case 3://左
            m_object.load(QString(":/resource/player%1left.png").arg(chooseindex));
            m_object = m_object.scaled(88,88);
            face = 3;
            return;
        case 4://右
            m_object.load(QString(":/resource/player%1right.png").arg(chooseindex));
            m_object = m_object.scaled(88,88);
            face = 4;
            return;
        }
    }
    else if(chooseindex == 1 || chooseindex == 4 || chooseindex == 5)
    {
        switch(flag)
        {
        case 1://上
            face = 1;
            return;
        case 2://下
            face = 2;
            return;
        case 3://左
            m_object.load(QString(":/resource/player%1left.png").arg(chooseindex));
            m_object = m_object.scaled(88,88);
            face = 3;
            return;
        case 4://右
            m_object.load(QString(":/resource/player%1right.png").arg(chooseindex));
            m_object = m_object.scaled(88,88);
            face = 4;
            return;
        default:
            return;
        }
    }
    else if(chooseindex == 6)
    {
        switch(flag)
        {
        case 1://上
            face = 1;
            return;
        case 2://下
            face = 2;
            return;
        case 3://左
            face = 3;
            return;
        case 4://右
            m_object.load(":/resource/player6.jpg");
            m_object = m_object.scaled(qCeil(0.2*893),qCeil(0.2*674));
            face = 4;
            return;
        default:
            return;
        }
    }
}

void Player::shoot(int v_key,int h_key)//玩家发射子弹
{
    if(chooseindex == 1) shoot1();
    else if(chooseindex == 2) shoot2(v_key,h_key);
    else if(chooseindex == 3) shoot3();
    else if(chooseindex == 4) shoot4(v_key,h_key);
    else if(chooseindex == 5) shoot5(v_key,h_key);
    else if(chooseindex == 6) shoot6(v_key,h_key);
}

void Player::shoot1()
{
    m_recorder++;//判断如果记录数字 未达到发射间隔，直接return
    if(m_recorder < bulletinterval) return;

    //到达发射时间处理
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            if(!i && !j) continue;
            Bullet *m_bullet = new Bullet(i,j,chooseindex,bulletspeed);
            bulletstore.append(m_bullet);
            bulletout(m_bullet);
        }
    }
    for(int i = 0;i < 4;i++)
    {
        Bullet *m_bullet = new Bullet(3,i,chooseindex,bulletspeed);
        bulletstore.append(m_bullet);
        bulletout(m_bullet);
    }
    for(int i = 0;i < 4;i++)
    {
        Bullet *m_bullet = new Bullet(4,i,chooseindex,bulletspeed);
        bulletstore.append(m_bullet);
        bulletout(m_bullet);
    }

    //重置发射时间间隔记录
    m_recorder = 0;
}

void Player::shoot2(int v_key,int h_key)//玩家2发射子弹
{
    m_recorder++;//判断如果记录数字 未达到发射间隔，直接return
    if(m_recorder < bulletinterval) return;

    if(v_key != 0 || h_key != 0)
    {
        Bullet *m_bullet = new Bullet(v_key,h_key,chooseindex,bulletspeed);//到达发射时间处理
        bulletstore.append(m_bullet);
        bulletout(m_bullet);
    }

    if(v_key == 0)
    {
        if(h_key == 0)//停止
        {
            m_recorder++;//判断如果记录数字 未达到发射间隔，直接return
            if(m_recorder < 2*bulletinterval) return;//静止则以较慢速度向周围发子弹
            for(int i = 0;i < 3;i++)
            {
                for(int j = 0;j < 3;j++)
                {
                    if(!i && !j) continue;
                    Bullet *m_bullet = new Bullet(i,j,chooseindex,bulletspeed);
                    bulletstore.append(m_bullet);
                    bulletout(m_bullet);
                }
            }
        }
        else if(h_key == 1)//左
        {
            Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 2)//右
        {
            Bullet *m_bullet2 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
    }

    else if(v_key == 1)
    {
        if(h_key == 0)//上
        {
            Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(3,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 1)//左上
        {
            Bullet *m_bullet2 = new Bullet(0,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(1,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(3,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 2)//右上
        {
            Bullet *m_bullet2 = new Bullet(0,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(1,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(3,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
    }

    else if(v_key == 2)
    {
        if(h_key == 0)//下
        {
            Bullet *m_bullet2 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(4,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 1)//左下
        {
            Bullet *m_bullet2 = new Bullet(0,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(4,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 2)//右下
        {
            Bullet *m_bullet2 = new Bullet(0,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(4,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
    }

    //重置发射时间间隔记录
    m_recorder = 0;
}

void Player::shoot3()//玩家3发射子弹
{
    shoot1();
}

void Player::shoot4(int v_key,int h_key)//玩家4发射子弹
{
    m_recorder++;//判断如果记录数字 未达到发射间隔，直接return
    if(m_recorder < bulletinterval) return;

    //到达发射时间处理
    if(v_key != 0 || h_key != 0)
    {
        Bullet *m_bullet = new Bullet(v_key,h_key,chooseindex,bulletspeed);
        bulletstore.append(m_bullet);
        bulletout(m_bullet);
    }
    else
    {
        if(face == 1)
        {
            Bullet *m_bullet = new Bullet(1,0,chooseindex,bulletspeed);
            bulletstore.append(m_bullet);
            bulletout(m_bullet);
        }
        else if(face == 2)
        {
            Bullet *m_bullet = new Bullet(2,0,chooseindex,bulletspeed);
            bulletstore.append(m_bullet);
            bulletout(m_bullet);
        }
        else if(face == 3)
        {
            Bullet *m_bullet = new Bullet(0,1,chooseindex,bulletspeed);
            bulletstore.append(m_bullet);
            bulletout(m_bullet);
        }
        else if(face == 4)
        {
            Bullet *m_bullet = new Bullet(0,2,chooseindex,bulletspeed);
            bulletstore.append(m_bullet);
            bulletout(m_bullet);
        }
    }

    //重置冷却器
    m_recorder = 0;
}

void Player::shoot5(int v_key,int h_key)//玩家5发射子弹
{
    m_recorder++;//判断如果记录数字 未达到发射间隔，直接return
    if(m_recorder < bulletinterval) return;

    if(v_key || h_key)
    {
        Bullet *m_bullet = new Bullet(v_key,h_key,chooseindex,bulletspeed);//到达发射时间处理
        bulletstore.append(m_bullet);
        bulletout(m_bullet);
    }

    if(v_key == 0)
    {
        if(h_key == 0)//停止
        {
            if(face == 1)
            {
                Bullet *m_bullet = new Bullet(1,0,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet);
                bulletout(m_bullet);
                Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet2);
                bulletout(m_bullet2);
                Bullet *m_bullet3 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet3);
                bulletout(m_bullet3);
                Bullet *m_bullet4 = new Bullet(3,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet4);
                bulletout(m_bullet4);
                Bullet *m_bullet5 = new Bullet(3,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet5);
                bulletout(m_bullet5);
            }
            else if(face == 2)
            {
                Bullet *m_bullet = new Bullet(2,0,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet);
                bulletout(m_bullet);
                Bullet *m_bullet2 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet2);
                bulletout(m_bullet2);
                Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet3);
                bulletout(m_bullet3);
                Bullet *m_bullet4 = new Bullet(4,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet4);
                bulletout(m_bullet4);
                Bullet *m_bullet5 = new Bullet(4,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet5);
                bulletout(m_bullet5);
            }
            else if(face == 3)
            {
                Bullet *m_bullet = new Bullet(0,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet);
                bulletout(m_bullet);
                Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet2);
                bulletout(m_bullet2);
                Bullet *m_bullet3 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet3);
                bulletout(m_bullet3);
                Bullet *m_bullet4 = new Bullet(3,3,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet4);
                bulletout(m_bullet4);
                Bullet *m_bullet5 = new Bullet(4,0,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet5);
                bulletout(m_bullet5);
            }
            else if(face == 4)
            {
                Bullet *m_bullet = new Bullet(0,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet);
                bulletout(m_bullet);
                Bullet *m_bullet2 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet2);
                bulletout(m_bullet2);
                Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet3);
                bulletout(m_bullet3);
                Bullet *m_bullet4 = new Bullet(3,0,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet4);
                bulletout(m_bullet4);
                Bullet *m_bullet5 = new Bullet(4,3,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet5);
                bulletout(m_bullet5);
            }
        }
        else if(h_key == 1)//左
        {
            Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 2)//右
        {
            Bullet *m_bullet2 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
    }

    else if(v_key == 1)
    {
        if(h_key == 0)//上
        {
            Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(3,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 1)//左上
        {
            Bullet *m_bullet2 = new Bullet(0,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(1,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(3,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 2)//右上
        {
            Bullet *m_bullet2 = new Bullet(0,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(1,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(3,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
    }

    else if(v_key == 2)
    {
        if(h_key == 0)//下
        {
            Bullet *m_bullet2 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(4,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 1)//左下
        {
            Bullet *m_bullet2 = new Bullet(0,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(4,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 2)//右下
        {
            Bullet *m_bullet2 = new Bullet(0,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(4,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
    }

    //重置发射时间间隔记录
    m_recorder = 0;
}

void Player::shoot6(int v_key,int h_key)//玩家6发射子弹
{
    m_recorder++;//判断如果记录数字 未达到发射间隔，直接return
    if(m_recorder < bulletinterval) return;

    if(v_key || h_key)
    {
        Bullet *m_bullet = new Bullet(v_key,h_key,chooseindex,bulletspeed);//到达发射时间处理
        bulletstore.append(m_bullet);
        bulletout(m_bullet);
    }

    if(v_key == 0)
    {
        if(h_key == 0)//停止
        {
            if(face == 1)
            {
                Bullet *m_bullet = new Bullet(1,0,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet);
                bulletout(m_bullet);
                Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet2);
                bulletout(m_bullet2);
                Bullet *m_bullet3 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet3);
                bulletout(m_bullet3);
                Bullet *m_bullet4 = new Bullet(3,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet4);
                bulletout(m_bullet4);
                Bullet *m_bullet5 = new Bullet(3,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet5);
                bulletout(m_bullet5);
            }
            else if(face == 2)
            {
                Bullet *m_bullet = new Bullet(2,0,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet);
                bulletout(m_bullet);
                Bullet *m_bullet2 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet2);
                bulletout(m_bullet2);
                Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet3);
                bulletout(m_bullet3);
                Bullet *m_bullet4 = new Bullet(4,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet4);
                bulletout(m_bullet4);
                Bullet *m_bullet5 = new Bullet(4,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet5);
                bulletout(m_bullet5);
            }
            else if(face == 3)
            {
                Bullet *m_bullet = new Bullet(0,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet);
                bulletout(m_bullet);
                Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet2);
                bulletout(m_bullet2);
                Bullet *m_bullet3 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet3);
                bulletout(m_bullet3);
                Bullet *m_bullet4 = new Bullet(3,3,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet4);
                bulletout(m_bullet4);
                Bullet *m_bullet5 = new Bullet(4,0,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet5);
                bulletout(m_bullet5);
            }
            else if(face == 4)
            {
                Bullet *m_bullet = new Bullet(0,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet);
                bulletout(m_bullet);
                Bullet *m_bullet2 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet2);
                bulletout(m_bullet2);
                Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet3);
                bulletout(m_bullet3);
                Bullet *m_bullet4 = new Bullet(3,0,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet4);
                bulletout(m_bullet4);
                Bullet *m_bullet5 = new Bullet(4,3,chooseindex,bulletspeed);//到达发射时间处理
                bulletstore.append(m_bullet5);
                bulletout(m_bullet5);
            }
        }
        else if(h_key == 1)//左
        {
            Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 2)//右
        {
            Bullet *m_bullet2 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
    }

    else if(v_key == 1)
    {
        if(h_key == 0)//上
        {
            Bullet *m_bullet2 = new Bullet(1,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(1,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(3,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 1)//左上
        {
            Bullet *m_bullet2 = new Bullet(0,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(1,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(3,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 2)//右上
        {
            Bullet *m_bullet2 = new Bullet(0,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(1,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(3,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(3,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
    }

    else if(v_key == 2)
    {
        if(h_key == 0)//下
        {
            Bullet *m_bullet2 = new Bullet(2,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(4,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 1)//左下
        {
            Bullet *m_bullet2 = new Bullet(0,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(4,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,1,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
        else if(h_key == 2)//右下
        {
            Bullet *m_bullet2 = new Bullet(0,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet2);
            bulletout(m_bullet2);
            Bullet *m_bullet3 = new Bullet(2,0,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet3);
            bulletout(m_bullet3);
            Bullet *m_bullet4 = new Bullet(4,2,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet4);
            bulletout(m_bullet4);
            Bullet *m_bullet5 = new Bullet(4,3,chooseindex,bulletspeed);//到达发射时间处理
            bulletstore.append(m_bullet5);
            bulletout(m_bullet5);
        }
    }

    //重置发射时间间隔记录
    m_recorder = 0;
}

void Player::bulletout(Bullet *bullet)//玩家子弹射出后子弹的参数配置
{
    if(bullet->m_Free)
    {
        bullet->m_Free = false;//将改子弹空闲状态改为假
        //设置发射的子弹坐标
        bullet->m_X = m_X + m_Rect.width()*0.5 - 18;
        bullet->m_Y = m_Y + m_Rect.height()*0.5 - 25;
        bullet->m_Rect.moveTo(bullet->m_X,bullet->m_Y);
    }
}

