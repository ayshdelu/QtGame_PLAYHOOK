#ifndef PLAYER_H
#define PLAYER_H

#include "GameManager.h"
#include "rope.h"
#include "tool.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QKeyEvent>

class QGraphicsView;  // 前置声明 QGraphicsView 类型

class Player : public QObject, public QGraphicsPixmapItem {

    Q_OBJECT
public:
    Player(QGraphicsItem *parent = nullptr,QWidget* Qwidget=nullptr,GameManager* manager=nullptr,Rope* rope=nullptr);

    void move();
    bool FLY = false; //飞行模式 用上下左右操控

    void change_color() // 更换颜色
    {
        if(rope_->QisCD == true || rope_->hasLine == true || press_Q == true)
        {
            setPixmap(QPixmap(":/resources/player/speed4.png"));
            return; //CD状态,无法发射钩锁
        }
        setPixmap(QPixmap(":/resources/player/speed1.png"));
    }

protected:
    void keyPressEvent(QKeyEvent *event) override;  //按键按下
    void keyReleaseEvent(QKeyEvent *event) override;  //按键松开

private slots:
    void updatePosition(); // 定时器驱动的位置更新
    void updateCol();

public:
    QWidget* Qwidget_; // 保存对 QWidget 的引用
    Rope* rope_;                     //技能类
    GameManager* manager_;         // 成员变量保存 manager
    QGraphicsView *gameView; // 保存 view 指针
    Tool *boxSelector;
    //wintime_dl *WDL_;

    QTimer *spaceTimer;          //腾空时间->这段时间内关闭Vy=0的碰撞检测
    QTimer *colTimer;            //控制角色碰撞的定时器
    QTimer *moveTimer;          // 控制角色移动的定时器
    QTimer *accelerationTimer;  // 控制加速的定时器
    QTimer *Rope_acc;           //手控绳子加速器
    QTimer *Wintimer;            //胜利

    qreal Vx; // 水平速度
    qreal Vmx; //水平最大速度 ??或许不需要
    qreal Vmy;
    qreal Vy; // 垂直速度
    qreal Vs; //跳跃加速度
    qreal prime_Vs ; //记录原跳跃速度
    qreal Ax; //水平加速度
    qreal Ay; //垂直加速度
    qreal Ag; //重力加速度
    qreal Af; //摩擦力加速度
    qreal space_high; //跳跃高度
    qint64 pressStartTime;  //按键按下时间戳
    Qt::Key currentDirection; //当前按键方向
    QPointF playerPos; //角色位置  <-----------

    qreal x_;  //副本,重启游戏时使用
    qreal y_;
    qreal Ag_;


    qreal x,y;  //图片的x,y相对坐标
    qreal scale = 0.5; //当前缩放比例
    QPointF cameraPos;  //摄像机位置

    bool is_Jumping=false; //是否跳跃 & 是否腾空
    bool is_Moving=false; //是否移动?
    bool is_Died; //是否死亡
    bool is_Fall; //是否自由落体

    bool L_isCol=false;  //四种碰撞类型
    bool R_isCol=false;
    bool UP_isCol=false;
    bool DW_isCol=false;
    bool NO_isCol=false;

    bool if_ignore_Col_UP = false; //是否忽略Col_UP信号

    void setGameView(QGraphicsView *);  //获取view函数
    bool is_D=false;
    bool is_A=false;
    bool is_space=false;

    bool press_Q = false;
signals:
    void backTomenu(); // 返回菜单
    void restartGame(); //重新开始游戏
    void gamewin(); //游戏结束
};

#endif // PLAYER_H
