/*
   Copyright (C) 2019 Laurent Montel <montel@kde.org>

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

#include "dkimcheckpolicyjob.h"
#include "dmarcpolicyjob.h"
#include "settings/messageviewersettings.h"
#include "messageviewer_dkimcheckerdebug.h"
using namespace MessageViewer;
DKIMCheckPolicyJob::DKIMCheckPolicyJob(QObject *parent)
    : QObject(parent)
{
}

DKIMCheckPolicyJob::~DKIMCheckPolicyJob()
{
}

bool DKIMCheckPolicyJob::canStart() const
{
    return !mEmailAddress.isEmpty();
}

bool DKIMCheckPolicyJob::start()
{
    if (!canStart()) {
        qCWarning(MESSAGEVIEWER_DKIMCHECKER_LOG) << "Impossible to start DKIMCheckPolicyJob" << mEmailAddress;
        Q_EMIT result(mCheckResult);
        deleteLater();
        return false;
    }
    if (MessageViewer::MessageViewerSettings::self()->useDMarc()) {
        DMARCPolicyJob *job = new DMARCPolicyJob(this);
        job->setEmailAddress(mEmailAddress);
        connect(job, &DMARCPolicyJob::result, this, &DKIMCheckPolicyJob::dmarcPolicyResult);
        if (!job->start()) {
            qCWarning(MESSAGEVIEWER_DKIMCHECKER_LOG) << "Impossible to start DKIMCheckPolicyJob" << mEmailAddress;
            Q_EMIT result(mCheckResult);
            deleteLater();
            return false;
        }
    } else {
        //Verify list
        //TODO
        Q_EMIT result(mCheckResult);
        deleteLater();
    }
    return true;
}

void DKIMCheckPolicyJob::dmarcPolicyResult(const MessageViewer::DMARCPolicyJob::DMARCResult &value)
{
    if (value.isValid()) {
        if (mCheckResult.status == DKIMCheckSignatureJob::DKIMStatus::EmailNotSigned) {
            mCheckResult.status = DKIMCheckSignatureJob::DKIMStatus::NeedToBeSigned;
            //qDebug() << " void DKIMCheckPolicyJob::dmarcPolicyResult(const MessageViewer::DMARCPolicyJob::DMARCResult &value)"<<value.mDomain << "value " << value.mSource;
            //TODO verify it.
            mCheckResult.signedBy = value.mSource;
        }
    }
    Q_EMIT result(mCheckResult);
    deleteLater();
}

MessageViewer::DKIMCheckSignatureJob::CheckSignatureResult DKIMCheckPolicyJob::checkResult() const
{
    return mCheckResult;
}

void DKIMCheckPolicyJob::setCheckResult(const MessageViewer::DKIMCheckSignatureJob::CheckSignatureResult &checkResult)
{
    mCheckResult = checkResult;
}

QString DKIMCheckPolicyJob::emailAddress() const
{
    return mEmailAddress;
}

void DKIMCheckPolicyJob::setEmailAddress(const QString &emailAddress)
{
    mEmailAddress = emailAddress;
}