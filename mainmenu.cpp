#include "mainmenu.h"
#include "ui_mainmenu.h"


MainMenu::MainMenu(QWidget *parent) :QDialog(parent),ui(new Ui::mainmenu)
{
    ui->setupUi(this); // 加载UI界面
    this->setWindowTitle("主菜单");
    QIcon icon(":/resources/map/GameMap6.png");
    if (icon.isNull()) {
        qDebug() << "图标加载失败，请检查路径";
    } else {
        setWindowIcon(icon);
    }

    setFixedSize(250, 476);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_gamestart_clicked()
{
    emit gameStart(); // 发射信号
    //qDebug() << "Game Start clicked!";
    open_game = true;
}

void MainMenu::on_instruction_clicked()
{
    emit Instruction();
}



void MainMenu::on_quit_clicked()
{
    emit Quit();
}

