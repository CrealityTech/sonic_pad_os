#ifndef QKEYBOARDLAYOUT_H
#define QKEYBOARDLAYOUT_H

#include <QObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QVector>


class QKeyboardLayout : public QObject
{
    Q_OBJECT
public:
    explicit QKeyboardLayout(QObject *parent = nullptr);

    bool LoadLayout(const QString & strJsonPath);
signals:

public slots:

private:
    QVector<QJsonValue> keyboardRows;
};

#endif // QKEYBOARDLAYOUT_H
