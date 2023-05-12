#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H
#include <QtGui/qpa/qplatforminputcontext.h>
#include <QInputMethodEvent>
#include "KeyboardContainer.h"
#include "QKeyboardLayout.h"

class InputContextPrivate;
class QAbstractInputMethod;
class InputContext : public QPlatformInputContext
{
    Q_OBJECT
public:
    InputContext();
    virtual ~InputContext();

    virtual bool isValid() const;

    virtual QRectF keyboardRect() const;

    virtual bool isInputPanelVisible() const;

    virtual void update(Qt::InputMethodQueries queries);

    QObject *focusObject();

    virtual void setFocusObject(QObject *object);

    virtual void hideInputPanel();

    virtual void showInputPanel();

    virtual bool isAnimating() const;

    Q_INVOKABLE void commit(const QString &text, int replaceFrom = 0, int replaceLength = 0);

    Q_INVOKABLE void commit();

    void sendKeyClick(int key, const QString &text, int modifiers = 0);

    void sendKeyEvent(QKeyEvent *event);

    void setPreeditText(const QString &text,QList<QInputMethodEvent::Attribute> attributes = QList<QInputMethodEvent::Attribute>(),int replaceFrom = 0, int replaceLength = 0);

    void sendEvent(QEvent *event);
signals:    
	void inputMethodHintsChanged();
public slots:
    void hideKeyboard();
protected:
    void sendPreedit(const QString &text, const QList<QInputMethodEvent::Attribute> &attributes, int replaceFrom, int replaceLength);
    void updateInputPanelVisible();
    QVariant inputMethodQuery(Qt::InputMethodQuery query);
private:
    KeyboardContainer * m_inputPanel;
    QObject *m_focusObject;
    QEvent *m_filterEvent;
    QAbstractInputMethod * inputMethod;
    QString preeditText;
    QKeyboardLayout mainLayout;
    QKeyboardLayout symbolLayout;
    bool m_visible;
    bool m_needHiden;
    Qt::InputMethodHints inputMethodHints;
};

#endif // INPUTCONTEXT_H
