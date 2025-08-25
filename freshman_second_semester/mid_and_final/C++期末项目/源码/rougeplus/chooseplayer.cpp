#include "chooseplayer.h"

#include <QPainter>
#include <QPixmap>

#include "myconfig.h"

ChoosePlayer::ChoosePlayer(QWidget *parent)
    : Scene{parent}
{
    this->setWindowTitle("挑选一个玩家吧");

    //选中标签
    QFont font;
    chooselabel->setParent(this);
    font.setFamily("华文新魏");
    font.setPointSize(20);
    chooselabel->setFont(font);
    choosestr = QString("现在选择的玩家是:%1").arg(chooseindex);
    chooselabel->setText(choosestr);
    chooselabel->move(GAME_WIDTH/2-130,-200);

    //六个选择人物按钮
    QPushButton *btn1 = new QPushButton("chooseplayer1",this);
    btn1->setFixedSize(150,150);
    btn1->move(160+0*((GAME_WIDTH-320)/3)+100+20,110+0*((GAME_HEIGHT-110)/2));
    btn1->show();
    connect(btn1,&QPushButton::clicked,this,[=]()
    {
        emit this->choose1();
        chooseindex = 1;
        choosestr = QString("现在选择的玩家是:%1").arg(chooseindex);
        chooselabel->setText(choosestr);
    });

    QPushButton *btn2 = new QPushButton("chooseplayer2",this);
    btn2->setFixedSize(150,150);
    btn2->move(160+1*((GAME_WIDTH-320)/3)+100+20,110+0*((GAME_HEIGHT-110)/2));
    btn2->show();
    connect(btn2,&QPushButton::clicked,this,[=]()
    {
        emit this->choose2();
        chooseindex = 2;
        choosestr = QString("现在选择的玩家是:%1").arg(chooseindex);
        chooselabel->setText(choosestr);
    });

    QPushButton *btn3 = new QPushButton("chooseplayer3",this);
    btn3->setFixedSize(150,150);
    btn3->move(160+2*((GAME_WIDTH-320)/3)+100+20,110+0*((GAME_HEIGHT-110)/2));
    btn3->show();
    connect(btn3,&QPushButton::clicked,this,[=]()
    {
        emit this->choose3();
        chooseindex = 3;
        choosestr = QString("现在选择的玩家是:%1").arg(chooseindex);
        chooselabel->setText(choosestr);
    });

    QPushButton *btn4 = new QPushButton("chooseplayer4",this);
    btn4->setFixedSize(150,150);
    btn4->move(160+0*((GAME_WIDTH-320)/3)+100+20,60+1*((GAME_HEIGHT-110)/2));
    btn4->show();
    connect(btn4,&QPushButton::clicked,this,[=]()
    {
        emit this->choose4();
        chooseindex = 4;
        choosestr = QString("现在选择的玩家是:%1").arg(chooseindex);
        chooselabel->setText(choosestr);
    });

    QPushButton *btn5 = new QPushButton("chooseplayer5",this);
    btn5->setFixedSize(150,150);
    btn5->move(160+1*((GAME_WIDTH-320)/3)+100+20,60+1*((GAME_HEIGHT-110)/2));
    btn5->show();
    connect(btn5,&QPushButton::clicked,this,[=]()
    {
        emit this->choose5();
        chooseindex = 5;
        choosestr = QString("现在选择的玩家是:%1").arg(chooseindex);
        chooselabel->setText(choosestr);
    });

    label6_1 = new QLabel("       你点我不到",this);
    label6_1->setFixedSize(150,150);
    label6_1->move(160+2*((GAME_WIDTH-320)/3)+100+20,60+1*((GAME_HEIGHT-110)/2));
    label6_1->show();

    btn6_2 = new QPushButton("chooseplayer6",this);
    btn6_2->setFixedSize(150,150);
    btn6_2->move(160+2*((GAME_WIDTH-320)/3)+100+20,60+1*((GAME_HEIGHT-110)/2));
    btn6_2->hide();
    connect(btn6_2,&QPushButton::clicked,this,[=]()
    {
        emit this->choose6();
        chooseindex = 6;
        choosestr = QString("现在选择的玩家是:%1").arg(chooseindex);
        chooselabel->setText(choosestr);
    });

}

