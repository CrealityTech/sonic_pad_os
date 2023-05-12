#include "NormalKeyboard.h"
#include "ui_NormalKeyboard.h"
#include <QtDebug>

NormalKeyboard::NormalKeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NormalKeyboard),
    modifier(Qt::ShiftModifier)
{
    ui->setupUi(this);
    letterkeys[ui->A] = Qt::Key_A;
    letterkeys[ui->B] = Qt::Key_B;
    letterkeys[ui->C] = Qt::Key_C;
    letterkeys[ui->D] = Qt::Key_D;
    letterkeys[ui->E] = Qt::Key_E;
    letterkeys[ui->F] = Qt::Key_F;
    letterkeys[ui->G] = Qt::Key_G;
    letterkeys[ui->H] = Qt::Key_H;
    letterkeys[ui->I] = Qt::Key_I;
    letterkeys[ui->J] = Qt::Key_J;
    letterkeys[ui->K] = Qt::Key_K;
    letterkeys[ui->L] = Qt::Key_L;
    letterkeys[ui->M] = Qt::Key_M;
    letterkeys[ui->N] = Qt::Key_N;
    letterkeys[ui->O] = Qt::Key_O;
    letterkeys[ui->P] = Qt::Key_P;
    letterkeys[ui->Q] = Qt::Key_Q;
    letterkeys[ui->R] = Qt::Key_R;
    letterkeys[ui->S] = Qt::Key_S;
    letterkeys[ui->T] = Qt::Key_T;
    letterkeys[ui->U] = Qt::Key_U;
    letterkeys[ui->V] = Qt::Key_V;
    letterkeys[ui->W] = Qt::Key_W;
    letterkeys[ui->X] = Qt::Key_X;
    letterkeys[ui->Y] = Qt::Key_Y;
    letterkeys[ui->Z] = Qt::Key_Z;

    letterkeys[ui->enter] = Qt::Key_Enter;
    letterkeys[ui->shiftl] = Qt::Key_Shift;
    letterkeys[ui->shiftr] = Qt::Key_Shift;
    letterkeys[ui->space] = Qt::Key_Space;
    letterkeys[ui->backspace] = Qt::Key_Backspace;
    letterkeys[ui->period] = Qt::Key_Period;
    letterkeys[ui->apostrophe] = Qt::Key_Apostrophe;
    letterkeys[ui->minus] = Qt::Key_Minus;
    letterkeys[ui->at] = Qt::Key_At;

    ui->groupLetter->addButton(ui->backspace);
    ui->groupLetter->addButton(ui->enter);
    ui->groupLetter->addButton(ui->shiftl);
    ui->groupLetter->addButton(ui->shiftr);
    ui->groupLetter->addButton(ui->space);
    ui->groupLetter->addButton(ui->period);
    ui->groupLetter->addButton(ui->apostrophe);
    ui->groupLetter->addButton(ui->minus);
    ui->groupLetter->addButton(ui->at);

    connect(ui->language,&QPushButton::clicked,this,&NormalKeyboard::changeLanguage);
    connect(ui->symbol,&QPushButton::clicked,this,&NormalKeyboard::changeSymbol);
    connect(ui->groupLetter,QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),this,&NormalKeyboard::letterClicked);
    connect(ui->hide,&QPushButton::clicked,this,&NormalKeyboard::hideKeyboard);

    ui->shiftr->setIcon(QPixmap(":/images/shift_checked.png"));
    ui->shiftl->setIcon(QPixmap(":/images/shift_checked.png"));
}

NormalKeyboard::~NormalKeyboard()
{
    delete ui;
}

void NormalKeyboard::setCurLanguage(const QString &lang)
{
    ui->space->setText(lang);
}

void NormalKeyboard::letterClicked(QAbstractButton* button)
{
    if(letterkeys.contains(button))
    {
        if(letterkeys[button] == Qt::Key_Shift)
        {
            if(modifier == Qt::NoModifier)
                modifier = Qt::ShiftModifier;
            else if(modifier == Qt::ShiftModifier)
                modifier = Qt::NoModifier;

            if(modifier == Qt::ShiftModifier)
            {
                ui->shiftr->setIcon(QPixmap(":/images/shift_checked.png"));
                ui->shiftl->setIcon(QPixmap(":/images/shift_checked.png"));
                capsLock();
            }
            else
            {
                ui->shiftr->setIcon(QPixmap(":/images/shift.png"));
                ui->shiftl->setIcon(QPixmap(":/images/shift.png"));
                capsLock();
            }

            return;
        }


        if(modifier == Qt::ShiftModifier)
        {
            keyPressed(letterkeys[button],buttonText(button).toUpper(),Qt::NoModifier);

            ui->shiftr->setIcon(QPixmap(":/images/shift.png"));
            ui->shiftl->setIcon(QPixmap(":/images/shift.png"));
            modifier = Qt::NoModifier;
            capsLock();
        }
        else
        {
            keyPressed(letterkeys[button],buttonText(button).toLower(),Qt::NoModifier);
        }
    }
}

QString NormalKeyboard::buttonText(QAbstractButton * button)
{
    if(letterkeys[button] == Qt::Key_Space)
        return QLatin1String(" ");
    else if(letterkeys[button] == Qt::Key_Enter)
        return QLatin1String();
    else
        return button->text();

}

void NormalKeyboard::capsLock()
{
    if(modifier == Qt::ShiftModifier)
    {
        ui->A->setText("A");
        ui->B->setText("B");
        ui->C->setText("C");
        ui->D->setText("D");
        ui->E->setText("E");
        ui->F->setText("F");
        ui->G->setText("G");
        ui->H->setText("H");
        ui->I->setText("I");
        ui->J->setText("J");
        ui->K->setText("K");
        ui->L->setText("L");
        ui->M->setText("M");
        ui->N->setText("N");
        ui->O->setText("O");
        ui->P->setText("P");
        ui->Q->setText("Q");
        ui->R->setText("R");
        ui->S->setText("S");
        ui->T->setText("T");
        ui->U->setText("U");
        ui->V->setText("V");
        ui->W->setText("W");
        ui->X->setText("X");
        ui->Y->setText("Y");
        ui->Z->setText("Z");
    }
    else
    {
        ui->A->setText("a");
        ui->B->setText("b");
        ui->C->setText("c");
        ui->D->setText("d");
        ui->E->setText("e");
        ui->F->setText("f");
        ui->G->setText("g");
        ui->H->setText("h");
        ui->I->setText("i");
        ui->J->setText("j");
        ui->K->setText("k");
        ui->L->setText("l");
        ui->M->setText("m");
        ui->N->setText("n");
        ui->O->setText("o");
        ui->P->setText("p");
        ui->Q->setText("q");
        ui->R->setText("r");
        ui->S->setText("s");
        ui->T->setText("t");
        ui->U->setText("u");
        ui->V->setText("v");
        ui->W->setText("w");
        ui->X->setText("x");
        ui->Y->setText("y");
        ui->Z->setText("z");
    }

}
