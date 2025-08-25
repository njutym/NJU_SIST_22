#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QSettings>

#include "mypushbutton.h"
#include "myconfig.h"
#include "playscene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainscene();//设置主界面
    play();//设置游玩界面
    store();//设置商店界面
    choose();//设置选择人物界面
    cheet();//设置作弊界面
    help();//设置帮助界面

    loadsave();//读取存档
    loadstore();//导入上商店购买数据
    loadplayer();//导入玩家参数

    //实现在非主界面退出时的存档
    connect(selectscene,&ChoosePlayer::save,this,[=]()
    {
        do_save();
    });
    connect(storescene,&MyStore::save,this,[=]()
    {
        do_save();
    });
    connect(cheetscene,&MyCheet::save,this,[=]()
    {
        do_save();
    });
    connect(helpscene,&HelpScene::save,this,[=]()
    {
        do_save();
    });

    //监听作弊行为,如果正在作弊，开启玩家6选择按钮
    connect(cheetscene,&MyCheet::docheet,this,[=]()
    {
        ischeet = true;
        selectscene->label6_1->hide();
        selectscene->btn6_2->show();
    });
    connect(cheetscene,&MyCheet::cancelcheet,this,[=]()
    {
        ischeet = false;
        selectscene->btn6_2->hide();
        selectscene->label6_1->show();
    });
}

void MainWindow::loadsave()
{
    QSettings setting("./Control Parameter.ini",QSettings::IniFormat);

    //导入金钱数量
    setting.beginGroup("moneynum");
    mymoney = setting.value("money",0).toInt();
    setting.endGroup();

    //导入全局道具购买数量
    setting.beginGroup("goodnum");
    goodnum1 = setting.value("goodnum1",0).toInt();
    goodnum2 = setting.value("goodnum2",0).toInt();
    goodnum3 = setting.value("goodnum3",0).toInt();
    goodnum4 = setting.value("goodnum4",0).toInt();
    goodnum5 = setting.value("goodnum5",0).toInt();
    goodnum6 = setting.value("goodnum6",0).toInt();
    setting.endGroup();

    //读入存档改变相应数值
    storescene->moneynum = mymoney;
    storescene->buytotal1 = goodnum1;
    storescene->buytotal2 = goodnum2;
    storescene->buytotal3 = goodnum3;
    storescene->buytotal4 = goodnum4;
    storescene->buytotal5 = goodnum5;
    storescene->buytotal6 = goodnum6;
}

void MainWindow::loadplayer()
{
    //默认选择玩家
    chooseindex = 1;
    hp = selectscene->hp1;
    speed = selectscene->speed1;
    attack = selectscene->attack1;
    bulletinterval = selectscene->bulletinterval1;
    bulletspeed = selectscene->bulletspeed1;

    //点击选择按钮，实现玩家参数导入
    connect(selectscene,&ChoosePlayer::choose1,this,[=]()
    {
        chooseindex = 1;
        hp = selectscene->hp1;
        speed = selectscene->speed1;
        attack = selectscene->attack1;
        bulletinterval = selectscene->bulletinterval1;
        bulletspeed = selectscene->bulletspeed1;
    });
    connect(selectscene,&ChoosePlayer::choose2,this,[=]()
    {
        chooseindex = 2;
        hp = selectscene->hp2;
        speed = selectscene->speed2;
        attack = selectscene->attack2;
        bulletinterval = selectscene->bulletinterval2;
        bulletspeed = selectscene->bulletspeed2;
    });
    connect(selectscene,&ChoosePlayer::choose3,this,[=]()
    {
        chooseindex = 3;
        hp = selectscene->hp3;
        speed = selectscene->speed3;
        attack = selectscene->attack3;
        bulletinterval = selectscene->bulletinterval3;
        bulletspeed = selectscene->bulletspeed3;
    });
    connect(selectscene,&ChoosePlayer::choose4,this,[=]()
    {
        chooseindex = 4;
        hp = selectscene->hp4;
        speed = selectscene->speed4;
        attack = selectscene->attack4;
        bulletinterval = selectscene->bulletinterval4;
        bulletspeed = selectscene->bulletspeed4;
    });
    connect(selectscene,&ChoosePlayer::choose5,this,[=]()
    {
        chooseindex = 5;
        hp = selectscene->hp5;
        speed = selectscene->speed5;
        attack = selectscene->attack5;
        bulletinterval = selectscene->bulletinterval5;
        bulletspeed = selectscene->bulletspeed5;
    });
    connect(selectscene,&ChoosePlayer::choose6,this,[=]()
    {
        chooseindex = 6;
        hp = selectscene->hp6;
        speed = selectscene->speed6;
        attack = selectscene->attack6;
        bulletinterval = selectscene->bulletinterval6;
        bulletspeed = selectscene->bulletspeed6;
    });
}

