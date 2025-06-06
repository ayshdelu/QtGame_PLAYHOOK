#ifndef WINTIME_H
#define WINTIME_H

#include <QDialog>
#include "Player.h"

namespace Ui {
class WinTime;
}

class WinTime : public QDialog
{
    Q_OBJECT

public:
    explicit WinTime(QWidget *parent = nullptr,Player *player = nullptr);
    ~WinTime();
    Player *player_;
    QGraphicsScene *scene;
    QGraphicsView *view;

    QTimer *toDL;

private:
    Ui::WinTime *ui;

signals:
    void change();
};

#endif // WINTIME_H
