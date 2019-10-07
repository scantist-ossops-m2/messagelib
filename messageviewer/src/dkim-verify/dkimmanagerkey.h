/*
   Copyright (C) 2018-2019 Laurent Montel <montel@kde.org>

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

#ifndef DKIMMANAGERKEY_H
#define DKIMMANAGERKEY_H

#include <QObject>
#include "messageviewer_export.h"
#include <QVector>
namespace MessageViewer {
struct MESSAGEVIEWER_EXPORT KeyInfo {
    QString keyValue;
    QString selector;
    QString domain;
    Q_REQUIRED_RESULT bool operator ==(const KeyInfo &) const;
};
class MESSAGEVIEWER_EXPORT DKIMManagerKey : public QObject
{
    Q_OBJECT
public:
    explicit DKIMManagerKey(QObject *parent = nullptr);
    ~DKIMManagerKey() override;

    static DKIMManagerKey *self();

    void loadKeys();
    void saveKeys();

    void addKey(const KeyInfo &key);
    void removeKey(const QString &key);

    Q_REQUIRED_RESULT QVector<KeyInfo> keys() const;

private:
    QVector<KeyInfo> mKeys;
};
}
Q_DECLARE_TYPEINFO(MessageViewer::KeyInfo, Q_MOVABLE_TYPE);
#endif // DKIMMANAGERKEY_H
