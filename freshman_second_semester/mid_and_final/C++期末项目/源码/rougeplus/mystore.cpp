#include "mystore.h"

#include <QPushButton>
#include <QPainter>

#include "myconfig.h"

MyStore::MyStore()
{
    this->setWindowTitle("oh my god 买它买它！");

    //六个购买商品按钮
    QPushButton *btn1 = new QPushButton("buy for 100 coins",this);
    btn1->setFixedSize(150,100);
    btn1->move(160+0*((GAME_WIDTH-320)/3)+100+20,110+0*((GAME_HEIGHT-110)/2));
    btn1->show();
    connect(btn1,&QPushButton::clicked,this,[=]()
    {
        if(moneynum >= 100)
        {
            moneynum -= 100;
            if(buytotal1 == 19) btn1->setText("已达到最大购买数量");
            if(buytotal1 < 20)
            {
                buytotal1++;
                emit this->choose1();
            }
        }
    });

    QPushButton *btn2 = new QPushButton("buy for 500 coins",this);
    btn2->setFixedSize(150,100);
    btn2->move(160+1*((GAME_WIDTH-320)/3)+100+20,110+0*((GAME_HEIGHT-110)/2));
    btn2->show();
    connect(btn2,&QPushButton::clicked,this,[=]()
    {
        if(moneynum >= 500)
        {
            moneynum -= 500;
            if(buytotal2 == 2) btn2->setText("已达到最大购买数量");
            if(buytotal2 < 3)
            {
                buytotal2++;
                emit this->choose2();
            }
        }
    });

    QPushButton *btn3 = new QPushButton("buy for 800 coins",this);
    btn3->setFixedSize(150,100);
    btn3->move(160+2*((GAME_WIDTH-320)/3)+100+20,110+0*((GAME_HEIGHT-110)/2));
    btn3->show();
    connect(btn3,&QPushButton::clicked,this,[=]()
    {
        if(moneynum >= 800)
        {
            moneynum -= 800;
            if(buytotal3 == 4) btn3->setText("已达到最大购买数量");
            if(buytotal3 < 5)
            {
                buytotal3++;
                emit this->choose3();
            }
        }
    });

    QPushButton *btn4 = new QPushButton("buy for 1000 coins",this);
    btn4->setFixedSize(150,100);
    btn4->move(160+0*((GAME_WIDTH-320)/3)+100+20,60+1*((GAME_HEIGHT-110)/2));
    btn4->show();
    connect(btn4,&QPushButton::clicked,this,[=]()
    {
        if(moneynum >= 1000)
        {
            moneynum -= 1000;
            if(buytotal4 == 1) btn4->setText("已达到最大购买数量");
            if(buytotal4 < 2)
            {
                buytotal4++;
                emit this->choose4();
           }
        }
    });

    QPushButton *btn5 = new QPushButton("buy for 2000 coins",this);
    btn5->setFixedSize(150,100);
    btn5->move(160+1*((GAME_WIDTH-320)/3)+100+20,60+1*((GAME_HEIGHT-110)/2));
    btn5->show();
    connect(btn5,&QPushButton::clicked,this,[=]()
    {
        if(moneynum >= 2000)
        {
            moneynum -= 2000;
            if(buytotal5 == 0) btn5->setText("已达到最大购买数量");
            if(buytotal5 < 1)
            {
                buytotal5++;
                emit this->choose5();
            }
        }
    });

    QPushButton *btn6 = new QPushButton("buy for 600 coins",this);
    btn6->setFixedSize(150,100);
    btn6->move(160+2*((GAME_WIDTH-320)/3)+100+20,60+1*((GAME_HEIGHT-110)/2));
    btn6->show();
    connect(btn6,&QPushButton::clicked,this,[=]()
    {
        if(moneynum >= 600)
        {
            moneynum-=600;
            if(buytotal6 == 5) btn6->setText("已达到最大购买数量");
            if(buytotal6 < 6)
            {
                buytotal6++;
                emit this->choose6();
            }
        }
    });
}

void MyStore::paintEvent(QPaintEvent *event)//重写绘图事件
{
    //画游戏背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/WindowBackground.gif");
    painter.drawPixmap(0,0,GAME_WIDTH,GAME_HEIGHT,pix);

    //显示六种全局道具的图片、选择按钮、参数
    QPainter painter1(this);
    QPixmap pix1;
    pix1.load(":/resource/good1.png");
    painter1.drawPixmap(160+0*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2),100,100,pix1);
    label1->setParent(this);
    label1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label1->setText(QString("初始血量+1\n至多+20\n\n已购买数量:%1").arg(buytotal1));
    label1->setGeometry(160+0*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2)+100,100,100);
    label1->show();

    QPainter painter2(this);
    QPixmap pix2;
    pix2.load(":/resource/good6.png");
    painter2.drawPixmap(160+1*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2),100,100,pix2);
    label2->setParent(this);
    label2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label2->setText(QString("初始炸弹数量+1\n至多+3\n\n已购买数量:%1").arg(buytotal2));
    label2->setGeometry(160+1*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2)+100,100,100);
    label2->show();

    QPainter painter3(this);
    QPixmap pix3;
    pix3.load(":/resource/good2.png");
    painter3.drawPixmap(160+2*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2),100,100,pix3);
    label3->setParent(this);
    label3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label3->setText(QString("掉金币概率+0.01\n至多为0.05\n\n已购买数量:%1").arg(buytotal3));
    label3->setGeometry(160+2*((GAME_WIDTH-320)/3),110+0*((GAME_HEIGHT-110)/2)+100,100,100);
    label3->show();

    QPainter painter4(this);
    QPixmap pix4;
    pix4.load(":/resource/good8.png");
    painter4.drawPixmap(160+0*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2),100,100,pix4);
    label4->setParent(this);
    label4->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label4->setText(QString("击敌经验加单位倍\n至多增加两倍\n\n已购买数量:%1").arg(buytotal4));
    label4->setGeometry(160+0*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2)+100,100,100);
    label4->show();

    QPainter painter5(this);
    QPixmap pix5;
    pix5.load(":/resource/good9.png");
    painter5.drawPixmap(160+1*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2),100,100,pix5);
    label5->setParent(this);
    label5->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label5->setText(QString("每局可复活次数+1\n至多+1\n\n已购买数量:%1").arg(buytotal5));
    label5->setGeometry(160+1*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2)+100,100,100);
    label5->show();

    QPainter painter6(this);
    QPixmap pix6;
    pix6.load(":/resource/good10.png");
    painter6.drawPixmap(160+2*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2),100,100,pix6);
    label6->setParent(this);
    label6->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label6->setText(QString("技能冷却减少5秒\n至多减少30秒\n\n已购买数量:%1").arg(buytotal6));
    label6->setGeometry(160+2*((GAME_WIDTH-320)/3),60+1*((GAME_HEIGHT-110)/2)+100,100,100);
    label6->show();

    //画钱袋子
    QPainter painter7(this);
    QPixmap pix7;
    pix7.load(":/resource/money.png");
    painter7.drawPixmap(GAME_WIDTH-250,88,100,100,pix7);
    moneylabel->setParent(this);
    moneylabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    moneylabel->setText(QString("金币数量:%1").arg(moneynum));
    moneylabel->setGeometry(GAME_WIDTH-150,88,100,100);
    moneylabel->show();
}

