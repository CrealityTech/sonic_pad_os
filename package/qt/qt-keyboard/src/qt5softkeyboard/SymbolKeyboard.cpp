#include "SymbolKeyboard.h"
#include "ui_SymbolKeyboard.h"

SymbolKeyboard::SymbolKeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolKeyboard)
{
    ui->setupUi(this);

    curPage = 0;
    maxPage = 2;

    page1.append({Qt::Key_1,"1"});
    page1.append({Qt::Key_2,"2"});
    page1.append({Qt::Key_3,"3"});
    page1.append({Qt::Key_4,"4"});
    page1.append({Qt::Key_5,"5"});
    page1.append({Qt::Key_6,"6"});
    page1.append({Qt::Key_7,"7"});
    page1.append({Qt::Key_8,"8"});
    page1.append({Qt::Key_9,"9"});
    page1.append({Qt::Key_0,"0"});

    page1.append({Qt::Key_At,"@"});
    page1.append({Qt::Key_NumberSign,"#"});
    page1.append({Qt::Key_Percent,"%"});
    page1.append({Qt::Key_Ampersand,"&&"});
    page1.append({Qt::Key_Asterisk,"*"});
    page1.append({Qt::Key_Minus,"-"});
    page1.append({Qt::Key_Plus,"+"});
    page1.append({Qt::Key_ParenLeft,"("});
    page1.append({Qt::Key_ParenRight,")"});

    page1.append({Qt::Key_Exclam,"!"});
    page1.append({Qt::Key_QuoteDbl,"\""});
    page1.append({Qt::Key_Less,"<"});
    page1.append({Qt::Key_Greater,">"});
    page1.append({Qt::Key_Apostrophe,"'"});
    page1.append({Qt::Key_Colon,":"});
    page1.append({Qt::Key_Semicolon,";"});
    page1.append({Qt::Key_Slash,"/"});
    page1.append({Qt::Key_Question,"?"});
    page1.append({Qt::Key_Period,"."});
    page1.append({0xE000, ":-)"});

    ////////////////////////////////////////////////////
    page1.append({Qt::Key_AsciiTilde,"~"});
    page1.append({Qt::Key_Agrave,"`"});
    page1.append({Qt::Key_Bar,"|"});
    page1.append({0x7B,"·"});
    page1.append({0x221A,"√"});
    page1.append({Qt::Key_division,"÷"});
    page1.append({Qt::Key_multiply,"×"});
    page1.append({Qt::Key_onehalf,"½"});
    page1.append({Qt::Key_BraceLeft,"{"});
    page1.append({Qt::Key_BraceRight,"}"});


    page1.append({Qt::Key_Dollar,"$"});
    page1.append({0x20AC,"€"});
    page1.append({0xC2,"£"});
    page1.append({0xA2,"¢"});
    page1.append({0xA5,"¥"});
    page1.append({Qt::Key_Equal,"="});
    page1.append({Qt::Key_section,"§"});
    page1.append({Qt::Key_BracketLeft,"["});
    page1.append({Qt::Key_BracketRight,"]"});

    page1.append({Qt::Key_Underscore,"_"});
    page1.append({0x2122,"™"});
    page1.append({0x00AE,"®"});
    page1.append({Qt::Key_guillemotleft,"«"});
    page1.append({Qt::Key_guillemotright,"»"});
    page1.append({0x201C,"“"});
    page1.append({0x201D,"”"});
    page1.append({Qt::Key_Backslash,"\\"});
    page1.append({Qt::Key_AsciiCircum,"^"});
    page1.append({0x2026,"\u2026"});
    page1.append({0xE000, ":-)"});

    symbolButtons.append(ui->btn1);
    symbolButtons.append(ui->btn2);
    symbolButtons.append(ui->btn3);
    symbolButtons.append(ui->btn4);
    symbolButtons.append(ui->btn5);
    symbolButtons.append(ui->btn6);
    symbolButtons.append(ui->btn7);
    symbolButtons.append(ui->btn8);
    symbolButtons.append(ui->btn9);
    symbolButtons.append(ui->btn10);
    symbolButtons.append(ui->btn11);
    symbolButtons.append(ui->btn12);
    symbolButtons.append(ui->btn13);
    symbolButtons.append(ui->btn14);
    symbolButtons.append(ui->btn15);
    symbolButtons.append(ui->btn16);
    symbolButtons.append(ui->btn17);
    symbolButtons.append(ui->btn18);
    symbolButtons.append(ui->btn19);
    symbolButtons.append(ui->btn20);
    symbolButtons.append(ui->btn21);
    symbolButtons.append(ui->btn22);
    symbolButtons.append(ui->btn23);
    symbolButtons.append(ui->btn24);
    symbolButtons.append(ui->btn25);
    symbolButtons.append(ui->btn26);
    symbolButtons.append(ui->btn27);
    symbolButtons.append(ui->btn28);
    symbolButtons.append(ui->btn29);
    symbolButtons.append(ui->btn30);

    ui->buttonGroup->addButton(ui->backspace);
    ui->buttonGroup->addButton(ui->enter);
    ui->buttonGroup->addButton(ui->space);


    keyMap[ui->space] = Qt::Key_Space;
    keyMap[ui->enter] = Qt::Key_Enter;
    keyMap[ui->backspace] = Qt::Key_Backspace;

    buttonRemap();

    connect(ui->language,&QPushButton::clicked,this,&SymbolKeyboard::changeLanguage);
    connect(ui->symbol,&QPushButton::clicked,this,&SymbolKeyboard::changeSymbol);
    connect(ui->buttonGroup,QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this,&SymbolKeyboard::onSymbolButtonClicked);

    connect(ui->pagel,&QPushButton::clicked,this,&SymbolKeyboard::onChangePage);
    connect(ui->pager,&QPushButton::clicked,this,&SymbolKeyboard::onChangePage);
    connect(ui->hide,&QPushButton::clicked,this,&SymbolKeyboard::hideKeyboard);
}

SymbolKeyboard::~SymbolKeyboard()
{
    delete ui;
}

void SymbolKeyboard::setCurLanguage(const QString &lang)
{
    ui->space->setText(lang);
}

void SymbolKeyboard::onChangePage()
{
    ++curPage;
    curPage %= maxPage;

    buttonRemap();
}

void SymbolKeyboard::onSymbolButtonClicked(QAbstractButton * button)
{
    keyPressed(Qt::Key(keyMap[button]),buttonText(button),Qt::NoModifier);
}

void SymbolKeyboard::buttonRemap()
{
    int start = curPage * symbolButtons.size();
    for(int i = start; i < (curPage + 1) * symbolButtons.size() && i < page1.size(); ++i)
    {
        symbolButtons[i - start]->setText(page1[i].second);
        keyMap[symbolButtons[i - start]] = page1[i].first;
    }

    ui->pagel->setText(QString("%1/%2").arg(curPage + 1).arg(maxPage));
    ui->pager->setText(QString("%1/%2").arg(curPage + 1).arg(maxPage));
}

QString SymbolKeyboard::buttonText(QAbstractButton * button)
{
    if(keyMap[button] == Qt::Key_Space)
    {
        return QLatin1String(" ");
    }
    else if(keyMap[button] == Qt::Key_Ampersand)
    {
        return QLatin1String("&");
    }
    else if(keyMap[button] == Qt::Key_Enter)
    {
        return QLatin1String();
    }
    else
    {
        return button->text();
    }
}
