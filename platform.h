#ifndef PLATFORM_H
#define PLATFORM_H
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QPainterPath>

class Platform : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Platform(QGraphicsItem *parent = nullptr);

    qreal scale = 0.4; //当前缩放比例
private:
    QTimer *blinkTimer;  // 闪烁动画
    QVector<QPixmap> images; // 图片列表
    int size;
    int number = 0;
};

#endif // PLATFORM_H