void MainWindow::loadstore()//导入上商店购买数据
{
    //点击商店购买按钮，实现玩家全局强化参数导入
    connect(storescene,&MyStore::choose1,this,[=]()
    {
        selectscene->hp1++;
        if(chooseindex == 1)//防止默认选择人物情况不及时更新买到的加血标志
        {
            hp = selectscene->hp1;
        }
        selectscene->hp2++;
        selectscene->hp3++;
        selectscene->hp4++;
        selectscene->hp5++;
        //玩家6保持5201314血量
        mymoney -= 100;
    });
    connect(storescene,&MyStore::choose2,this,[=]()
    {
        bombnum++;
        mymoney -= 500;
    });
    connect(storescene,&MyStore::choose3,this,[=]()
    {
        coindropprobability += 0.01;
        mymoney -= 800;
    });
    connect(storescene,&MyStore::choose4,this,[=]()
    {
        expincreasspeed++;
        mymoney -= 1000;
    });
    connect(storescene,&MyStore::choose5,this,[=]()
    {
        resurrectionnum++;
        mymoney -= 2000;
    });
    connect(storescene,&MyStore::choose6,this,[=]()
    {
        skillinterval -= 5;
        mymoney -= 600;
    });
}

void MainWindow::mainscene()//设置主界面
{
    //设置主界面属性
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);
    setWindowIcon(QIcon(":/resource/WindowIcon.jpg"));
    setWindowTitle("田大王强带你玩肉鸽");

    //设置主界面左下角图标
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1 = QString("肉鸽 tym");
    label->setFont(font);
    label->setText(str1);
    label->setGeometry(20,GAME_HEIGHT-50,120,50);

    //菜单栏点击退出实现退出游戏
    connect(ui->Quit,&QAction::triggered,this,[=]()
    {
        this->close();
    });
}

