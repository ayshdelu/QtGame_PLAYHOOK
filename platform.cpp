#include "platform.h"
#include <QPainter>
#include <QTimer>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>

Platform::Platform(QGraphicsItem *parent) {

    // 设置平台图片
    images.append(QPixmap(":/resources/map/GameMap1.png"));
    images.append(QPixmap(":/resources/map/GameMap2.png"));
    images.append(QPixmap(":/resources/map/GameMap3.png"));
    size = images.size();
    int order[4];
    order[0] = 0;
    order[1] = 1;
    order[2] = 2;
    order[3] = 1;

    blinkTimer = new QTimer(this);
    connect(blinkTimer, &QTimer::timeout, [this, order]() {
        if(number >3)
            {
            number = 0;
        }

        //qDebug()<<number<<order[number];
        setPixmap(images[order[number]]);
        number +=1;

        // setVisible(!isVisible());  // 切换可见性
    });
    blinkTimer->start(500);  // 每500ms切换一次
    setPixmap(images[number]);
}

