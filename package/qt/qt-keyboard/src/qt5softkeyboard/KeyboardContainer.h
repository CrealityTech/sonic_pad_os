#ifndef KEYBOARDCONTAINER_H
#define KEYBOARDCONTAINER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "NormalKeyboard.h"
#include "SymbolKeyboard.h"
#include "CandidatesListWidget.h"

class QPropertyAnimation;
class KeyboardContainer : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardContainer(QWidget *parent = nullptr);
    ~KeyboardContainer();

    bool isAnimating() const;
signals:
    void changeLanguage();
    void changeSymbol();
    void hideKeyboard();
    void keyPressed(Qt::Key key, const QString &text,Qt::KeyboardModifiers modifiers);
    void chooseCandidate(int id);
public slots:
    void setCandidateList(const QStringList & texts);
    void setLanguageName(const QString & name);
    void onChangeSymbol();
    void onHideSymbol();
    void animationHide();
    void animationShow();
	void setInputMethodHints(Qt::InputMethodHints hints);
    void onAnimationFinished();
private:
    NormalKeyboard * normalKeyboard;
    SymbolKeyboard * symbolKeyboard;
    CandidatesListWidget * textDisplayWidget;
    QVBoxLayout * mainLayout;
    QHBoxLayout * bottomLayout;
    QPropertyAnimation * animation;
    bool m_hiding;
};

#endif // KEYBOARDCONTAINER_H
