#include "GameManager.h"
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include "QFile"

GameManager::GameManager(QObject* parent)
    : QObject(parent), isGameOver(false), isWin(false) {

    Get_Col(Col_Plat_C,Col_Plat_D);
    //qDebug() << "Col_Plat_C size:" << Col_Plat_C;
}

GameManager::~GameManager() {
    gameTimer->stop();
    delete gameTimer;
}

//初始化坐标信息
void GameManager::Get_Col(QVector<QPointF> &Col_Plat_C, QVector<QPointF> &Col_Plat_D) {
    auto readPoints = [](const QString& filePath, QVector<QPointF>& points) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Failed to open file:" << filePath;
            return;
        }

        QTextStream in(&file);
        QString line = in.readLine(); // 读取第一行（可能是数量）
        line = in.readLine(); // 读取第二行（数据）

        QStringList coords = line.split(' ');
        for (int i = 0; i + 1 < coords.size(); i += 2) {
            bool okX, okY;
            qreal x = coords[i].toDouble(&okX);
            qreal y = coords[i + 1].toDouble(&okY);
            if (okX && okY) {
                points.append(QPointF(x, y));
            } else {
                qWarning() << "Invalid coordinate in file:" << filePath;
            }
        }

        file.close();
    };

    readPoints(":/resources/data/Col_Data_C.txt", Col_Plat_C);
    readPoints(":/resources/data/Col_Data_D.txt", Col_Plat_D);
}

//四种碰撞函数
void GameManager::Col_type(QPointF Col_Player_A, QPointF Col_Player_B,
                           QVector<QPointF> Col_Plat_C, QVector<QPointF> Col_Plat_D)
{
    qreal minOverlap = 0;
    for (int i = 0; i < Col_Plat_C.size(); ++i) {
        if (i >= Col_Plat_D.size()) break;
        QPointF platC = Col_Plat_C[i];
        QPointF platD = Col_Plat_D[i];
        // 玩家矩形的边界
        qreal playerMinX = Col_Player_A.x();
        qreal playerMaxX = Col_Player_B.x();
        qreal playerMinY = Col_Player_A.y();
        qreal playerMaxY = Col_Player_B.y();
        // 平台矩形的边界
        qreal platMinX = platC.x();
        qreal platMaxX = platD.x();
        qreal platMinY = platC.y();
        qreal platMaxY = platD.y();
        // 检测是否相交
        bool overlapX = (playerMinX <= platMaxX && playerMaxX >= platMinX);
        bool overlapY = (playerMinY <= platMaxY && playerMaxY >= platMinY);
        //qDebug()<<overlapX<<"and "<<overlapY;

        if (overlapX && overlapY)
        {
            // 计算各轴的重叠量
            qreal overlapLeft = platMaxX - playerMinX;
            qreal overlapRight = playerMaxX - platMinX;
            qreal overlapTop = platMaxY - playerMinY;
            qreal overlapBottom = playerMaxY - platMinY;
            // 找出最小重叠方向
            qreal minOverlapX = qMin(overlapLeft, overlapRight);
            qreal minOverlapY = qMin(overlapTop, overlapBottom);

            if (minOverlapX < minOverlapY) {
                // X轴方向碰撞（左右）
                if (overlapLeft > overlapRight) {
                    minOverlap=overlapRight;
                    emit Col_L(minOverlap);
                    return;
                } else {
                    minOverlap=overlapLeft;
                    emit Col_R(minOverlap);
                    return;
                }
            } else {
                // Y轴方向碰撞（上下）
                if (overlapTop > overlapBottom) {
                    minOverlap=overlapBottom;
                    emit Col_UP(minOverlap);
                    return;
                } else {
                    minOverlap=overlapTop;
                    emit Col_DW(minOverlap);
                    return;
                }
            }
        }
    }
    emit Col_NO();
    return;
}

//掉出世界
void GameManager::outWorld() {
    QPointF platC = Col_Out_A;
    QPointF platD = Col_Out_B;

    qreal playerMinX = Col_Player_A.x();
    qreal playerMaxX = Col_Player_B.x();
    qreal playerMinY = Col_Player_A.y();
    qreal playerMaxY = Col_Player_B.y();
    // 平台矩形的边界
    qreal platMinX = platC.x();
    qreal platMaxX = platD.x();
    qreal platMinY = platC.y();
    qreal platMaxY = platD.y();
    // 检测是否相交
    bool overlapX = (playerMinX <= platMaxX && playerMaxX >= platMinX);
    bool overlapY = (playerMinY <= platMaxY && playerMaxY >= platMinY);
    //qDebug()<<overlapX<<" "<<overlapY;

    if (overlapX && overlapY)
    {
        emit Col_OUT();
    }
}


//获得胜利
void GameManager::Game_Win() {

    QPointF platC = Col_Win_A;
    QPointF platD = Col_Win_B;

    qreal playerMinX = Col_Player_A.x();
    qreal playerMaxX = Col_Player_B.x();
    qreal playerMinY = Col_Player_A.y();
    qreal playerMaxY = Col_Player_B.y();
    // 平台矩形的边界
    qreal platMinX = platC.x();
    qreal platMaxX = platD.x();
    qreal platMinY = platC.y();
    qreal platMaxY = platD.y();
    // 检测是否相交
    bool overlapX = (playerMinX <= platMaxX && playerMaxX >= platMinX);
    bool overlapY = (playerMinY <= platMaxY && playerMaxY >= platMinY);

    if (overlapX && overlapY)
    {
        emit Col_WIN();
    }
};

//结算
void GameManager::Game_Fly() {

    //qDebug()<<"000";
    QPointF platC = Col_FLY_A;
    QPointF platD = Col_FLY_A;

    qreal playerMinX = Col_Player_A.x();
    qreal playerMaxX = Col_Player_B.x();
    qreal playerMinY = Col_Player_A.y();
    qreal playerMaxY = Col_Player_B.y();
    // 平台矩形的边界
    qreal platMinX = platC.x();
    qreal platMaxX = platD.x();
    qreal platMinY = platC.y();
    qreal platMaxY = platD.y();
    // 检测是否相交
    bool overlapX = (playerMinX <= platMaxX && playerMaxX >= platMinX);
    bool overlapY = (playerMinY <= platMaxY && playerMaxY >= platMinY);

    //qDebug()<<overlapX<<" "<<overlapY;
    if (overlapY)
    {
        emit Col_Fly();
    }
};

