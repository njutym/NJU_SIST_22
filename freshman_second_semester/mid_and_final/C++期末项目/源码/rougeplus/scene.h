//场景类，作为商店类、游玩界面类、作弊类、帮助类的父类
#ifndef SCENE_H
#define SCENE_H

#include <QMainWindow>
#include <QCloseEvent>

class Scene: public QMainWindow
{
    Q_OBJECT
public:
    explicit Scene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void setup();//设置游玩界面属性及图标
    void setbar();//左上角菜单栏
    void setbackbutton();//返回按钮
    void closeEvent(QCloseEvent *event);
signals:
    void chooseSceneBack();//返回信号
    void save();//存档信号
};

#endif // SCENE_H
