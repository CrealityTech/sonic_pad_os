#ifndef NUMKEYBOARD_H
#define NUMKEYBOARD_H


#include <QWidget>

namespace Ui {
class NumKeyboard;
}

class NumKeyboard : public QWidget
{
    Q_OBJECT

public:
    explicit NumKeyboard(QWidget *parent = nullptr);
    ~NumKeyboard();

private:
    Ui::NumKeyboard *ui;
};

#endif // NUMKEYBOARD_H
