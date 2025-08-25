//自定义个性化按钮类
#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
private:
    QString normalImgPath;
    QString pressImgPath;
public:
    MyPushButton(QString normalImg,QString pressImg = "");
    void zoom1();
    void zoom2();
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // MYPUSHBUTTON_H
