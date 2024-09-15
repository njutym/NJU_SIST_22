#include "mygood.h"

MyGood::MyGood(int index,double X,double Y)
{
    goodindex = index;
    m_object.load(QString(":/resource/good%1.png").arg(goodindex));
    m_object = m_object.scaled(44,44);
    m_Rect.setWidth(m_object.width());
    m_Rect.setHeight(m_object.height());
    m_X = X;
    m_Y = Y;
    m_Rect.moveTo(m_X,m_Y);
}

