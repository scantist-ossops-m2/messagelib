/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "composerviewinterface.h"
#include "composerviewbase.h"
#include <MessageComposer/AttachmentModel>
#include <KFormat>
using namespace MessageComposer;
ComposerViewInterface::ComposerViewInterface(ComposerViewBase *composerView)
    : mComposerView(composerView)
{
}

ComposerViewInterface::~ComposerViewInterface()
{
}

QString ComposerViewInterface::subject() const
{
    if (mComposerView) {
        return mComposerView->subject();
    }
    return {};
}

QString ComposerViewInterface::to() const
{
    if (mComposerView) {
        return mComposerView->to();
    }
    return {};
}

QString ComposerViewInterface::cc() const
{
    if (mComposerView) {
        return mComposerView->cc();
    }
    return {};
}

QString ComposerViewInterface::from() const
{
    if (mComposerView) {
        return mComposerView->from();
    }
    return {};
}

MessageComposer::ComposerAttachmentInterface ComposerViewInterface::attachments()
{
    MessageComposer::ComposerAttachmentInterface attachmentInterface;
    if (mComposerView) {
        const int countElement = mComposerView->attachmentModel()->attachments().count();
        attachmentInterface.setCount(countElement);
        QStringList fileNames;
        QStringList nameAndSize;
        QStringList names;

        fileNames.reserve(countElement);
        nameAndSize.reserve(countElement);
        names.reserve(countElement);
        const auto attachments = mComposerView->attachmentModel()->attachments();
        for (const MessageCore::AttachmentPart::Ptr &attachment : attachments) {
            fileNames.append(attachment->fileName());
            names.append(attachment->name());
            nameAndSize.append(QStringLiteral("%1 (%2)").arg(attachment->name(), KFormat().formatByteSize(attachment->size())));
        }
        attachmentInterface.setNames(names);
        attachmentInterface.setNamesAndSize(nameAndSize);
        attachmentInterface.setFileNames(fileNames);
    }
    return attachmentInterface;
}

QString ComposerViewInterface::shortDate() const
{
    QLocale locale;
    return locale.toString(QDate::currentDate(), QLocale::ShortFormat);
}

QString ComposerViewInterface::longDate() const
{
    QLocale locale;
    return locale.toString(QDate::currentDate(), QLocale::LongFormat);
}

QString ComposerViewInterface::shortTime() const
{
    QLocale locale;
    return locale.toString(QTime::currentTime(), QLocale::ShortFormat);
}

QString ComposerViewInterface::longTime() const
{
    QLocale locale;
    return locale.toString(QTime::currentTime(), QLocale::LongFormat);
}

QString ComposerViewInterface::insertDayOfWeek() const
{
    const QDateTime date = QDateTime::currentDateTime().toLocalTime();
    const QString str = QLocale().dayName(date.date().dayOfWeek(), QLocale::LongFormat);
    return str;
}
