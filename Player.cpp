#include "Player.h"
#include <QKeyEvent>
#include <QDateTime>
#include <QTimer>
#include <QPainter>
#include <qgraphicsview.h>
#include "GameManager.h"
#include "rope.h"
#include <QMessageBox>
#include <QProcess>


Player::Player(QGraphicsItem *parent,QWidget* Qwidget,GameManager* manager,Rope *rope):
    QGraphicsPixmapItem(parent),Qwidget_(Qwidget),manager_(manager),rope_(rope) {


    // 加载角色初始图像
    setPixmap(QPixmap(":/resources/player/speed1.png"));

    // 初始化速度
    Vx = 0.0; //Vx Vy 有正负表示方向
    Vy = 0.0;
    Ax = 1.0;
    Ay = 1.0;
    Ag = 0.2; //<---------整个运动中只能在一处添加重力加速度,我设置在了connect NO信号中
    Ag_=Ag;
    Vs = -5.0;//跳跃初速度
    prime_Vs = Vs;
    Af = 0.5; //当水平速度不为0,且当前碰撞信号为up时,就让Vx趋向于0
    Vmx = 4.0;
    Vmy = 5.0;  //最大下落速度

    x = 140.0;
    x_ =x;
    y = 3960.0;
    y_ =y;
    FLY = false; //飞行模式


    // 设置定时器以定期更新位置      <------------各种定时器
    colTimer = new QTimer(this);
    moveTimer = new QTimer(this);
    accelerationTimer = new QTimer(this);
    spaceTimer = new QTimer(this);
    Rope_acc = new QTimer(this);
    rope_->QCDTimer = new QTimer(this);
    Wintimer = new QTimer(this);

    // 启动移动定时器（每 16ms 更新一次位置，约 60 FPS）
    connect(moveTimer, &QTimer::timeout, this, &Player::updatePosition);
    connect(colTimer, &QTimer::timeout, this, &Player::updateCol);
    moveTimer->start(16);
    colTimer->start(12);        //<---------- 碰撞定时器频率设置

    // 按下按键时,启动加速定时器（每 100ms 检查一次按键状态）
    connect(accelerationTimer, &QTimer::timeout, [this]() {   //水平加速
        if (is_Moving && is_A)
        {
            Vx -= Ax; // 逐步增加速度
            if (Vx <= -Vmx) {
                    Vx = -Vmx;
            }
        }
        if (is_Moving && is_D)
        {
            Vx += Ax; // 逐步增加速度
            if (Vx >= Vmx) {
                Vx = Vmx;
            }
        }
    });

    connect(Rope_acc,&QTimer::timeout,[this](){  //当按下Q时,每100ms进行一次rope加速,直到松开Q或者抵达作用最大时长
        if(rope_->if_Power == true) //给予加速度
        {
            Vx += rope_->Ax;
            Vy += rope_->Ay;
            //qDebug()<<"doing";   //
        }
    });

    //跳跃瞬间忽略UP信号定时器,避免触发信号Vy被重置为0
    connect(spaceTimer, &QTimer::timeout, [this](){
        if_ignore_Col_UP = false; // 50ms 后允许 Col_UP 响应
        //qDebug()<<"now is stop UP information <------";
        spaceTimer->stop();
    });

    //启动碰撞信号接收逻辑                   <----------------------碰撞信号接收器
    connect(manager_, &GameManager::Col_L,[this](qreal overlapX){//L信号

        Vx=0;
        is_Fall = true;
        is_Jumping = true; //不允许蹬墙跳

        L_isCol =true;
        R_isCol =false;
        UP_isCol=false;
        DW_isCol=false;
        NO_isCol=false;
        // 角色在平台左侧，向左推离
        //qDebug()<<overlapX;
        this->x -= overlapX;
        manager_->Col_Player_A.setX(manager_->Col_Player_A.x() - overlapX);
        manager_->Col_Player_B.setX(manager_->Col_Player_B.x() - overlapX);
        rope_->VstartPoint.setX(rope_->VstartPoint.x() - overlapX);

        //qDebug()<<"CoL L is doing";

    });

    connect(manager_, &GameManager::Col_R,[this](qreal overlapX){ //R信号
        Vx=0;
        is_Fall = true;
        is_Jumping = true;

        L_isCol =false;
        R_isCol =true;
        UP_isCol=false;
        DW_isCol=false;
        NO_isCol=true;
        //角色在平台右侧,向右推离
        //qDebug()<<overlapX;
        this->x += overlapX;
        manager_->Col_Player_A.setX(manager_->Col_Player_A.x() + overlapX);
        manager_->Col_Player_B.setX(manager_->Col_Player_B.x() + overlapX);
        rope_->VstartPoint.setX(rope_->VstartPoint.x() + overlapX);
        //qDebug()<<"CoL R is doing";

    });

    connect(manager_, &GameManager::Col_UP,[this](qreal overlapY){ //保存连接
        if(if_ignore_Col_UP == true) {return;}  //UP信号
        Vy=0;
        is_Fall = false; //只有这种情况下不自由落体
        is_Jumping = false; //false状态下允许跳跃

        L_isCol =false;
        R_isCol =false;
        UP_isCol=true;
        DW_isCol=false;
        NO_isCol=false;
        // 角色在平台上侧，向上推离
        //qDebug()<<overlapY;
        this->y -= overlapY;
        manager_->Col_Player_A.setY(manager_->Col_Player_A.y() - overlapY);
        manager_->Col_Player_B.setY(manager_->Col_Player_B.y() - overlapY);
        rope_->VstartPoint.setY(rope_->VstartPoint.y() - overlapY);

        //qDebug()<<"CoL UP is doing";

    });

    connect(manager_, &GameManager::Col_DW,[this](qreal overlapY){ //DW信号
        Vy=0;
        is_Fall = true;
        is_Jumping = true;

        L_isCol =false;
        R_isCol =false;
        UP_isCol=false;
        DW_isCol=true;
        NO_isCol=false;
        // 角色在平台下侧，向下推离
        //qDebug()<<overlapY;
        this->y += overlapY;
        manager_->Col_Player_A.setY(manager_->Col_Player_A.y() + overlapY);
        manager_->Col_Player_B.setY(manager_->Col_Player_B.y() + overlapY);
        rope_->VstartPoint.setY(rope_->VstartPoint.y() + overlapY);

        //qDebug()<<"CoL DW is doing";
    });

    connect(manager_, &GameManager::Col_NO,[this](){     //接收NO信号

        is_Fall = true;
        is_Jumping = true;

        L_isCol =false;
        R_isCol =false;
        UP_isCol=false;
        DW_isCol=false;
        NO_isCol=true;
        //qDebug()<<"Col no";
    });

    connect(manager_,&GameManager::Col_OUT,[this](){   //接收掉出信号

        int choice = QMessageBox::question(nullptr,"Game Over :D","再来一次？",QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::Yes); // 默认按钮

        if(choice == QMessageBox::Yes){
           emit restartGame(); //将所有值回调为初始状态
        }
        else{
            emit restartGame();
            emit backTomenu();
        }
    });

    connect(manager_,&GameManager::Col_WIN,[this](){ //胜利
        //qDebug()<<"win";
        Vy = -0.8;
        Ag = -0.01; //原地飞升
    });

    connect(manager_,&GameManager::Col_Fly,[this](){ //结算
        //qDebug()<<"over";
        emit gamewin();
    });


    connect(rope_->QCDTimer,&QTimer::timeout,[this](){
        rope_->QisCD = false;
        rope_->QCDTimer ->stop();
    });

    connect(this,&Player::restartGame,[this](){ //重启游戏-初始化相关数据
        //qDebug()<<"is restart";
        Vx = 0;
        Vy = 0;
        Ag = Ag_;
        x = x_;
        y =y_;
        manager_->Col_Player_A = manager_->Col_Player_A_;
        manager_->Col_Player_B = manager_->Col_Player_B_;
        rope_->VstartPoint = rope_->VstartPoint_;
    });

};

