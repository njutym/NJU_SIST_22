//掉落物类
#ifndef MYGOOD_H
#define MYGOOD_H

#include <QMainWindow>

class MyGood
{
    friend class PlayScene;//友元申明
private:
    //坐标
    double m_X;
    double m_Y;
    int goodindex;//储存掉落物种类
    QPixmap m_object;//资源对象
    QRect m_Rect;//图片矩形边框
public:
    MyGood(){};
    MyGood(int index,double X,double Y);
};

#endif // MYGOOD_H
