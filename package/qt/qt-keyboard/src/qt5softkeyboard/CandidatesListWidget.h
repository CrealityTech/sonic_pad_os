#ifndef CANDIDATELIST_WIDGET_H
#define CANDIDATELIST_WIDGET_H

#include <QWidget>

class QPushButton;
class CandidatesListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CandidatesListWidget(QWidget *parent = nullptr);

signals:
    void chooseText(int id);
public slots:
    void setCandidatesList(const QStringList & texts);
    void onNextPage();
    void onLastPage();
    void clear();
protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *e);
private:
    QStringList candidatesList;
    QList<QRect> textRects;
    QList<int>    pageHeadIndex;
    QPushButton * lastPage;
    QPushButton * nextPage;
    int pageIndex = 0;
    int curIndex = 0;
    int headTextIndex = 0;
    int tailTextIndex = 0;
};

#endif // TEXTSELECTWIDGET_H
