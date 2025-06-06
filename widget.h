#ifndef WIDGET_H
#define WIDGET_H


#include "Player.h"
#include "tool.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
//#include "GameManager.h"
//extern GameManager* manager; //对象允许调用声明
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    //GameManager manager;

    Ui::Widget *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    Tool *boxSelector;
    Player *player;
    Rope *rope;
};

#endif // WIDGET_H
