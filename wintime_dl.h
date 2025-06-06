#ifndef WINTIME_DL_H
#define WINTIME_DL_H

#include "Player.h"
#include "wintime.h"
#include <QDialog>

namespace Ui {
class wintime_dl;
}

class wintime_dl : public QDialog
{
    Q_OBJECT

public:
    explicit wintime_dl(QWidget *parent = nullptr,Player *player = nullptr,WinTime *wintime = nullptr);
    ~wintime_dl();
    Player *player_;
    WinTime *wintime_;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTimer *againTimer;

private:
    Ui::wintime_dl *ui;
signals:
    void gameagain();
};

#endif // WINTIME_DL_H
