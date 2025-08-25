#include "bullet.h"

#include "myconfig.h"

Bullet::Bullet(int v_key,int h_key,int bulletindex,double BULLET_SPEED)//玩家子弹构造
{
    vertical_direction = v_key;//保存传进来的恒定垂直方向
    horizontal_direction = h_key;//保存传进来的恒定水平方向
    if(bulletindex == 5)
    {
        srand((unsigned int)time(NULL));//时间种子随机数
        int randbullet = rand()%4;
        //加载子弹图片
        if(randbullet == 0) m_Bullet.load(":/resource/bullet5-1.png");
        else if(randbullet == 1) m_Bullet.load(":/resource/bullet5-2.png");
        else if(randbullet == 2) m_Bullet.load(":/resource/bullet5-3.png");
        else if(randbullet == 3) m_Bullet.load(":/resource/bullet5-4.png");
    }
    else m_Bullet.load(QString(":/resource/bullet%1.png").arg(bulletindex));//加载子弹图片
    m_Free = true;//子弹初始状态设置为true
    m_Speed = BULLET_SPEED;//子弹速度
    if(bulletindex == 6) m_Bullet = m_Bullet.scaled(94,94);
    m_Rect.setWidth(m_Bullet.width());//子弹矩形框
    m_Rect.setHeight(m_Bullet.height());//子弹矩形框
}

