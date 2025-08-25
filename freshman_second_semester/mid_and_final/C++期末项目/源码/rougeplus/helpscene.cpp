#include "helpscene.h"

#include <QLabel>
#include <QFont>

HelpScene::HelpScene(QWidget *parent)
    : Scene{parent}
{
    QLabel *label = new QLabel(this);
    label->setFixedSize(1500,1000);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    QString str("                                       欢迎游玩田大王强自主开发的roguelike(类幸存者)游戏！！！\n"
                "\n\n\n基本操作:WSAD或者5213进行上下左右移动,J使用炸弹,K使用人物技能\n"
                "获胜条件:坚持存活170s并且打败出现的Boss\n"
                "失败条件:玩家血量在打败Boss前小于等于0\n"
                "支持局内强化系统(各种掉落物和经验设定)\n"
                "支持全局强化系统(商店购买永久强化物品)\n"
                "支持关闭游戏自动存档\n"
                "支持自主修改存档读入(Control Parameter文件)\n"
                "支持作弊\n"
                "bilibili视频演示:         \n"
                "联系作者:1532606676@qq.com\n"
                "制作不易,欢迎游玩");
    label->setText(str);
    label->move(190,-280);
}

