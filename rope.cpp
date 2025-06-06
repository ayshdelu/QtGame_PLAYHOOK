#include "rope.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QtMath>
#include <qtimer.h>

Rope::Rope(QWidget *parent,GameManager *manager) : QWidget(parent), manager_(manager){
    // 设置自定义鼠标样式
    //QPixmap cursorPixmap("D:\\QT_project\\QGame_Apex\\resours\\picture\\cursor.png"); // 光标图片路径
    //QCursor ropeCursor(cursorPixmap, 67, 62); // (0,0) 是光标的热点位置->热点位置就是实际点击时生效的位置
    //setCursor(ropeCursor);
    //Primehead{170,7473}; //绳头初始坐标,与初始点一致
    tan_x = 0; //起始点与鼠标点的仰角正切值
    MaxTime = 1400; //绳索飞行最长时间
    Ag = 0.09; //绳头自然下落加速度
    Ap = 1.6;
    Vp = 6.6; //绳头发射初速度
    Vx = 0;
    Vy = 0;
    if_hasLine = false;
    hasLine = false;
    bool On_head = false; //发射角度90向上

    //<----初始化定时器
    moveTimer = new QTimer(this);
    agTimer = new QTimer(this);
    CD = new QTimer(this);
    agTimer -> start(16);

    //<----rope相关接收器
    connect(this,&Rope::ropeMoving,[this](){  //处理绳头速度,反应到update函数中实时更新
    //qDebug()<<"rope Vx: <--------------------"<<this->Vx;
    is_Moving = true;
    });

    connect(agTimer,&QTimer::timeout,[this](){
        //qDebug()<<"AG:"<<this->Ag;
        if(is_Moving == true){

        connect(this,&Rope::agstop,[this](){
            this->Ag = 0;
        });
        this->Vy += this->Ag;  //让rope_Vy受到重力加速度
        //qDebug()<<"rope Vy: "<<this->Vy;
        }
        else return;

    });

    connect(moveTimer, &QTimer::timeout,[this](){  //接收rope的绳头飞行时长信号 结束飞行
        //emit agstop();
        //qDebug()<<"moveTimer ";
        hasLine = false;
        is_Moving = false;
        // Primehead = VstartPoint;
        // Tiepoint = VstartPoint;
        moveTimer->stop();
    });

    connect(CD,&QTimer::timeout,[this](){
        if_launch = true;
        CD->stop();
    });
}

void Rope::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    if (hasLine) {
        QPainter painter(this);
        painter.setPen(QPen(QColor(210, 145, 50), 4)); // 设置为棕褐色
        painter.drawLine(startPoint, endPoint);
    }
}

void Rope::Check_Col(){
    int size = manager_->Col_Plat_C.size();
    for (int i = 0; i < size; ++i)
    {
        if(Primehead.x()>=manager_->Col_Plat_C[i].x() && Primehead.x()<=manager_->Col_Plat_D[i].x()
            && Primehead.y()>=manager_->Col_Plat_C[i].y() && Primehead.y()<=manager_->Col_Plat_D[i].y())
        {
            if_hasLine = true; //表示可以钩锁
            is_Col = true; //表示绳头碰撞
        }
    }
}


void Rope::Launch_rope(qreal tan_x,qreal Vp)   //<-----------绳头速度计算
{
    if(if_launch == false || QisCD == true) return; //CD时不能发射
    //qDebug()<<"Vp:"<<Vp;
    if(direction == 0){ //正上方抛出
        Vx=0;
        Vy=-Vp;
    }
    else if(direction == 1){
        Vx = qSqrt((Vp*Vp)/(1+tan_x*tan_x));
        Vy = -(Vx*tan_x);
    }
    else if(direction == 2){
        Vx = -qSqrt((Vp*Vp)/(1+tan_x*tan_x));
        Vy = Vx*tan_x;
    }
    else if(direction == 3){
        Vx = -qSqrt((Vp*Vp)/(1+tan_x*tan_x));
        Vy = -(Vx*tan_x);
    }
    else{
        Vx = qSqrt((Vp*Vp)/(1+tan_x*tan_x));
        Vy = Vx*tan_x;
    }
    //qDebug()<<"LLL Vx:"<<Vx<<" Vy:"<<Vy;
    emit ropeMoving(); //发射信号
    moveTimer -> start(MaxTime); //钩锁飞出时长
    CD ->start(100);  //内置钩锁CD
    if_launch = false;
}