void MainWindow::play()//设置开始按钮，连接游玩界面
{
    //设置开始按钮并添加特效
    MyPushButton * startBtn = new MyPushButton(":/resource/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.25);
    connect(startBtn,&MyPushButton::clicked,this,[=]()
    {
        PlayScene *playscene = new PlayScene(chooseindex,hp,speed,attack,bulletinterval,bulletspeed,bombnum,resurrectionnum,coindropprobability,expincreasspeed,skillinterval);//点击开始，构造PlayScene
        connect(playscene,&PlayScene::increasemoney,this,[=]()//增加全局金币
        {
            mymoney += playscene->coinnum;
            storescene->moneynum = mymoney;
        });
        connect(playscene,&PlayScene::save,this,[=]()
        {
            do_save();
        });
        connect(playscene,&PlayScene::chooseSceneBack,this,[=]()//点击返回，回到主界面，将PlayScene析构
        {
            this->setGeometry(playscene->geometry());//优化窗口
            playscene->stopgame();//额外申请的空间的析构
            playscene->~PlayScene();//普通析构
            this->show();
        });
        startBtn->zoom1();
        startBtn->zoom2();
        playscene->reset();
        QTimer::singleShot(300,this,[=]()//设置切换界面时间间隔
        {
            this->hide();
            playscene->setGeometry(this->geometry());//优化窗口
            playscene->show();
            playscene->playgame();
        });
    });
}

void MainWindow::store()//设置商店界面
{
    //设置商店按钮
    QPushButton * storeBtn = new QPushButton("商店",this);
    storeBtn->resize(150,150*0.618);
    storeBtn->move(this->width()*0.5-storeBtn->width()*0.5,this->height()*0.6);
    storescene = new MyStore;
    connect(storescene,&MyStore::chooseSceneBack,this,[=]()
    {
        this->setGeometry(storescene->geometry());
        storescene->hide();
        this->show();
    });
    connect(storeBtn,&MyPushButton::clicked,this,[=]()
    {
        storescene->setGeometry(this->geometry());
        this->hide();
        storescene->show();
    });
}

void MainWindow::choose()//设置选择人物界面
{
    //设置选择人物按钮
    QPushButton * selectBtn = new QPushButton("选择人物",this);
    selectBtn->resize(150,150*0.618);
    selectBtn->move(this->width()*0.5-selectBtn->width()*0.5,this->height()*0.45);
    selectscene = new ChoosePlayer;
    connect(selectscene,&ChoosePlayer::chooseSceneBack,this,[=]()
    {
        this->setGeometry(selectscene->geometry());
        selectscene->hide();
        this->show();
    });
    connect(selectBtn,&MyPushButton::clicked,this,[=]()
    {
        selectscene->setGeometry(this->geometry());
        this->hide();
        selectscene->show();
    });
}

void MainWindow::cheet()//设置作弊界面
{
    //设置作弊按钮
    QPushButton * cheetBtn = new QPushButton("作弊",this);
    cheetBtn->resize(150,150*0.618);
    cheetBtn->move(this->width()*0.5-cheetBtn->width()*0.5,this->height()*0.75);
    cheetscene = new MyCheet;
    connect(cheetscene,&MyCheet::chooseSceneBack,this,[=]()
    {
        this->setGeometry(cheetscene->geometry());
        cheetscene->hide();
        this->show();
    });
    connect(cheetBtn,&MyPushButton::clicked,this,[=]()
    {
        cheetscene->setGeometry(this->geometry());
        this->hide();
        cheetscene->show();
    });
}

void MainWindow::help()//设置帮助界面
{
    //设置帮助按钮
    QPushButton * helpBtn = new QPushButton("帮助",this);
    helpBtn->resize(150,150*0.618);
    helpBtn->move(this->width()*0.5+400,this->height()*0.75-50);
    helpscene = new HelpScene;
    connect(helpscene,&HelpScene::chooseSceneBack,this,[=]()
    {
        this->setGeometry(helpscene->geometry());
        helpscene->hide();
        this->show();
    });
    connect(helpBtn,&MyPushButton::clicked,this,[=]()
    {
        helpscene->setGeometry(this->geometry());
        this->hide();
        helpscene->show();
    });
}

void MainWindow::paintEvent(QPaintEvent *)
{
    //绘制主界面背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/WindowBackground.gif");
    painter.drawPixmap(0,0,GAME_WIDTH,GAME_HEIGHT,pix);
}

void MainWindow::closeEvent(QCloseEvent *)//重写关闭游戏事件，实现存档
{
    do_save();
}

void MainWindow::do_save()//进行存档
{
    QSettings setting("./Control Parameter.ini",QSettings::IniFormat);

    //全局金币数量存档
    setting.beginGroup("moneynum");
    setting.setValue("money",mymoney);
    setting.endGroup();

    //全局道具购买数量存档
    setting.beginGroup("goodnum");
    setting.setValue("goodnum1",goodnum1);
    setting.setValue("goodnum2",goodnum2);
    setting.setValue("goodnum3",goodnum3);
    setting.setValue("goodnum4",goodnum4);
    setting.setValue("goodnum5",goodnum5);
    setting.setValue("goodnum6",goodnum6);
    setting.endGroup();
}

MainWindow::~MainWindow()
{
    delete ui;
}