void Player::updateCol(){        //<<---------------------------启动检验碰撞定时器
    //manager_->call();

    //检测碰撞并发出四种信号
    manager_->Col_type(manager_->Col_Player_A,manager_->Col_Player_B,manager_->Col_Plat_C,manager_->Col_Plat_D);
}

void Player::keyPressEvent(QKeyEvent *event) {  //按下逻辑 -----------------
    manager_->Col_type(manager_->Col_Player_A, manager_->Col_Player_B, manager_->Col_Plat_C, manager_->Col_Plat_D); //立即检测一次碰撞关系

    //qDebug() << "按下按键：" << event->key(); //<----切换飞行模式
    // if(event->key() == Qt::Key_P){
    //     if(FLY == true)
    //     FLY = false;
    //     else{
    //         FLY = true;
    //     }
    // }

    if (event->key() == Qt::Key_Escape) {
        if (event->isAutoRepeat()) return;
        emit backTomenu();
    }

    if (event->key() == Qt::Key_Q) {     //<------------技能
        if (event->isAutoRepeat()) return;
        if (rope_->QisCD == true) return;
        press_Q = true;
        rope_->hasLine = true ;

        QPoint globalPos = QCursor::pos();            // 获取全局坐标
        QPoint localPos = Qwidget_->mapFromGlobal(globalPos);   // 转为当前控件的局部坐标
        rope_->VendPoint = gameView->mapToScene(localPos); //设置好rope的VendPoint
        rope_->VendPoint.setY(rope_->VendPoint.y()+3482);

        rope_->Get_tan_x(rope_->VstartPoint,rope_->VendPoint);
        rope_->Launch_rope(rope_->tan_x,rope_->Vp); //调用发射函数->开启定时&发射信号

        Rope_acc->start(rope_->Acc_Eff);  //<----这个值也会作用到rope加速大小

        rope_->update(); // 触发 Rope 的重绘
    }

    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D ||
        event->key() == Qt::Key_Space ) {
        //记录方向,更新currentDirection

        if (event->key() == Qt::Key_A) {
            if (event->isAutoRepeat()) return; // 忽略自动重复事件   !!!
            is_A = true;
            is_Moving = true;
            currentDirection = Qt::Key_A;
        }
        else if (event->key() == Qt::Key_D) {
            if (event->isAutoRepeat()) return;
            is_D = true;
            is_Moving = true;
            currentDirection = Qt::Key_D;
        }
        else if (event->key() == Qt::Key_Space) {
            if (event->isAutoRepeat()) return;
            is_space = true;

            currentDirection = Qt::Key_Space;
        }
        if (event->key() == Qt::Key_Space && is_Jumping==false) {
            if (event->isAutoRepeat()) return;
            manager_->Col_type(manager_->Col_Player_A, manager_->Col_Player_B, manager_->Col_Plat_C, manager_->Col_Plat_D);
            Vy += Vs;
            is_Fall = true;
            is_Jumping = true; //不能连续跳跃
            is_space = true;
            //qDebug()<<"now is jumping and Vy is:"<<Vy;

            if_ignore_Col_UP = true; //<---短时间内忽略上碰撞,避免Vy被重置为0
            spaceTimer->start(50); // 50ms 后允许 Col_UP 响应
        }
        pressStartTime = QDateTime::currentMSecsSinceEpoch();  // 记录按下时间->储存到pressStartTime中
        //is_Moving = true;
        accelerationTimer->start(100); // 启动加速定时器
    }
    if(FLY == true)  //飞行模式
    {
        if(event->key() == Qt::Key_Left)
        {
            x -= 25;
            //qDebug()<<"this is l";
            manager_->Col_Player_A.setX(manager_->Col_Player_A.x() - 25);
            manager_->Col_Player_B.setX(manager_->Col_Player_B.x() - 25);
            rope_->VstartPoint.setX(rope_->VstartPoint.x() - 25);
        }
        if(event->key() == Qt::Key_Right)
        {
            x += 25;
            manager_->Col_Player_A.setX(manager_->Col_Player_A.x() + 25);
            manager_->Col_Player_B.setX(manager_->Col_Player_B.x() + 25);
            rope_->VstartPoint.setX(rope_->VstartPoint.x() + 25);
        }
        if(event->key() == Qt::Key_Up)
        {
            y -= 50;
            manager_->Col_Player_A.setY(manager_->Col_Player_A.y() - 50);
            manager_->Col_Player_B.setY(manager_->Col_Player_B.y() - 50);
            rope_->VstartPoint.setY(rope_->VstartPoint.y() - 50);
        }
        if(event->key() == Qt::Key_Down)
        {
            y += 50;
            manager_->Col_Player_A.setY(manager_->Col_Player_A.y() + 50);
            manager_->Col_Player_B.setY(manager_->Col_Player_B.y() + 50);
            rope_->VstartPoint.setY(rope_->VstartPoint.y() + 50);
        }
    }
}

