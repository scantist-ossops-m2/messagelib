/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "remotecontentmanager.h"
#include "remotecontentinfo.h"
#include <KConfigGroup>
#include <KSharedConfig>

using namespace MessageViewer;
RemoteContentManager::RemoteContentManager(QObject *parent)
    : QObject(parent)
{
    loadSettings();
}

RemoteContentManager::~RemoteContentManager()
{
    writeSettings();
}

RemoteContentManager *RemoteContentManager::self()
{
    static RemoteContentManager s_self;
    return &s_self;
}

bool RemoteContentManager::isAutorized(const QString &url) const
{
    for (const RemoteContentInfo &info : qAsConst(mRemoveContentInfo)) {
        if (info.url() == url) {
            return info.status() == RemoteContentInfo::RemoteContentInfoStatus::Authorized;
        }
    }
    return false;
}

void RemoteContentManager::loadSettings()
{
    mRemoveContentInfo.clear();
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group(config, "RemoteContent");
    const QStringList blockedUrl = group.readEntry("Blocked", QStringList());
    const QStringList authorizedUrl = group.readEntry("Authorized", QStringList());
    for (const QString &url : blockedUrl) {
        RemoteContentInfo info;
        info.setUrl(url);
        info.setStatus(RemoteContentInfo::RemoteContentInfoStatus::Blocked);
        mRemoveContentInfo.append(info);
    }
    for (const QString &url : authorizedUrl) {
        RemoteContentInfo info;
        info.setUrl(url);
        info.setStatus(RemoteContentInfo::RemoteContentInfoStatus::Authorized);
        mRemoveContentInfo.append(info);
    }
}

void RemoteContentManager::writeSettings()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group(config, "RemoteContent");
    QStringList blockedUrl;
    QStringList authorizedUrl;
    for (const RemoteContentInfo &info : qAsConst(mRemoveContentInfo)) {
        switch (info.status()) {
        case RemoteContentInfo::RemoteContentInfoStatus::Unknown:
            break;
        case RemoteContentInfo::RemoteContentInfoStatus::Blocked:
            blockedUrl.append(info.url());
            break;
        case RemoteContentInfo::RemoteContentInfoStatus::Authorized:
            authorizedUrl.append(info.url());
            break;
        }
    }
    group.writeEntry("Blocked", blockedUrl);
    group.writeEntry("Authorized", authorizedUrl);
}

void RemoteContentManager::addRemoteContent(const RemoteContentInfo &info)
{
    mRemoveContentInfo.append(info);
}