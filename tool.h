#ifndef TOOL_H
#define TOOL_H
#include <QWidget>
#include <QPoint>
#include <QGraphicsView>
#include <QGraphicsScene>

class Tool:public QWidget
{
    Q_OBJECT

public:
    explicit Tool(QWidget *parent = nullptr,QGraphicsView* view = nullptr); // 显式声明构造函数

signals:
    void selectionFinished( QPointF& topLeft, QPointF& bottomRight);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    //widget坐标下的点,仅仅用来绘图,因为QPainter 在 QWidget 上绘图时，必须使用 局部坐标
    QPoint startPoint;
    QPoint currentPoint;
    QPoint endPoint;
    //从wiget坐标系下转换到QGview坐标系下对应的点
    QPointF VstartPoint;
    QPointF VcurrentPoint;
    QPointF VendPoint;
    bool isSelecting;
    QGraphicsView* view_; // 保存对 QGraphicsView 的引用
};

#endif // TOOL_H
