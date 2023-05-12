/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include "webview.h"
#include "watchdog.h"
#include <QMutex>
#include <QFile>

QUrl commandLineUrlArgument()
{
    const QStringList args = QCoreApplication::arguments();
    for (const QString &arg : args.mid(1)) {
        if (!arg.startsWith(QLatin1Char('-')))
            return QUrl::fromUserInput(arg);
    }
    return QUrl(QStringLiteral("https://www.qt.io"));
}

void logMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static qint64 max = 10485760;// 10 * 1024 * 1024;
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch (type) {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
        abort();
    default:
        break;
    }
    QString message = QString("[%1] %2 [%3] [%4] [%5] %6").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(text).arg(context.file).arg(context.function).arg(context.line).arg(msg);

    QFile file("/mnt/UDISK/qt_minimal.txt");
    if (file.size() > max) { //超过指定大小时截取文件
        file.rename("/mnt/UDISK/tmp.txt");
        QFile logfile("/mnt/UDISK/qt_minimal.txt");

        file.open(QIODevice::ReadWrite);
        logfile.open(QIODevice::ReadWrite);
        qint64 nresult = 2097152; // 2 * 1024 * 1024;
        file.seek(file.size() - nresult);
        char lbuffer[256];
        int count;
        while ((count = file.read(lbuffer, 256)) == 256) {
            logfile.write(lbuffer, count);
        }
        logfile.write(message.toLatin1(), message.count());
        file.close();
        file.remove();
        logfile.close();
    } else {
        QTextStream text_stream(&file);
        file.open(QIODevice::ReadWrite | QIODevice::Append);
        text_stream << message << endl;
        file.flush();
        file.close();
    }
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("QtExamples");
    qputenv("QT_IM_MODULE",QByteArray("Qt5Input"));
    QApplication app(argc, argv);
    qInstallMessageHandler(logMessageOutput);

    watchdog watch_dog;
    WebView view;
    qWarning("========== create view... ==========");
    view.setContextMenuPolicy(Qt::NoContextMenu);
    //view.setAttribute(Qt::WA_TranslucentBackground);
    //view.setStyleSheet("background:transparent");
    view.setUrl(commandLineUrlArgument());
    qWarning("setUrl done");
    view.resize(600, 1024);
    qWarning("resize 600x1024 done");
    view.page()->setBackgroundColor(QColor(0, 0, 0));
    qWarning("setBackgroundColor done");
    //view.show();
    qWarning("app.exec...");
    app.exec();
    return 0;
}
