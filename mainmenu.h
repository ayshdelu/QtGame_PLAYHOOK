#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>


namespace Ui {
class mainmenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

signals:
    void gameStart(); // 游戏开始信号
    void Instruction(); //游戏说明
    void Quit(); //退出游戏

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

    bool open_game = false;

private slots:
    void on_gamestart_clicked();

    void on_instruction_clicked();

    void on_quit_clicked();

private:
    Ui::mainmenu *ui;
};

#endif // MAINMENU_H

