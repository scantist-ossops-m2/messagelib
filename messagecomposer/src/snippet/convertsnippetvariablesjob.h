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

#ifndef CONVERTSNIPPETVARIABLESJOB_H
#define CONVERTSNIPPETVARIABLESJOB_H

#include <QObject>
#include "messagecomposer_export.h"
namespace MessageComposer {
class ComposerViewInterface;
class MESSAGECOMPOSER_EXPORT ConvertSnippetVariablesJob : public QObject
{
    Q_OBJECT
public:
    explicit ConvertSnippetVariablesJob(QObject *parent = nullptr);
    ~ConvertSnippetVariablesJob();
    void start();

    void setText(const QString &str);
    Q_REQUIRED_RESULT QString text() const;

    MessageComposer::ComposerViewInterface *composerViewInterface() const;
    void setComposerViewInterface(MessageComposer::ComposerViewInterface *composerViewInterface);
    static Q_REQUIRED_RESULT QString convertVariables(MessageComposer::ComposerViewInterface *composerView, const QString &text);

    Q_REQUIRED_RESULT bool canStart() const;
Q_SIGNALS:
    void textConverted(const QString &str);

private:
    static QString getFirstNameFromEmail(QString address);
    static QString getLastNameFromEmail(QString address);
    QString mText;
    MessageComposer::ComposerViewInterface *mComposerViewInterface = nullptr;
};
}
#endif // CONVERTVARIABLESJOB_H
