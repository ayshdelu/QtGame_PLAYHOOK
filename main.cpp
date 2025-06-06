#include "widget.h"
#include "mainmenu.h"
#include "gameinstruction.h"
#include "wintime.h"
#include "Player.h"
#include "wintime_dl.h"
#include <QApplication>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainMenu m;//主菜单
    m.show();
    GameInstruction I; //游戏说明
    Widget* w = new Widget(); //游戏窗口
    WinTime WIN(nullptr,w->player); //胜利结算
    wintime_dl WDL(nullptr,w->player,&WIN); //dl


    //  开始游戏
    QObject::connect(&m, &MainMenu::gameStart, w, &Widget::show);
    QObject::connect(&m, &MainMenu::gameStart, &m, &MainMenu::close);
    // 游戏说明
    QObject::connect(&m, &MainMenu::Instruction, &m, &MainMenu::close);
    QObject::connect(&m, &MainMenu::Instruction, &I, &GameInstruction::show);
    //退出游戏
    QObject::connect(&m, &MainMenu::Quit,[](){
        qDebug() << "正在退出游戏...";
        QApplication::quit();
    });
    // 游戏内返回主菜单
    QObject::connect(w->player, &Player::backTomenu, &m, &MainMenu::show);
    QObject::connect(w->player, &Player::backTomenu, w, &Widget::close);
    //说明栏返回主菜单
    QObject::connect(&I, &GameInstruction::backTomenu, &I, &GameInstruction::close);
    QObject::connect(&I, &GameInstruction::backTomenu, &m, &MainMenu::show);
    //胜利结算
    QObject::connect(w->player, &Player::gamewin, w, &Widget::close);
    QObject::connect(w->player, &Player::gamewin, &WIN, &WinTime::show);
    QObject::connect(&WIN, &WinTime::change,&WDL, &wintime_dl::show);
    QObject::connect(&WIN, &WinTime::change,&WIN, &WinTime::close);
    //通关重启
    QObject::connect(&WDL, &wintime_dl::gameagain,&WDL, &wintime_dl::close);
    QObject::connect(&WDL, &wintime_dl::gameagain, &m, &MainMenu::show);
    QObject::connect(&WDL, &wintime_dl::gameagain, [&](){
        emit w->player->restartGame();
    });




    return a.exec();
}
