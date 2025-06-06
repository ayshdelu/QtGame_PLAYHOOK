#include "widget.h"
#include "./ui_widget.h"
#include <QVBoxLayout>
#include "Player.h"
#include "platform.h"
#include <QUrl>

Widget::~Widget()
{
    delete ui;
}

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    setFixedSize(800, 600);  //强制窗口大小

    //ui->setupUi(this);
    this->setWindowTitle(" : ' Just Do it ... '");
    QIcon icon(":/resources/player/speed1.png");
    if (icon.isNull()) {
        qDebug() << "图标加载失败，请检查路径";
    } else {
        setWindowIcon(icon);
    }

    scene = new QGraphicsScene(this);  // 创建场景

    scene->setSceneRect(0, 0, 5124, 10388);  // 场景范围

    view = new QGraphicsView(scene, this);  // 将场景绑定到视图
    view->setFixedSize(800,600); // 固定视图大小
    view->setRenderHint(QPainter::Antialiasing);  // 抗锯齿渲染
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);  // 优化性能
    view->setDragMode(QGraphicsView::ScrollHandDrag);  // 可选：允许拖动查看

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setLayout(new QVBoxLayout);  // 设置布局
    layout()->addWidget(view);  // 将视图添加到主窗口

    Platform *platform = new Platform();  // <----------创建自定义平台对象

    platform->setScale(platform->scale);  // 图片缩小为0.2
    platform->setPos(0,0);  // 设置图片位置为 (0, 0)
    scene->addItem(platform);

    // QPointF cameraPos(1200,3780);   //视野测试<---固定摄像机
    // view->centerOn(cameraPos);

    GameManager *manager = new GameManager();  //<------------创建对象manager
    //manager->show(manager->Col_Plat_D);

    rope = new Rope(this,manager);  //<----------创建角色技能对象
    rope->setGeometry(0, -3480, 5124, 10388);
    //rope->show();



    player = new Player(nullptr,this,manager,rope);  // <--------创建玩家对象
    player->setGameView(view); // 将 view 传给 player
    //  主动设置焦点,焦点是用来接收按键的控件
    player->setFocus();

    player->setScale(player->scale); //缩小

    scene->addItem(player);//添加角色

    connect(player,&Player::restartGame,[this](){
        //qDebug()<<"is add";
        scene->addItem(player);
    });


    //鼠标框选工具tool
    // Tool *boxSelector = new Tool(this,view);

    // boxSelector->setGeometry(0, -3480, 5124, 10388);
    // boxSelector->show();

    // connect(boxSelector, &Tool::selectionFinished,
    //         [manager](QPointF topLeft,QPointF bottomRight) {
    //             qDebug() << "左上角：" << topLeft;
    //             qDebug() << "右下角：" << bottomRight;

    //             //<---因为误删文件,下面函数已失效--->
    //             // manager->Set_Plat_Col(topLeft,manager->Col_Plat_C);
    //             // manager->Set_Plat_Col(bottomRight,manager->Col_Plat_D);
    //             //保存到文件:
    //             //manager->savePointsToFile_C(manager->Col_Plat_C,manager->FILENAME_C);    //<--------------开关设置
    //             //manager->savePointsToFile_D(manager->Col_Plat_D,manager->FILENAME_D);
    //         });

}
