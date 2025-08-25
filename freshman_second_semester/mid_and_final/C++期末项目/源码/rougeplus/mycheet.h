//作弊类
#ifndef MYCHEET_H
#define MYCHEET_H

#include "scene.h"

class MyCheet : public Scene
{
    Q_OBJECT
public:
    explicit MyCheet(QWidget *parent = nullptr);
signals:
    void docheet();
    void cancelcheet();
};

#endif // MYCHEET_H
