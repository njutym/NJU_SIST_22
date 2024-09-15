//本项目由田大王强独立自主完成，从学习Qt到项目完工共计25天

//以下为参考教程和文献和资源

//资源来源： 百度图片、爱给网游戏素材
//原视频链接：https://www.bilibili.com/video/BV1g4411H78N //传智教育bilibliQt教程
//原文链接：https://blog.csdn.net/itcast_cn/article/details/104251531 //黑马程序员官方某飞机大战教程
//原文链接：https://blog.csdn.net/Jerich2213/article/details/127816451//Jerich morningStar实现自定义丝滑键盘事件教程
//原文链接：https:blog.csdn.net/weixin_42411702/article/details/123510939 //赵斌韬Qsetting类教程
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


