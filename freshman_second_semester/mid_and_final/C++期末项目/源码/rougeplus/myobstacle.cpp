#include "myobstacle.h"

MyObstacle::MyObstacle(int index,double X,double Y)
{
    obstacleindex = index;
    switch(index)
    {
    case 1:
        m_object.load(":/resource/obstacle1.png");
        m_object = m_object.scaled(88,88);
        break;
    case 2:
        m_object.load(":/resource/obstacle2.png");
        m_object = m_object.scaled(200,200);
        break;
    case 3:
        m_object.load(":/resource/obstacle3.png");
        m_object = m_object.scaled(88,88);
        break;
    case 4:
        m_object.load(":/resource/obstacle4.png");
        m_object = m_object.scaled(88,88);
        break;
    case 5:
        m_object.load(":/resource/obstacle5.png");
        m_object = m_object.scaled(44,44);
        break;
    case 6:
        m_object.load(":/resource/obstacle6.png");
        m_object = m_object.scaled(44,44);
        break;
    case 7:
        m_object.load(":/resource/obstacle7.png");
        m_object = m_object.scaled(270,70);
        break;
    case 8:
        m_object.load(":/resource/bossdefend.png");
        m_object = m_object.scaled(200,200);
    default:
        return;
    }
    if(index == 6)
    {
        m_Rect.setWidth(m_object.width()-20);
        m_Rect.setHeight(15);
    }
    else if(index == 5)
    {
        m_Rect.setWidth(m_object.width()-35);
        m_Rect.setHeight(m_object.height()-30);
    }
    else
    {
        m_Rect.setWidth(m_object.width()-20);
        m_Rect.setHeight(m_object.height()-20);
    }
    m_X = X;
    m_Y = Y;
    m_Rect.moveTo(m_X,m_Y);
}