void ChoosePlayer::paintEvent(QPaintEvent *event)//重写绘图事件
{
    //画游戏背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/WindowBackground.gif");
    painter.drawPixmap(0,0,GAME_WIDTH,GAME_HEIGHT,pix);

    //显示六种玩家的图片、选择按钮、参数
    QPainter painter1(this);
    QPixmap pix1;
    pix1.load(":/resource/player1right.png");
    painter1.drawPixmap(160+0*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2),100,100,pix1);
    label1->setParent(this);
    label1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label1->setText(QString("hp:%1\nspeed:%2\nattack:%3\nbulletinterval:%4\nbulletspeed:%5\n技能:5秒吸血").arg(hp1).arg(speed1).arg(attack1).arg(bulletinterval1).arg(bulletspeed1));
    label1->setGeometry(160+0*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2)+100,100,150);
    label1->show();

    QPainter painter2(this);
    QPixmap pix2;
    pix2.load(":/resource/player2right.png");
    painter2.drawPixmap(160+1*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2),100,100,pix2);
    label2->setParent(this);
    label2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label2->setText(QString("hp:%1\nspeed:%2\nattack:%3\nbulletinterval:%4\nbulletspeed:%5\n技能:10秒加攻速").arg(hp2).arg(speed2).arg(attack2).arg(bulletinterval2).arg(bulletspeed2));
    label2->setGeometry(160+1*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2)+100,100,150);
    label2->show();

    QPainter painter3(this);
    QPixmap pix3;
    pix3.load(":/resource/player3right.png");
    painter3.drawPixmap(160+2*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2),100,100,pix3);
    label3->setParent(this);
    label3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label3->setText(QString("hp:%1\nspeed:%2\nattack:%3\nbulletinterval:%4\nbulletspeed:%5\n技能:增加金币\n自带一次复活").arg(hp3).arg(speed3).arg(attack3).arg(bulletinterval3).arg(bulletspeed3));
    label3->setGeometry(160+2*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2)+100,100,150);
    label3->show();

    QPainter painter4(this);
    QPixmap pix4;
    pix4.load(":/resource/player4right.png");
    painter4.drawPixmap(160+0*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2),100,100,pix4);
    label4->setParent(this);
    label4->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label4->setText(QString("hp:%1\nspeed:%2\nattack:%3\nbulletinterval:%4\nbulletspeed:%5\n技能:10秒无敌\n子弹爆炸").arg(hp4).arg(speed4).arg(attack4).arg(bulletinterval4).arg(bulletspeed4));
    label4->setGeometry(160+0*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2)+100,100,150);
    label4->show();

    QPainter painter5(this);
    QPixmap pix5;
    pix5.load(":/resource/player5right.png");
    painter5.drawPixmap(160+1*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2),100,100,pix5);
    label5->setParent(this);
    label5->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label5->setText(QString("hp:%1\nspeed:%2\nattack:%3\nbulletinterval:%4\nbulletspeed:%5\n技能:5秒伤害狂暴\n子弹穿透").arg(hp5).arg(speed5).arg(attack5).arg(bulletinterval5).arg(bulletspeed5));
    label5->setGeometry(160+1*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2)+100,100,150);
    label5->show();

    QPainter painter6(this);
    QPixmap pix6;
    pix6.load(":/resource/player6.jpg");
    painter6.drawPixmap(160+2*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2),100,100,pix6);
    label6->setParent(this);
    label6->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label6->setText(QString("hp:%1\nspeed:%2\nattack:%3\nbulletinterval:%4\nbulletspeed:%5\n技能:增加大量金币\n出场自带999金币\n无敌且穿透\n子弹爆炸且穿透").arg(hp6).arg(speed6).arg(attack6).arg(bulletinterval6).arg(bulletspeed6));
    label6->setGeometry(160+2*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2)+100,100,150);
    label6->show();
}
