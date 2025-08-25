//障碍物类
#ifndef MYOBSTACLE_H
#define MYOBSTACLE_H

#include <QMainWindow>

class MyObstacle
{
    friend class PlayScene;//友元申明
    friend class Enemy;//友元申明
    friend class EliteEnemy;//友元申明
private:
    //坐标
    double m_X;
    double m_Y;
    int obstacleindex;//障碍物种类
    QPixmap m_object;//资源对象
    QRect m_Rect;//图片矩形边框
public:
    MyObstacle(){};
    MyObstacle(int index,double X,double Y);
};

#endif // MYOBSTACLE_H
