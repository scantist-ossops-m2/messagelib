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

#include "convertsnippetvariablesjob.h"
#include "messagecomposer_debug.h"
#include "composer/composerviewinterface.h"
#include <TemplateParser/TemplatesUtil>
using namespace MessageComposer;
ConvertSnippetVariablesJob::ConvertSnippetVariablesJob(QObject *parent)
    : QObject(parent)
{
}

ConvertSnippetVariablesJob::~ConvertSnippetVariablesJob()
{
    delete mComposerViewInterface;
}

void ConvertSnippetVariablesJob::setText(const QString &str)
{
    mText = str;
}

bool ConvertSnippetVariablesJob::canStart() const
{
    if (mText.isEmpty() || !mComposerViewInterface) {
        return false;
    }
    return true;
}

void ConvertSnippetVariablesJob::start()
{
    if (!canStart()) {
        Q_EMIT textConverted(QString());
        deleteLater();
        return;
    }
    Q_EMIT textConverted(convertVariables());
    deleteLater();
}

QString ConvertSnippetVariablesJob::text() const
{
    return mText;
}

MessageComposer::ComposerViewInterface *ConvertSnippetVariablesJob::composerViewInterface() const
{
    return mComposerViewInterface;
}

void ConvertSnippetVariablesJob::setComposerViewInterface(MessageComposer::ComposerViewInterface *composerViewInterface)
{
    mComposerViewInterface = composerViewInterface;
}

QString ConvertSnippetVariablesJob::convertVariables() const
{
    QString result;
    const int tmpl_len = mText.length();
    for (int i = 0; i < tmpl_len; ++i) {
        const QChar c = mText[i];
        if (c == QLatin1Char('%')) {
            const QString cmd = mText.mid(i + 1);
            if (cmd.startsWith(QLatin1String("CCADDR"))) {
                i += strlen("CCADDR");
                const QString str = mComposerViewInterface->cc();
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("FULLSUBJECT"))) {
                i += strlen("FULLSUBJECT");
                const QString str = mComposerViewInterface->subject();
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("TOADDR"))) {
                i += strlen("TOADDR");
                const QString str = mComposerViewInterface->to();
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("TOFNAME"))) {
                i += strlen("TOFNAME");
                const QString str = TemplateParser::Util::getFirstNameFromEmail(mComposerViewInterface->to());
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("TOLNAME"))) {
                i += strlen("TOLNAME");
                const QString str = TemplateParser::Util::getLastNameFromEmail(mComposerViewInterface->to());
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("FROMADDR"))) {
                i += strlen("FROMADDR");
                const QString str = mComposerViewInterface->from();
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("DOW"))) {
                i += strlen("DOW");
                const QString str = mComposerViewInterface->insertDayOfWeek();
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("DATE"))) {
                i += strlen("DATE");
                const QString str = mComposerViewInterface->longDate();
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("SHORTDATE"))) {
                i += strlen("SHORTDATE");
                const QString str = mComposerViewInterface->shortDate();
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("TIME"))) {
                i += strlen("TIME");
                const QString str = mComposerViewInterface->shortTime();
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("TIMELONG"))) {
                i += strlen("TIMELONG");
                const QString str = mComposerViewInterface->longTime();
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("ATTACHMENTCOUNT"))) {
                i += strlen("ATTACHMENTCOUNT");
                const QString str = QString::number(mComposerViewInterface->attachments().count());
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("ATTACHMENTNAMES"))) {
                i += strlen("ATTACHMENTNAMES");
                const QString str = mComposerViewInterface->attachments().names().join(QLatin1Char(','));
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("ATTACHMENTFILENAMES"))) {
                i += strlen("ATTACHMENTFILENAMES");
                const QString str = mComposerViewInterface->attachments().fileNames().join(QLatin1Char(','));
                result.append(str);
            } else if (cmd.startsWith(QLatin1String("ATTACHMENTNAMESANDSIZES"))) {
                i += strlen("ATTACHMENTNAMESANDSIZES");
                const QString str = mComposerViewInterface->attachments().namesAndSize().join(QLatin1Char(','));
                result.append(str);
            } else {
                result.append(c);
            }
        } else {
            result.append(c);
        }
    }
    return result;
}
