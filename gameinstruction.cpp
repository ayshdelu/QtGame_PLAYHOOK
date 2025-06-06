#include "gameinstruction.h"
#include "ui_gameinstruction.h"
#include <QKeyEvent>

GameInstruction::GameInstruction(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GameInstruction)
{
    ui->setupUi(this);
    this->setWindowTitle("游戏说明");
    QIcon icon(":/resources/map/GameMap6.png");
    if (icon.isNull()) {
        qDebug() << "图标加载失败，请检查路径";
    } else {
        setWindowIcon(icon);
    }
    setFixedSize(259,400);
}

GameInstruction::~GameInstruction()
{
    delete ui;
}

void GameInstruction::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        if (event->isAutoRepeat()) return;
        emit backTomenu();
    }
}
