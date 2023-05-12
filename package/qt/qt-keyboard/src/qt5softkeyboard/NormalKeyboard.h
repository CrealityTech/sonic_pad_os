#ifndef NORMALKEYBOARD_H
#define NORMALKEYBOARD_H

#include <QWidget>
#include <QPushButton>
#include <algorithm>
#include <QMap>

namespace Ui {
class NormalKeyboard;
}

class NormalKeyboard : public QWidget
{
    Q_OBJECT

public:
    explicit NormalKeyboard(QWidget *parent = nullptr);
    ~NormalKeyboard();

signals:
    void changeLanguage();
    void changeSymbol();
    void hideKeyboard();
    void keyPressed(Qt::Key key, const QString &text,Qt::KeyboardModifiers modifiers);
public slots:
    void setCurLanguage(const QString& lang);
private slots:
    void letterClicked(QAbstractButton *);
protected:
    QString buttonText(QAbstractButton *);
    //大小写字母切换
    void capsLock();
private:
    Ui::NormalKeyboard *ui;
    QMap<QAbstractButton*,Qt::Key> letterkeys;
    Qt::KeyboardModifiers modifier;
};

#endif // NORMALKEYBOARD_H
