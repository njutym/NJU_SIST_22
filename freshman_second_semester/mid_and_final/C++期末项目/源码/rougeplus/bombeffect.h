//爆炸特效类
#ifndef BOMBEFFECT_H
#define BOMBEFFECT_H

#include <QPixmap>
#include <QVector>

#define BOMB_PATH  ":/resource/bomb-%1.png"   //爆炸资源图片
#define BOMB_NUM  20     //爆炸数量
#define BOMB_MAX  7      //爆炸图片最大索引
#define BOMB_INTERVAL 20   //爆炸切图时间间隔

class Bombeffect
{
    friend class PlayScene;//友元申明
private:
    //爆炸位置
    int m_X;
    int m_Y;
    //爆炸状态
    bool m_Free;
    //爆炸切图的时间间隔
    int m_Recoder;
    //爆炸时加载的图片下标
    int m_index;
public:
    Bombeffect();
    //更新信息（播放图片下标、播放间隔）
    void updateInfo();
    //放爆炸资源数组
    QVector<QPixmap> m_pixArr;
};

#endif // BOMBEFFECT_H
