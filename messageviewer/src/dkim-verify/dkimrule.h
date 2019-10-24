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

#ifndef DKIMRULE_H
#define DKIMRULE_H
#include "messageviewer_export.h"
#include <QDebug>
#include <QObject>
namespace MessageViewer {
class MESSAGEVIEWER_EXPORT DKIMRule
{
public:
    DKIMRule();
    Q_REQUIRED_RESULT QString domain() const;
    void setDomain(const QString &domain);

    Q_REQUIRED_RESULT  QStringList signedDomainIdentifier() const;
    void setSignedDomainIdentifier(const QStringList &signedDomainIdentifier);

    Q_REQUIRED_RESULT  QString from() const;
    void setFrom(const QString &from);

    Q_REQUIRED_RESULT bool enabled() const;
    void setEnabled(bool enabled);

    Q_REQUIRED_RESULT bool isValid() const;

private:
    QStringList mSignedDomainIdentifier;
    QString mDomain;
    QString mFrom;
    bool mEnabled = true;
};
}
Q_DECLARE_METATYPE(MessageViewer::DKIMRule)
Q_DECLARE_TYPEINFO(MessageViewer::DKIMRule, Q_MOVABLE_TYPE);
MESSAGEVIEWER_EXPORT QDebug operator <<(QDebug d, const MessageViewer::DKIMRule &t);

#endif // DKIMRULE_H