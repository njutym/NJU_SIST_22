//主界面类
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "mycheet.h"
#include "mystore.h"
#include "chooseplayer.h"
#include "helpscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    //人物选择参数
    int chooseindex;
    int hp;
    double speed;
    int attack;
    int bulletinterval;
    double bulletspeed;
    int bombnum = 1;
    int resurrectionnum = 0;
    double coindropprobability = 0;
    int expincreasspeed = 0;
    int skillinterval = 60;

    //全局道具存档信息
    int goodnum1;
    int goodnum2;
    int goodnum3;
    int goodnum4;
    int goodnum5;
    int goodnum6;
    int mymoney;//全局金币数量

    bool ischeet = false;//是否开启作弊

    //商店、作弊、选择人物、帮助场景
    MyStore * storescene = NULL;
    MyCheet * cheetscene = NULL;
    ChoosePlayer * selectscene = NULL;
    HelpScene * helpscene = NULL;

public:
    MainWindow(QWidget *parent = nullptr);//构造函数
    ~MainWindow();//析构函数
    void paintEvent(QPaintEvent * event);//绘图事件
    void mainscene();//设置主界面
    void play();//设置游玩界面
    void store();//设置商店界面
    void choose();//设置选择人物界面
    void cheet();//设置作弊界面
    void help();//设置帮助界面
    void loadplayer();//导入玩家参数
    void loadstore();//导入上商店购买数据

    void loadsave();//读取存档
    void do_save();//进行存档
    void closeEvent(QCloseEvent *);//重写关闭游戏事件，实现存档
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
