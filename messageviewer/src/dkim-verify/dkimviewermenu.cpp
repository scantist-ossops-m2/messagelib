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


#include "dkimviewermenu.h"
#include <QMenu>
#include <KLocalizedString>

using namespace MessageViewer;
DKIMViewerMenu::DKIMViewerMenu(QObject *parent)
    : QObject(parent)
{
    initialize();
}

DKIMViewerMenu::~DKIMViewerMenu()
{
    delete mMenu;
}

QMenu *DKIMViewerMenu::menu() const
{
    return mMenu;
}

void DKIMViewerMenu::initialize()
{
    mMenu = new QMenu;
    QAction *checkSignature = mMenu->addAction(i18n("Recheck DKIM Signature"));
    connect(checkSignature, &QAction::triggered, this, &DKIMViewerMenu::recheckSignature);

    QAction *updateDKIMKey = mMenu->addAction(i18n("Update DKIM key"));
    connect(updateDKIMKey, &QAction::triggered, this, &DKIMViewerMenu::updateDKimKey);
    //TODO add more
}
