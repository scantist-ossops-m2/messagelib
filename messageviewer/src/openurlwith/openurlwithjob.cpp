/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "openurlwithjob.h"
#include "messageviewer_debug.h"
#include <KIO/CommandLauncherJob>
#include <KNotificationJobUiDelegate>
using namespace MessageViewer;
OpenUrlWithJob::OpenUrlWithJob(QObject *parent)
    : QObject{parent}
{
}

OpenUrlWithJob::~OpenUrlWithJob()
{
}

bool OpenUrlWithJob::canStart() const
{
    return mInfo.isValid();
}

void OpenUrlWithJob::start()
{
    if (!canStart()) {
        deleteLater();
        qCWarning(MESSAGEVIEWER_LOG) << " Impossible to start OpenUrlWithJob";
        return;
    }
    auto job = new KIO::CommandLauncherJob(mInfo.command(), QStringList() << mInfo.commandLine());
    job->setUiDelegate(new KNotificationJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled));
    job->start();
    deleteLater();
}

const OpenWithUrlInfo &OpenUrlWithJob::info() const
{
    return mInfo;
}

void OpenUrlWithJob::setInfo(const OpenWithUrlInfo &newInfo)
{
    mInfo = newInfo;
}
