#include "KeyboardContainer.h"
#include <QVBoxLayout>
#include <QtDebug>
#include <QPropertyAnimation>
#include <QApplication>
#include <QScreen>

KeyboardContainer::KeyboardContainer(QWidget *parent) : QWidget(parent)
{
	normalKeyboard = new NormalKeyboard(this);
	symbolKeyboard = new SymbolKeyboard(this);
	textDisplayWidget = new CandidatesListWidget(this);
	textDisplayWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	textDisplayWidget->setMinimumSize(400, 30);
	textDisplayWidget->resize(600, 30);

	mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(textDisplayWidget);

	QWidget * widgetContainer = new QWidget(this);
	bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(normalKeyboard);
	bottomLayout->addWidget(symbolKeyboard);
	bottomLayout->setMargin(0);
	widgetContainer->setLayout(bottomLayout);
	widgetContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	mainLayout->addWidget(widgetContainer);

	//mainLayout->setStretch(0, 0);
	mainLayout->setStretch(1, 1);
	mainLayout->setSpacing(5);
	mainLayout->setContentsMargins(5, 5, 5, 5);

	symbolKeyboard->hide();

        //将KeyboardContainer设置为模态窗口会导致下面的语句不起作用,虚拟键盘会抢输入框焦点
        //setWindowModality(Qt::WindowModal);
	setWindowFlags(windowFlags() | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);

    setStyleSheet("*{font-size: 18pt;font-family: 'Microsoft YaHei';outline: none} \
    QWidget\
    {\
        color: #eff0f1;\
        background-color: rgb(0,0,0);\
        selection-background-color:#3daee9;\
        selection-color: #eff0f1;\
        background-clip: border;\
        border-image: none;\
        border: 0px transparent black;\
        outline: 0;\
    }\
    QPushButton\
    {\
        color:rgb(255,255,255);\
        background-color:rgb(56,53,51);\
        border: none;\
        border-radius: 5px;\
    }\
    QPushButton:hover:pressed\
    {\
        color:rgb(255,255,255);\
        background-color:rgb(45,125,154);\
    }\
    QPushButton:hover:!pressed\
    {\
        color:rgb(255,255,255);\
        background-color:rgb(56,53,51);\
    }\
    QPushButton:disabled\
    {\
        color:rgb(120,120,120);\
        background-color:rgb(20,20,20);\
    }");

    textDisplayWidget->setStyleSheet("*{font-size: 18pt;font-family: 'Microsoft YaHei';outline: none} \
    QPushButton\
    {\
        color:rgb(255,255,255);\
        background-color:rgb(56,53,51);\
        border: none;\
        border-radius: 0px;\
    }\
    QPushButton:hover:pressed\
    {\
        color:rgb(255,255,255);\
        background-color:rgb(45,125,154);\
    }\
    QPushButton:hover:!pressed\
    {\
        color:rgb(255,255,255);\
        background-color:rgb(42,40,38);\
    }\
    QPushButton:disabled\
    {\
        color:rgb(120,120,120);\
        background-color:rgb(20,20,20);\
    }");

    connect(normalKeyboard, &NormalKeyboard::changeLanguage, this, &KeyboardContainer::changeLanguage);
    connect(symbolKeyboard, &SymbolKeyboard::changeLanguage, this, &KeyboardContainer::changeLanguage);
    connect(normalKeyboard, &NormalKeyboard::changeSymbol, this, &KeyboardContainer::onChangeSymbol);
    connect(symbolKeyboard, &SymbolKeyboard::changeSymbol, this, &KeyboardContainer::onChangeSymbol);

	connect(normalKeyboard, &NormalKeyboard::hideKeyboard, this, &KeyboardContainer::hideKeyboard);
    connect(symbolKeyboard, &SymbolKeyboard::hideKeyboard, this, &KeyboardContainer::hideKeyboard);

    connect(normalKeyboard, &NormalKeyboard::keyPressed, this, &KeyboardContainer::keyPressed);
    connect(symbolKeyboard, &SymbolKeyboard::keyPressed, this, &KeyboardContainer::keyPressed);
    connect(textDisplayWidget,&CandidatesListWidget::chooseText,this,&KeyboardContainer::chooseCandidate);

    animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(300);
    connect(animation,&QAbstractAnimation::finished,this,&KeyboardContainer::onAnimationFinished);
    m_hiding = false;
}

KeyboardContainer::~KeyboardContainer()
{
    if(isAnimating())
        animation->stop();
}

bool KeyboardContainer::isAnimating() const
{
    return animation->state() == QAbstractAnimation::Running;
}

void KeyboardContainer::setCandidateList(const QStringList &texts)
{
    textDisplayWidget->setCandidatesList(texts);
}

void KeyboardContainer::setLanguageName(const QString &name)
{
    normalKeyboard->setCurLanguage(name);
    symbolKeyboard->setCurLanguage(name);
}

void KeyboardContainer::onChangeSymbol()
{
    if(normalKeyboard->isVisible())
    {
        normalKeyboard->hide();
        symbolKeyboard->show();
    }
    else {
        normalKeyboard->show();
        symbolKeyboard->hide();
    }

    emit changeSymbol();
}

void KeyboardContainer::onHideSymbol()
{
    if(!normalKeyboard->isVisible())
    {
        normalKeyboard->show();
        symbolKeyboard->hide();
    }
}

void KeyboardContainer::animationHide()
{
    int screenHeight = qApp->primaryScreen()->size().height();

    if(isAnimating())
        animation->stop();

    m_hiding = true;

    animation->setStartValue(QPoint(pos().x(),pos().y()));
    animation->setEndValue(QPoint(pos().x(),screenHeight));
    animation->start();
}

void KeyboardContainer::animationShow()
{
    move(this->pos().x(),qApp->primaryScreen()->size().height());

    show();

    int screenHeight = qApp->primaryScreen()->size().height();

    animation->setStartValue(QPoint(pos().x(),screenHeight));
    animation->setEndValue(QPoint(pos().x(),screenHeight - height()));
    animation->start();
}

void KeyboardContainer::setInputMethodHints(Qt::InputMethodHints hints)
{
    Q_UNUSED(hints);
}

void KeyboardContainer::onAnimationFinished()
{
    if(m_hiding)
        hide();

    m_hiding = false;
}
