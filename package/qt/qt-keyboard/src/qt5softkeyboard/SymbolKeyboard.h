#ifndef SYMBOLKEYBOARD_H
#define SYMBOLKEYBOARD_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include <QAbstractButton>

namespace Ui {
class SymbolKeyboard;
}

class SymbolKeyboard : public QWidget
{
    Q_OBJECT

public:
    explicit SymbolKeyboard(QWidget *parent = nullptr);
    ~SymbolKeyboard();
signals:
    void changeLanguage();
    void changeSymbol();
    void hideKeyboard();
    void keyPressed(Qt::Key key, const QString &text,Qt::KeyboardModifiers modifiers);

public slots:
    void setCurLanguage(const QString& lang);

private slots:
    void onChangePage();
    void onSymbolButtonClicked(QAbstractButton*);
protected:
    void buttonRemap();
    QString buttonText(QAbstractButton *);
private:
    Ui::SymbolKeyboard *ui;
    QVector<QPair<int,QString>> page1;
    int curPage;
    int maxPage;
    QVector<QAbstractButton*> symbolButtons;
    QMap<QAbstractButton*,int> keyMap;
};

#endif // SYMBOLKEYBOARD_H
