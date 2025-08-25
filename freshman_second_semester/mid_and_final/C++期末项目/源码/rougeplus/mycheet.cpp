#include "mycheet.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "myconfig.h"

MyCheet::MyCheet(QWidget *parent)
    : Scene{parent}
{
    this->setWindowTitle("你小子想作弊！");

    QLabel * ischeetlabel =  new QLabel(this);
    QFont font;
    font.setPointSize(25);
    ischeetlabel->setFont(font);
    ischeetlabel->move(GAME_WIDTH/2-100,100);
    ischeetlabel->setFixedSize(400,100);
    ischeetlabel->setText("您处于未开挂状态");
    ischeetlabel->show();

    QLabel * cuelabel =  new QLabel(this);
    cuelabel->move(GAME_WIDTH/2-350,GAME_HEIGHT/2-100);
    cuelabel->setFixedSize(500,100);
    cuelabel->setText("请输入作弊码(纯数字格式,表示田大王强qq号):");
    cuelabel->show();

    QLineEdit * password = new QLineEdit(this);
    password->move(GAME_WIDTH/2-100,GAME_HEIGHT/2-100);
    password->setFixedSize(400,100);

    QPushButton * affirmbtn = new QPushButton(this);
    affirmbtn->move(GAME_WIDTH/2+200,GAME_HEIGHT/2);
    affirmbtn->setText("确认");
    affirmbtn->show();

    QPushButton * cancelcheetbtn = new QPushButton(this);
    cancelcheetbtn->move(GAME_WIDTH/2+100,GAME_HEIGHT/2);
    cancelcheetbtn->setText("取消开挂");
    cancelcheetbtn->hide();

    connect(affirmbtn,&QPushButton::clicked,this,[=]()
    {
        if(password->text() == "1532606676")
        {
            emit this->docheet();
            ischeetlabel->setText("您已成功开挂！！！");
            cancelcheetbtn->show();
        }
    });

    connect(cancelcheetbtn,&QPushButton::clicked,this,[=]()
    {
        emit this->cancelcheet();
        cancelcheetbtn->hide();
        ischeetlabel->setText("您处于未开挂状态");
    });
}

