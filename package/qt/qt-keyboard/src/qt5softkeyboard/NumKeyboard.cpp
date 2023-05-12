#include "NumKeyboard.h"
#include "ui_NumKeyboard.h"

NumKeyboard::NumKeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NumKeyboard)
{
    ui->setupUi(this);


}

NumKeyboard::~NumKeyboard()
{
    delete ui;
}
