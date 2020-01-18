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

#ifndef DKIMVIEWERMENU_H
#define DKIMVIEWERMENU_H

#include <QObject>
#include "messageviewer_export.h"
class QMenu;
class QAction;
namespace MessageViewer {
class MESSAGEVIEWER_EXPORT DKIMViewerMenu : public QObject
{
    Q_OBJECT
public:
    explicit DKIMViewerMenu(QObject *parent = nullptr);
    ~DKIMViewerMenu();
    QMenu *menu() const;

    void setEnableUpdateDkimKeyMenu(bool enabled);
Q_SIGNALS:
    void recheckSignature();
    void updateDkimKey();
    void showDkimRules();

private:
    void initialize();
    QMenu *mMenu = nullptr;
    QAction *mUpdateDKIMKey = nullptr;
};
}

#endif // DKIMVIEWERMENU_H