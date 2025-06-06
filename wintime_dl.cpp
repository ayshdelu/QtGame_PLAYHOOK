#include "wintime_dl.h"
#include "ui_wintime_dl.h"

wintime_dl::wintime_dl(QWidget *parent,Player *player,WinTime *wintime) :
    QDialog(parent),
    player_(player),
    wintime_(wintime),
    ui(new Ui::wintime_dl)
{
    againTimer = new QTimer(this);
    ui->setupUi(this);
    this->setWindowTitle("See you later...");
    QIcon icon(":/resources/player/sans.png");
    if (icon.isNull()) {
        qDebug() << "图标加载失败，请检查路径";
    } else {
        setWindowIcon(icon);
    }
    setFixedSize(250,476);
    scene = new QGraphicsScene(this);  // 创建场景
    scene->setSceneRect(0, 0, 250,476);  // 场景范围
    view = new QGraphicsView(scene, this);  // 将场景绑定到视图
    view->setStyleSheet("background:transparent;");  // 设置背景透明
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 关闭滚动条
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(250,476); // 固定视图大小


    connect(wintime_, &WinTime::change, [this](){
        //qDebug()<<"getchange";
        againTimer->start(8600);
        player_->x=125;
        player_->y=490;

        scene->addItem(player_);
        view->centerOn(player_);
        player_->setPos(player_->x, player_->y);
    });
    connect(againTimer,&QTimer::timeout,[this](){
        againTimer->stop();
        emit player_->restartGame();
        emit gameagain();
    });
}

wintime_dl::~wintime_dl()
{
    delete ui;
}
