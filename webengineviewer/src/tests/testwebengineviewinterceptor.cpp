/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "testwebengineviewinterceptor.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEngineUrlRequestInterceptor>


class CidUrlRequestInterceptor : public QWebEngineUrlRequestInterceptor
{
public:
    explicit CidUrlRequestInterceptor(QObject *p = nullptr);
    void interceptRequest(QWebEngineUrlRequestInfo &info) override;
    ~CidUrlRequestInterceptor();
};

CidUrlRequestInterceptor::CidUrlRequestInterceptor(QObject *p)
    : QWebEngineUrlRequestInterceptor(p)
{

}

void CidUrlRequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    const QUrl urlRequestUrl(info.requestUrl());
    if (urlRequestUrl.scheme() == QLatin1String("cid")) {
        //QUrl r = QUrl::fromLocalFile(QStringLiteral("/source5/kde/kde5.14/share/icons/oxygen/base/16x16/status/audio-volume-medium.png"));
        //const QUrl r = QUrl(QStringLiteral("qrc:repeat.svg"));
        const QUrl r = QUrl(QStringLiteral("qrc:audio-volume-medium.png"));
        qDebug() << " r " << r;
        info.redirect(r);
    }
    return;
}

CidUrlRequestInterceptor::~CidUrlRequestInterceptor()
{

}

TestWebEngineViewInterceptor::TestWebEngineViewInterceptor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    mWebEngineView = new QWebEngineView(this);
    mWebEngineView->page()->profile()->setUrlRequestInterceptor(new CidUrlRequestInterceptor(this));

    const QString htmlStr = QStringLiteral("<html>"
                                           "<head>"
                                           "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">"
                                           "</head>"
                                           "<body><p>Xcvwxcvwxvccwv xwvc w<img src=\"cid:1506382412@KDE\" />cvbxcvbxcvbcvxbvcxbx</p>"
                                           "<p>Xcv</p>"
                                           "<p>bxcvbxcvbxcvb</p>"
                                           "<br /></body>"
                                           "</html>");

    mWebEngineView->setHtml(htmlStr);
    vbox->addWidget(mWebEngineView);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    TestWebEngineViewInterceptor *testWebEngine = new TestWebEngineViewInterceptor;
    testWebEngine->show();
    testWebEngine->resize(600, 400);
    const int ret = app.exec();
    return ret;
}

