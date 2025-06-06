#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <QObject>
#include <QRectF>
#include <QTimer>

class GameManager : public QObject {
    Q_OBJECT

public:
    explicit GameManager(QObject* parent = nullptr);
    ~GameManager();

    void Set_Plat_Col();
    void Get_Col(QVector<QPointF> &Col_Plat_C,QVector<QPointF> &Col_Plat_D);

    // 碰撞区域存储
    QPointF Col_Player_A{153,7453};//7454
    QPointF Col_Player_B{186,7486};//7485
    //碰撞区域副本,用于重启游戏
    QPointF Col_Player_A_{153,7453};
    QPointF Col_Player_B_{186,7486};
    QVector<QPointF> Col_Plat_C;
    QVector<QPointF> Col_Plat_D;

    QPointF Col_Out_A{-10000,7900};  //掉出世界
    QPointF Col_Out_B{10000,8100};

    QPointF Col_Win_A{1838,3652};  //获胜
    QPointF Col_Win_B{1921,3701};

    QPointF Col_FLY_A{-10000,3400};  //结算
    QPointF Col_FLY_B{10000,3500};


    // 游戏状态相关方法
    void checkGameOver();
    void checkWinCondition();
    void outWorld(QRectF playerA,  QRectF playerB,  QRectF worldBoundary);
    void Game_Win(QRectF playerA,  QRectF& playerB, QRectF& winAreaA,  QRectF& winAreaB);

signals:
    // 碰撞信号
    void Col_L(qreal overlapX);      // 左侧碰撞
    void Col_R(qreal overlapX);      // 右侧碰撞
    void Col_UP(qreal overlapY);     // 顶部碰撞
    void Col_DW(qreal overlapY);     // 底部碰撞
    void Col_NO();                   // 无碰撞
    void Col_OUT();                  // 掉出屏幕
    void Col_WIN();                  // 胜利条件
    void Col_Fly();                  // 结算

public:
    QTimer* gameTimer;               // 主游戏循环定时器
    bool isGameOver;                 // 游戏结束标志
    bool isWin;                      // 胜利标志

    void Col_type(QPointF playerA,QPointF playerB, QVector<QPointF> Col_Plat_C,QVector<QPointF> Col_Plat_D);
    void outWorld();   //掉出
    void Game_Win();   //胜利
    void Game_Fly();   //结算

};
#endif // GAMEMANAGER_H
