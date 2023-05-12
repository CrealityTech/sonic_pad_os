#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <QBasicTimer>
#include <QtCore/qobject.h>
#include <QTcpSocket>
#include <QByteArray>

class watchdog : public QObject
{
public:
    watchdog(QObject *parent = 0);

protected:
    void timerEvent(QTimerEvent *event) override;
private:
    QBasicTimer timer;
    QTcpSocket *tcpSocket;
};

#endif // WATCHDOG_H