void Player::keyReleaseEvent(QKeyEvent *event) {  //松开逻辑 -----------------
    manager_->Col_type(manager_->Col_Player_A, manager_->Col_Player_B, manager_->Col_Plat_C, manager_->Col_Plat_D); //立即检测一次碰撞关系
    //qDebug() << "释放按键：" << event->key();
    if (event->key() == Qt::Key_A) {
        if (event->isAutoRepeat()) return;
        is_A = false;
    }
    if (event->key() == Qt::Key_D) {
        if (event->isAutoRepeat()) return;

        is_D = false;
    }

    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D ) {
        if (event->isAutoRepeat()) return; // 忽略自动重复事件 !!!!!!!!!!!!
        //qint64 duration = QDateTime::currentMSecsSinceEpoch() - pressStartTime;  //持续时间为duration
        is_Moving = false;
        accelerationTimer->stop(); // 停止加速定时器

        //Vx = 0.0; // 重置速度  <---------可以改为缓速减0

        currentDirection = Qt::Key_unknown; // 重置按键方向!!!!!!!!!!
    }

    if(event->key() == Qt::Key_Q)
    {
        if (event->isAutoRepeat()) return;

        //qDebug()<<"overing";
        rope_->is_Moving = false; //重置钩锁状态
        rope_->hasLine = false;
        rope_->if_Power = false;

        if(rope_->QisCD == false && press_Q == true){
        rope_->QisCD = true;
        rope_->QCDTimer->start(400);
        }
        Rope_acc -> stop();
        press_Q=false;
    }
}

