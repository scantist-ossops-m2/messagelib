/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "remotecontentmenu.h"
#include "remote-content/remotecontentmanager.h"
#include "remotecontentconfiguredialog.h"
#include "remotecontentinfo.h"
#include <KLocalizedString>
#include <QAction>
#include <QDebug>
#include <QPointer>

using namespace MessageViewer;
RemoteContentMenu::RemoteContentMenu(QWidget *parent)
    : QMenu(parent)
    , mConfigureRemoteContentAction(new QAction(i18n("Configure"), this))
{
    setTitle(i18n("Remote Content"));
    connect(this, &QMenu::aboutToShow, this, &RemoteContentMenu::updateMenu);
}

RemoteContentMenu::~RemoteContentMenu()
{
}

void RemoteContentMenu::slotConfigure()
{
    QPointer<MessageViewer::RemoteContentConfigureDialog> remoteContentDialog = new MessageViewer::RemoteContentConfigureDialog(this);
    remoteContentDialog->exec();
    delete remoteContentDialog;
}

QStringList RemoteContentMenu::urls() const
{
    return mUrls;
}

void RemoteContentMenu::setUrls(const QStringList &urls)
{
    mUrls = urls;
}

void RemoteContentMenu::updateMenu()
{
    for (auto act : qAsConst(mListAction)) {
        removeAction(act);
    }
    const int numberOfUrl{mUrls.count()};
    mListAction.reserve(numberOfUrl);
    for (const QString &url : qAsConst(mUrls)) {
        QAction *act = addAction(i18n("Authorize %1", url));
        connect(act, &QAction::triggered, this, [this, url]() {
            authorize(url);
            mUrls.removeAll(url);
        });
        mListAction << act;
    }
    mListAction << addSeparator();
    if (numberOfUrl > 0) {
        QAction *act = addAction(i18n("Authorize all Urls"));
        connect(act, &QAction::triggered, this, [this]() {
            for (const QString &url : qAsConst(mUrls)) {
                authorize(url);
            }
            mUrls.clear();
        });
        mListAction << act;
        mListAction << addSeparator();
    }
    addAction(mConfigureRemoteContentAction);
    connect(mConfigureRemoteContentAction, &QAction::triggered, this, &RemoteContentMenu::slotConfigure);
}

void RemoteContentMenu::authorize(const QString &url)
{
    RemoteContentInfo info;
    info.setUrl(url);
    info.setStatus(RemoteContentInfo::RemoteContentInfoStatus::Authorized);
    RemoteContentManager::self()->addRemoteContent(info);
    Q_EMIT updateEmail();
}

void RemoteContentMenu::clearUrls()
{
    mUrls.clear();
}

void RemoteContentMenu::appendUrl(const QString &url)
{
    if (!mUrls.contains(url)) {
        mUrls.append(url);
    }
}
