#include "CandidatesListWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QDebug>
#include <QtWidgets>

CandidatesListWidget::CandidatesListWidget(QWidget *parent) : QWidget(parent)
{
    lastPage = new QPushButton(this);
    nextPage= new QPushButton(this);

    lastPage->setText("<");
    nextPage->setText(">");

    lastPage->setMinimumSize(50,40);
    nextPage->setMinimumSize(50,40);

    nextPage->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    lastPage->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    connect(lastPage,&QPushButton::clicked,this,&CandidatesListWidget::onLastPage);
    connect(nextPage,&QPushButton::clicked,this,&CandidatesListWidget::onNextPage);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(lastPage);
    layout->addWidget(nextPage);
    layout->addItem(new QSpacerItem(80,40,QSizePolicy::Expanding,QSizePolicy::Minimum));
    layout->setSpacing(5);

    setLayout(layout);
    setFont(QFont("Arial",20));

    lastPage->setEnabled(false);
    nextPage->setEnabled(false);
}

void CandidatesListWidget::setCandidatesList(const QStringList &texts)
{
    candidatesList = texts;
    pageIndex = 0;
    curIndex = 0;
    headTextIndex = 0;
    tailTextIndex = 0;
    textRects.clear();
    pageHeadIndex.clear();
    pageHeadIndex.append(0);
    update();
}

void CandidatesListWidget::onLastPage()
{
    if(pageIndex == 0)
        return;

    --pageIndex;

    headTextIndex = pageHeadIndex.at(pageIndex);

    if(pageIndex == 0)
        lastPage->setEnabled(false);

    update();
}

void CandidatesListWidget::clear()
{

}

void CandidatesListWidget::onNextPage()
{
    if(tailTextIndex >= candidatesList.size() - 1)
        return;

    lastPage->setEnabled(true);

    headTextIndex = tailTextIndex + 1;
    ++pageIndex;

    if(pageIndex >= pageHeadIndex.size())
        pageHeadIndex.append(headTextIndex);

    update();
}

void CandidatesListWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    QFont font("Microsoft YaHei",18);
    painter.setFont(font);

    QRect rectText = nextPage->geometry();
    rectText.setLeft(rectText.right() + 20);
    rectText.setRight(width());

    textRects.clear();
    int i;
    for(i = headTextIndex;i < candidatesList.size();++i)
    {
        if(i == headTextIndex)
            painter.setPen(QColor(Qt::white));
        else
            painter.setPen(QColor(121,193,59));

        QString strTextDraw = /*QString::number(i - headTextIndex + 1) + "." + */candidatesList[i];

        QRect br = painter.boundingRect(rectText,Qt::AlignLeft | Qt::AlignVCenter,strTextDraw);

        if(br.right() + 30 >= width())
        {
            nextPage->setEnabled(true);
            break;
        }
        else
            painter.drawText(rectText,Qt::AlignLeft | Qt::AlignVCenter,strTextDraw);

        textRects.append(br);
        tailTextIndex = i;

        rectText.translate(br.width() + 30,0);
    }

    if(i >= candidatesList.size())
        nextPage->setEnabled(false);
}

void CandidatesListWidget::mousePressEvent(QMouseEvent *e)
{
    for(int i = 0;i < textRects.size();++i)
    {
        if(textRects[i].contains(e->pos()))
            chooseText(headTextIndex + i);
    }
}