void Player::setGameView(QGraphicsView *view) {  //获取view的方法
    gameView = view;
}

void Player::updatePosition() {   //这个函数通过不断检测速度来更新图像位置<-----------------updatePosition()自动更新物理量
    change_color(); //player类中的角色换色函数
    //qDebug() << "x:" << x << "y:" << y << "Vx:" << Vx << "currentDirection:" << currentDirection;

    // 启用焦点策略
    setFlag(QGraphicsItem::ItemIsFocusable, true); //  启用焦点能力
    setFocus(); //  显式获取焦点

    manager_->outWorld(); //检测是否掉出
    manager_->Game_Win();  //检测是否胜利
    manager_->Game_Fly(); //检查是否可以结算

    //setPixmap(QPixmap("D:\\QT_project\\QGame_Apex\\resours\\picture\\speed_1.png"));

    if(is_Fall == true && FLY ==false){
        Vy += Ag; //    重力加速度
    }

    if(Vx != 0)      //<--------x轴移动
    {
        x += Vx;
        //qDebug()<<"Vx is:"<<Vx;
        manager_->Col_Player_A.setX(manager_->Col_Player_A.x() + Vx);
        manager_->Col_Player_B.setX(manager_->Col_Player_B.x() + Vx);
        rope_->VstartPoint.setX(rope_->VstartPoint.x() + Vx);
        //摩擦力加速度
        if((UP_isCol == true && is_Moving == false)||
            (UP_isCol == true &&is_Moving == true &&
            currentDirection==Qt::Key_A && Vx>0)||
            (UP_isCol == true &&is_Moving == true &&
            currentDirection==Qt::Key_D && Vx<0)){   //在主动移动且移动方向与速度方向相同时不启用制动力
            if((Vx < Af && Vx>0) || (Vx>-Af && Vx<0)){Vx=0;} //震荡清0
            if(Vx >0){
                Vx -= Af;
            }
            if(Vx <0){
                Vx +=Af;
            }
        }
    }

    if(Vy != 0)    //<---------y轴移动    UP_isCol == false || is_space == true
    {
        //qDebug()<<"Vy is:"<<Vy;
        y += Vy;  //重力位移
        manager_->Col_Player_A.setY(manager_->Col_Player_A.y() + Vy);
        manager_->Col_Player_B.setY(manager_->Col_Player_B.y() + Vy);
        rope_->VstartPoint.setY(rope_->VstartPoint.y() + Vy);
    }

    //qDebug()<<"x:"<<manager_->Col_Player_A.x()<<"y:"<<manager_->Col_Player_A.y(); //验证发现角色碰撞边框位移无误


    if (gameView) {
        gameView->centerOn(x,y-40); // 跟随角色的摄像机 <---y-40,让视野中心偏上方
    }
    // else{
    //     gameView->centerOn(x,4100);
    // }



    //设置rope相关值 <-----------
    //qDebug()<<rope_->startPoint.y();
    rope_->startPoint.setX(rope_->VstartPoint.x()); //设置startPoint点
    if(rope_->startPoint.x()>=423 )  //在屏幕边缘时固定起始点
    {
        rope_->startPoint.setX(423);
    }
    if(rope_->VstartPoint.y() <= 3844)
    {
        // rope_->startPoint.setY(3845);
        rope_->startPoint.setY(rope_->VstartPoint.y());
    }
    else{
        rope_->startPoint.setY(3844);
    }

    //rope_->Set_rope_end(); //设置endPoint点

    //qDebug()<<"Vstart"<<rope_->VstartPoint<<"Vend:"<<rope_->VendPoint;

    //绳头位移
    if(rope_->is_Col == false && rope_->is_Moving == true)
    {
        if(rope_->Vx!=0)
        {
            rope_->Primehead.setX(rope_->Primehead.x()+rope_->Vx);
        }

        if(rope_->Vy!=0)
        {
            rope_->Primehead.setY(rope_->Primehead.y()+rope_->Vy);
        }
    }
    //qDebug()<<"Primehead"<<rope_->Primehead;  //跟踪绳头
    rope_->endPoint = gameView->mapFromScene(rope_->Primehead);
    //qDebug()<<"end:"<<rope_->endPoint;
    rope_->Check_Col(); //检查能否钩锁  返回if_hasLine=true
    if(rope_->if_hasLine == true)  //验证抛射终点能否触发钩锁
    {
        rope_->if_hasLine = false;   //重置为false
        rope_->if_Power = true; //给予受力
        rope_->Tiepoint=rope_->Primehead;  //将碰撞点的Primehead值给Tiepoint
        //qDebug()<<"Tie"<<rope_->Tiepoint;
    }
    //让Tiepoint在绳头发生碰撞前一直跟着VstarPoint
    if(rope_->is_Col == false)
    {
        rope_->Tiepoint = rope_->VstartPoint;
    }
    //收回钩锁
    if(rope_->is_Moving == false){
        rope_->Primehead = rope_->VstartPoint;
        rope_->is_Col = false;
    }
    //qDebug()<<"Vstar"<<rope_->VstartPoint<<"Tie"<<rope_->Tiepoint<<"Pri"<<rope_->Primehead;
    rope_->Get_tan_Ax(rope_->VstartPoint,rope_->Tiepoint); //计算tan_Ax
    //qDebug()<<rope_->tan_Ax;
    rope_->Give_Power(rope_->tan_Ax); //设置Ax Ay
    //qDebug()<<"rAx:"<<rope_->Ax<<"rAy:"<<rope_->Ay<<"  Vx :"<<Vx<<"Vy :"<<Vy;
    //qDebug()<<"Vx :"<<Vx<<"Vy :"<<Vy;


    this->setPos(x, y);
    //qDebug()<<x<<" and "<<y;
    //qDebug()<<":"<<manager_->Col_Player_A<<"    "<<manager_->Col_Player_B;
    // 更新角色位置到界面上
    scene()->update();  // 强制更新场景
}





