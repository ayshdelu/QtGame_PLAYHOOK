#ifndef GAMEINSTRUCTION_H
#define GAMEINSTRUCTION_H

#include <QDialog>

namespace Ui {
class GameInstruction;
}

class GameInstruction : public QDialog
{
    Q_OBJECT

public:
    explicit GameInstruction(QWidget *parent = nullptr);
    ~GameInstruction();
signals:
    void backTomenu();

private:
    Ui::GameInstruction *ui;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAMEINSTRUCTION_H
