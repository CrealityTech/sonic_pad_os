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

#include "webview.h"
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    connect(this, &QWebEngineView::loadStarted, [this]() {
        qWarning("WebEngineView::loadStarted ...");
    });

    connect(this, &QWebEngineView::loadProgress, [this](int progress) {
        qWarning("WebEngineView::loadProgress = %d", progress);
    });

    connect(this, &QWebEngineView::loadFinished, [this](bool ok) {
        qWarning("WebEngineView::loadFinished = %d , will show...", ok);
        this->show();
    });

    connect(this, &QWebEngineView::iconChanged, [this](const QIcon &icon) {
        qWarning("WebEngineView::iconChanged = %s", qPrintable(icon.name()));
    });

    connect(this, &QWebEngineView::iconUrlChanged, [this](const QUrl &url) {
        qWarning("WebEngineView::iconUrlChanged = %s", qPrintable(url.toString()));
    });

    connect(this, &QWebEngineView::titleChanged, [this](const QString &title) {
        qWarning("WebEngineView::titleChanged = %s", qPrintable(title));
    });

    connect(this, &QWebEngineView::urlChanged, [this](const QUrl &url) {
        qWarning("WebEngineView::urlChanged = %s", qPrintable(url.toString()));
    });

    connect(this, &QWebEngineView::renderProcessTerminated,
            [this](QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode) {
        QString status;
        switch (termStatus) {
        case QWebEnginePage::NormalTerminationStatus:
            status = tr("Render process normal exit");
            break;
        case QWebEnginePage::AbnormalTerminationStatus:
            status = tr("Render process abnormal exit");
            break;
        case QWebEnginePage::CrashedTerminationStatus:
            status = tr("Render process crashed");
            break;
        case QWebEnginePage::KilledTerminationStatus:
            status = tr("Render process killed");
            break;
        }
#if 0
        QMessageBox::StandardButton btn = QMessageBox::question(window(), status,
                                                   tr("Render process exited with code: %1\n"
                                                      "Do you want to reload the page ?").arg(statusCode));
        if (btn == QMessageBox::Yes)
            QTimer::singleShot(0, [this] { reload(); });
#else
        QMessageBox::StandardButton btn = QMessageBox::information(window(), status,
                                                   tr("Render process exited with code: %1\n"
                                                      "You should reload the page!").arg(statusCode),
                                                   QMessageBox::Ok, QMessageBox::Ok);
        if (btn == QMessageBox::Ok)
            QTimer::singleShot(0, [this] { reload(); });

#endif
    });
}

