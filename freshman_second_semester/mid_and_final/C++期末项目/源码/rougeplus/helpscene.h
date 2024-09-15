//帮助类
#ifndef HELPSCENE_H
#define HELPSCENE_H

#include "scene.h"

class HelpScene : public Scene//继承Scene类
{
    Q_OBJECT
public:
    explicit HelpScene(QWidget *parent = nullptr);
};

#endif // HELPSCENE_H
