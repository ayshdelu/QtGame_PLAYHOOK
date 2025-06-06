#include "wintime.h"
#include "ui_wintime.h"
#include "Player.h"

WinTime::WinTime(QWidget *parent,Player *player)
    : QDialog(parent),
    player_(player)
    , ui(new Ui::WinTime)
{
    toDL = new QTimer(this);
    ui->setupUi(this);
    this->setWindowTitle("(*^▽^*)");
    QIcon icon(":/resources/player/sans.png");
    if (icon.isNull()) {
        qDebug() << "图标加载失败，请检查路径";
    } else {
        setWindowIcon(icon);
    }
    setFixedSize(366,543);
    scene = new QGraphicsScene(this);  // 创建场景
    scene->setSceneRect(0, 0, 366,543);  // 场景范围
    view = new QGraphicsView(scene, this);  // 将场景绑定到视图
    view->setStyleSheet("background:transparent;");  // 设置背景透明
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 关闭滚动条
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(366,543); // 固定视图大小



    connect(player_, &Player::gamewin, [this](){
        toDL->start(9000);
        player_->Ag = 0;
        player_->Vy = -1;
        player_->x=180;
        player_->y=560;

        scene->addItem(player_);
        view->centerOn(player_);
        player_->setPos(player_->x, player_->y);

        //qDebug()<<"fly";
    });

    connect(toDL,&QTimer::timeout,[this](){
        toDL->stop();
        //qDebug()<<"ischange";
        emit change();
    });
}

WinTime::~WinTime()
{
    delete ui;
}
