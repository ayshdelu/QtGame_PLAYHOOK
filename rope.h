#ifndef ROPE_H
#define ROPE_H
#include "GameManager.h"
#include <QWidget>
#include <QPoint>
#include <QApplication>
#include <QPainter>
#include <QKeyEvent>
#include <QCursor>
#include <QPixmap>


class Rope : public QWidget {
    Q_OBJECT

public:
    explicit Rope(QWidget *parent = nullptr,GameManager *manager = nullptr);
    bool hasLine = false; //

    void clearDrawing() {
        hasLine = false; // 修改标志位
        this->update(); // 请求重绘
    }


    //局部坐标系
    QPoint startPoint{170,3844}; //绳索起始点<---这两个点都要转换到场景坐标下  绘图使用局部坐标 ， 计算使用场景坐标
    QPoint endPoint; //鼠标位置点
    void Set_rope_end(){  //获取endPoint
        endPoint = mapFromGlobal(QCursor::pos());
    }

    //场景坐标系(实际坐标系)
    QPointF VstartPoint{170,7473}; //绳索起始点
    QPointF VstartPoint_{170,7473}; //副本,用于重启游戏
    QPointF VendPoint; //鼠标位置点

    //绳头
    QTimer *moveTimer; //飞行时长定时器
    QTimer *agTimer; //演算重力加速度
    QTimer *QCDTimer; //Q技能CD
    QTimer *CD; //发射前摇
    QTimer *PT; //受力时长计时器

    QPointF Primehead{170,7473}; //绳头初始坐标,与初始点一致
    qreal tan_x = 0; //起始点与鼠标点的仰角正切值->始终贴近X轴的角
    qreal MaxTime=500; //绳索飞行最长时间
    qreal Ag = 0.1; //绳头自然下落速度
    qreal Vp = 8; //绳头发射初速度
    qreal Vx = 0;
    qreal Vy = 0;
    qreal direction; //抛出方向(四个象限)
    bool On_head; //发射角度90向上
    bool if_launch = true; //能否发射钩锁
    bool if_hasLine = false; //能否触发钩锁
    bool is_Moving = false; //是否正在移动
    bool is_Col = false; //表示绳头是否发生碰撞,碰撞时则停止
    bool QisCD = false;
    void Get_tan_x(QPointF VstartPoint,QPointF VendPoint);
    void Launch_rope(qreal tan_x,qreal Vp); ////发射绳头
    void Check_Col(); //检测能否进行钩锁
    //绳索对角色速度进行作用
    QPointF Tiepoint; //钩锁拉住的点
    qreal Ap = 1; //钩锁给予的拉力总值
    qreal Ax = 0; //钩锁对角色的作用力 ->取决于当前角色与绳头的位置关系
    qreal Ay = 0;
    qreal tan_Ax = 0; //计算受力角
    qreal Adirection;
    void Get_tan_Ax(QPointF VstartPoint,QPointF Tiepoint);
    void Give_Power(qreal tan_Ax); //分配Ax Ay
    bool if_Power; //是否受力
    qreal Powertime = 2000; //受力时长   <------两种结束方式,一种依靠时长,一种依靠松开Q
    qreal Acc_Eff = 50; //加速效率,这个值越小,加速越强

signals:
    void ropeMoving(); //绳头抛出信号
    void agstop();

protected:
    void paintEvent(QPaintEvent *event) override;
    //void keyPressEvent(QKeyEvent *event) override;
private:
    GameManager *manager_;
};

#endif // ROPE_H
