#include <QMenuBar>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>

#include "myconfig.h"
#include "scene.h"
#include "mypushbutton.h"

Scene::Scene(QWidget * parent)
    : QMainWindow{parent}
{
    setup(); //设置游玩界面属性及图标
    setbar();//左上角菜单栏
    setbackbutton();//返回按钮
}

void Scene::setup()
{
    this->setFixedSize(GAME_WIDTH,GAME_HEIGHT);
    this->setWindowIcon(QIcon(":/resource/WindowIcon.jpg"));
    this->setWindowTitle("");
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1 = QString("肉鸽 tym");
    label->setFont(font);
    label->setText(str1);
    label->setGeometry(20,GAME_HEIGHT-50,120,50);
}

void Scene::setbar()
{
    QMenuBar * bar = menuBar();
    setMenuBar(bar);
    QMenu * startMenu = bar->addMenu("开始");
    QAction * quit = startMenu->addAction("退出");
    connect(quit,&QAction::triggered,this,[=]()
    {
        this->close();
    });
}

void Scene::setbackbutton()
{
    MyPushButton * backBtn = new MyPushButton(":/resource/BackButton.png","://resource/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());
    connect(backBtn,&MyPushButton::clicked,this,[=]()
    {
        QTimer::singleShot(200,this,[=]()
        {
            emit this->chooseSceneBack();
        });
    });
}

void Scene::paintEvent(QPaintEvent *)//绘制背景图片
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/WindowBackground.gif");
    painter.drawPixmap(0,0,GAME_WIDTH,GAME_HEIGHT,pix);
}


void Scene::closeEvent(QCloseEvent *event)
{
    emit this->save();
}


