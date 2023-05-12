#ifndef QPINYININPUTMETHOD_H
#define QPINYININPUTMETHOD_H

#include "QAbstractInputMethod.h"

class QPinyinInputMethodPrivate;
class InputContext;
class QPinyinInputMethod : public QAbstractInputMethod
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPinyinInputMethod)
public:
    explicit QPinyinInputMethod(QObject *parent = nullptr);
    ~QPinyinInputMethod();

    InputContext *inputContext() const;
	void setInputContext(InputContext * context);
    virtual bool keyEvent(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers);
    virtual void reset();
    virtual void changeLanguage();
    virtual void chooseCandidate(int id);
signals:

public slots:
private:
    QPinyinInputMethodPrivate *d_ptr;
    InputContext * m_inputContext;
};

#endif // QPINYININPUTMETHOD_H