void Rope::Get_tan_x(QPointF VstartPoint,QPointF VendPoint){  //tan_x计算函数
    if(VendPoint.y()<=VstartPoint.y() && VendPoint.x()>VstartPoint.x()) //一象限
    {
        tan_x = (VstartPoint.y()-VendPoint.y())/(VendPoint.x()-VstartPoint.x());
        direction = 1;
    }
    else if(VendPoint.y()<=VstartPoint.y() && VendPoint.x()<VstartPoint.x()) //二象限
    {
        tan_x = (VstartPoint.y()-VendPoint.y())/(VstartPoint.x()-VendPoint.x());
        direction = 2;
    }
    else if(VendPoint.y()>=VstartPoint.y() && VendPoint.x()<VstartPoint.x()) //三象限
    {
        tan_x = (VendPoint.y()-VstartPoint.y())/(VstartPoint.x()-VendPoint.x());
        direction = 3;
    }
    else if(VendPoint.y()>=VstartPoint.y() && VendPoint.x()>VstartPoint.x())  //四象限
    {
        tan_x = (VendPoint.y()-VstartPoint.y())/(VendPoint.x()-VstartPoint.x());
        direction = 4;
    }
    else if(VendPoint.y()>VstartPoint.y() && VendPoint.y()==VstartPoint.y())  //正上方
    {
        On_head = true;
        direction = 0;
    }
    //qDebug()<<"tanx:"<<tan_x;
}

void Rope::Give_Power(qreal tan_Ax)    //<------------绳头给予力加速度计算
{
    if(Adirection == 0){ //正上方抛出
        Ax=0;
        Ay=-Ap;
    }
    else if(Adirection == 1){
        Ax = qSqrt((Ap*Ap)/(1+tan_Ax*tan_Ax)) ;  //1-> 1.5
        Ay = -(Ax*tan_Ax);
    }
    else if(Adirection == 2){
        Ax = -qSqrt((Ap*Ap)/(1+tan_Ax*tan_Ax)) ;
        Ay = Ax*tan_Ax;
    }
    else if(Adirection == 3){
        Ax = -qSqrt((Ap*Ap)/(1+tan_Ax*tan_Ax));
        Ay = -(Ax*tan_Ax);
    }
    else if(Adirection == 4){
        Ax = qSqrt((Ap*Ap)/(1+tan_Ax*tan_Ax));
        Ay = Ax*tan_Ax;
    }
    else if(Adirection == 5)
    {
        Ax = 0;
        Ay = 0;
    }
    //qDebug()<<"now is Adirect:"<<Adirection;
}

void Rope::Get_tan_Ax(QPointF VstartPoint,QPointF Tiepoint){  //tan_x计算函数 VendPoint
    //if(is_Col == false) return ;

    //qDebug()<<"Vstar:"<<VstartPoint<<"Tie:"<<Tiepoint;  //??为什么左右移动时计算结果会变成0？
    if(Tiepoint.y()<=VstartPoint.y() && Tiepoint.x()>VstartPoint.x()) //一象限
    {
        tan_Ax = (VstartPoint.y()-Tiepoint.y())/(Tiepoint.x()-VstartPoint.x());
        Adirection = 1;
    }
    else if(Tiepoint.y()<=VstartPoint.y() && Tiepoint.x()<VstartPoint.x()) //二象限
    {
        tan_Ax = (VstartPoint.y()-Tiepoint.y())/(VstartPoint.x()-Tiepoint.x());
        Adirection = 2;
    }
    else if(Tiepoint.y()>=VstartPoint.y() && Tiepoint.x()<VstartPoint.x()) //三象限
    {
        tan_Ax = (Tiepoint.y()-VstartPoint.y())/(VstartPoint.x()-Tiepoint.x());
        Adirection = 3;
    }
    else if(Tiepoint.y()>=VstartPoint.y() && Tiepoint.x()>VstartPoint.x())  //四象限
    {
        tan_Ax = (Tiepoint.y()-VstartPoint.y())/(Tiepoint.x()-VstartPoint.x());
        Adirection = 4;
    }
    else if(Tiepoint.y()>VstartPoint.y() && Tiepoint.y()==VstartPoint.y())  //正上方
    {
        Adirection = 0;
    }
    else if(Tiepoint == VstartPoint)
    {
        Adirection = 5;
    }
    //qDebug()<<"tanAx:"<<tan_Ax<<"Vstar:"<<VstartPoint.y()<<"Tie:"<<Tiepoint.y()<<"Adirec:"<<Adirection;
    //按下Q时 tie.y短时间内等于vstar.y 导致tan_Ax = 0
}