void Bullet::left()//左
{
    m_X  -= m_Speed;
    if(m_X <= -m_Rect.width()) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::right()//右
{
    m_X  += m_Speed;
    if(m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::up()//上
{
    m_Y  -= m_Speed;
    if(m_Y <= -m_Rect.height()) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::down()//下
{
    m_Y += m_Speed;
    if(m_Y >= m_Rect.height() + GAME_HEIGHT) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::left_up()//左上
{
    m_X  -= m_Speed/sqrt(2);
    if(m_X <= -m_Rect.width()) m_Free = true;
    m_Y  -= m_Speed/sqrt(2);
    if(m_Y <= -m_Rect.height()) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::left_down()//左下
{
    m_X  -= m_Speed/sqrt(2);
    if(m_X <= -m_Rect.width()) m_Free = true;
    m_Y += m_Speed/sqrt(2);
    if(m_Y >= m_Rect.height() + GAME_HEIGHT) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::right_up()//右上
{
    m_X  += m_Speed/sqrt(2);
    if(m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
    m_Y  -= m_Speed/sqrt(2);
    if(m_Y <= -m_Rect.height()) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::right_down()//右下
{
    m_X  += m_Speed/sqrt(2);
    if(m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
    m_Y += m_Speed/sqrt(2);
    if(m_Y >= m_Rect.height() + GAME_HEIGHT) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::rightup_right()//右上和右之间
{
    m_X += m_Speed*cos(M_PI/8);
    if(m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
    m_Y -= m_Speed*sin(M_PI/8);
    if(m_Y <= -m_Rect.height()) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::rightup_up()//右上和上之间
{
    m_X  += m_Speed*sin(M_PI/8);
    if(m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
    m_Y  -= m_Speed*cos(M_PI/8);
    if(m_Y <= -m_Rect.height()) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::leftup_up()//左上和上之间
{
    m_X  -= m_Speed*sin(M_PI/8);
    if(m_X <= -m_Rect.width()) m_Free = true;
    m_Y  -= m_Speed*cos(M_PI/8);
    if(m_Y <= -m_Rect.height()) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::leftup_left()//左上和左之间
{
    m_X  -= m_Speed*cos(M_PI/8);
    if(m_X <= -m_Rect.width()) m_Free = true;
    m_Y  -= m_Speed*sin(M_PI/8);
    if(m_Y <= -m_Rect.height()) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::leftdown_left()//左下和左之间
{
    m_X  -= m_Speed*cos(M_PI/8);
    if(m_X <= -m_Rect.width()) m_Free = true;
    m_Y += m_Speed*sin(M_PI/8);
    if(m_Y >= m_Rect.height() + GAME_HEIGHT) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::leftdown_down()//左下和下之间
{
    m_X  -= m_Speed*sin(M_PI/8);
    if(m_X <= -m_Rect.width()) m_Free = true;
    m_Y += m_Speed*cos(M_PI/8);
    if(m_Y >= m_Rect.height() + GAME_HEIGHT) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::rightdown_down()//右下和下之间
{
    m_X  += m_Speed*sin(M_PI/8);
    if(m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
    m_Y += m_Speed*cos(M_PI/8);
    if(m_Y >= m_Rect.height() + GAME_HEIGHT) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::rightdown_right()//右下和右之间
{
    m_X  += m_Speed*cos(M_PI/8);
    if(m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
    m_Y += m_Speed*sin(M_PI/8);
    if(m_Y >= m_Rect.height() + GAME_HEIGHT) m_Free = true;
    m_Rect.moveTo(m_X,m_Y);
}

void Bullet::updatePosition()//玩家子弹更新位置、打Boss时精英怪子弹更新位置
{
    //玩家可以控制子弹的空闲状态为false
    //子弹移动
    switch(vertical_direction)
    {
    case 0:
        switch(horizontal_direction)
        {
        case 0://停止
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            return;
        case 1://左
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            left();
            return;
        case 2://右
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            right();
            return;
        }
    case 1:
        switch(horizontal_direction)
        {
        case 0://上
            up();
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            return;
        case 1://左上
            left_up();
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            return;
        case 2://右上
            right_up();
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            return;
        }
    case 2:
        switch(horizontal_direction)
        {
        case 0://下
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            down();
            return;
        case 1://左下
            left_down();
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            return;
        case 2://右下
            right_down();
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            return;
        }
    case 3:
        switch(horizontal_direction)
        {
        case 0://右上和右之间
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            rightup_right();
            return;
        case 1://右上和上之间
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            rightup_up();
            return;
        case 2://左上和上之间
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            leftup_up();
            return;
        case 3://左上和左之间
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            leftup_left();
            return;
        }
    case 4:
        switch(horizontal_direction)
        {
        case 0://左下和左之间
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            leftdown_left();
            return;
        case 1://左下和下之间
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            leftdown_down();
            return;
        case 2://右下和下之间
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            rightdown_down();
            return;
        case 3://右下和右之间
            //消除出界子弹
            if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
                || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
            rightdown_right();
            return;
        }
    default:
        return;
    }
}

Bullet::Bullet(double x,double y)//精英敌人子弹构造
{
    direction_x = x;//保存传进来的恒定位置
    direction_y = y;//保存传进来的恒定位置
    vertical_direction = x;//保存传进来的恒定垂直方向
    horizontal_direction = y;//保存传进来的恒定水平方向
    m_Bullet.load(ENEMY_BULLET_PATH);//加载子弹图片
    m_Free = true;//子弹状态
    m_Speed = ENEMY_BULLET_SPEED;//子弹速度
    m_Rect.setWidth(m_Bullet.width());//子弹矩形框
    m_Rect.setHeight(m_Bullet.height());//子弹矩形框
}

void Bullet::eliteenemyupdatePosition(int num)//精英敌人子弹更新位置
{
    if(num < 170)
    {
        //子弹移动，勾股定理算移动x和y的距离
        double dx = direction_x - m_X;
        double dy = direction_y - m_Y;
        double hypotenus = sqrt(dx*dx + dy*dy);
        double xspeed = m_Speed*dx/hypotenus;
        double yspeed = m_Speed*dy/hypotenus;
        m_X += xspeed;
        m_Y += yspeed;
        m_Rect.moveTo(m_X,m_Y);
        //消除出界子弹
        if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
        || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
    }
    else
    {
        updatePosition();
    }
}

Bullet::Bullet(double x,double y,int kind,double randangle,double shoot_x,double shoot_y)//Boss子弹构造
{
    angle = randangle*M_PI/180;
    direction_x = x;
    direction_y = y;
    shootx = shoot_x;
    shooty = shoot_y;
    m_Free = true;
    bulletkind = kind;
    switch(kind)
    {
    case 1: case 4:
        m_Bullet.load(":/resource/bossbullet1.png");
        m_Rect.setWidth(m_Bullet.width());//子弹矩形框
        m_Rect.setHeight(m_Bullet.height());//子弹矩形框
        m_Speed = 1;
        break;
    case 2:
        m_Bullet.load(":/resource/bossbullet2.png");
        m_Rect.setWidth(m_Bullet.width());//子弹矩形框
        m_Rect.setHeight(m_Bullet.height());//子弹矩形框
        m_Speed = 1.1;
        break;
    case 3:
        m_Bullet.load(":/resource/bossbullet3.png");
        m_Rect.setWidth(m_Bullet.width());//子弹矩形框
        m_Rect.setHeight(m_Bullet.height());//子弹矩形框
        m_Speed = 0.8;
        break;
    case 5:
        m_Bullet.load(":/resource/bossbullet4.png");
        m_Bullet = m_Bullet.scaled(200,200);
        m_Rect.setWidth(m_Bullet.width());//子弹矩形框
        m_Rect.setHeight(m_Bullet.height());//子弹矩形框
        m_Speed = 0.8;
        break;
    default:
        break;
    }
}

void Bullet::bossbulletupdatePosition(int currentx,int currenty)//Boss类子弹更新位置
{
    double dx = currentx - m_X;
    double dy = currenty - m_Y;
    double hypotenus = sqrt(dx*dx + dy*dy);
    double xspeed = m_Speed*dx/hypotenus;
    double yspeed = m_Speed*dy/hypotenus;
    switch(bulletkind)
    {
    case 1: case 2: case 4: case 5:
        m_Y -= m_Speed*sin(angle);//!!!注意电脑坐标系和数学坐标系的不同！！！原点在左上角
        m_X += m_Speed*cos(angle);
        m_Rect.moveTo(m_X,m_Y);
        break;
    case 3:
        m_X += xspeed;
        m_Y += yspeed;
        m_Rect.moveTo(m_X,m_Y);
        if(sqrt((shootx - m_X) * (shootx - m_X)+ (shooty - m_Y) * (shooty - m_Y)) >= 500)
        {
            m_Free = true;
        }
        break;
    default:
        break;
    }

    if(m_Y <= -m_Rect.height() || m_Y >= m_Rect.height() + GAME_HEIGHT
    || m_X <= -m_Rect.width() || m_X >= m_Rect.width() + GAME_WIDTH) m_Free = true;
}
