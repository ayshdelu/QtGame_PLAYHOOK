#include "tool.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

Tool::Tool(QWidget *parent,QGraphicsView* view) : QWidget(parent),view_(view), isSelecting(false)
{
    setMouseTracking(true); // 启用鼠标移动追踪
}

void Tool::mousePressEvent(QMouseEvent *event)  //<----按下鼠标
{
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();

        VstartPoint = view_->mapToScene(event->pos());

        //qDebug()<<" Vst is:"<<VstartPoint;

        isSelecting = true;
        update(); // 触发重绘
    }
}

void Tool::mouseMoveEvent(QMouseEvent *event)
{
    if (isSelecting && view_) {
        //QPointF VcurrentPoint = view_->mapToScene(event->pos());
        currentPoint = event->pos();//globalPosition();
        update(); // 触发重绘
    }
}

void Tool::mouseReleaseEvent(QMouseEvent *event) //<----释放鼠标
{
    if (event->button() == Qt::LeftButton && isSelecting && view_) {
        isSelecting = false;
        endPoint = event->pos();
        VendPoint = view_->mapToScene(event->pos());

        // 计算左上角和右下角
        QPointF topLeft = VstartPoint;
        QPointF bottomRight = VendPoint;
        if (VstartPoint.x() > VendPoint.x()) std::swap(topLeft.rx(), bottomRight.rx());
        if (VstartPoint.y() > VendPoint.y()) std::swap(topLeft.ry(), bottomRight.ry());

        emit selectionFinished(topLeft, bottomRight);
        update(); // 清除绘制的矩形框
    }
}

void Tool::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (isSelecting && startPoint != currentPoint) {
        QPainter painter(this);
        painter.setPen(Qt::red);
        painter.drawRect(QRectF(startPoint, currentPoint));
    }
}
