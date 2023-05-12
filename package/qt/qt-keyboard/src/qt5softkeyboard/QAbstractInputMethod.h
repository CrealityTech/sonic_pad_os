#ifndef QABSTRACTINPUTMETHOD_H
#define QABSTRACTINPUTMETHOD_H

#include <QObject>

class QAbstractInputMethod : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractInputMethod(QObject *parent = nullptr);
    virtual ~QAbstractInputMethod();
    enum class InputMode {
        Latin,
        Numeric,
        Dialable,
        Pinyin,
        Cangjie,
        Zhuyin,
        Hangul,
        Hiragana,
        Katakana,
        FullwidthLatin,
        Greek,
        Cyrillic,
        Arabic,
        Hebrew,
        ChineseHandwriting,
        JapaneseHandwriting,
        KoreanHandwriting,
        Thai
    };

    virtual bool keyEvent(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers) = 0;
    virtual void reset() {}
    virtual void changeLanguage() {}
    virtual void chooseCandidate(int) {}
signals:
    void showCandidateList(const QStringList & strings);
    void showLanguageName(const QString & name);
public slots:
};

#endif // QABSTRACTINPUTMETHOD_H
