/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mdnwarningwidget.h"
#include <KLocalizedString>
#include <QAction>

using namespace MessageViewer;
MDNWarningWidget::MDNWarningWidget(QWidget *parent)
    : KMessageWidget(parent)
    , mIgnoreAction(new QAction(i18nc("@action", "Ignore"), this))
    , mSendAction(new QAction(i18nc("@action", "Send"), this))
{
    setVisible(false);
    setMessageType(Information);
    setWordWrap(true);
    // TODO
    // setText(i18n("This message may be a scam. <a href=\"scamdetails\">(Details...)</a>"));
    connect(mIgnoreAction, &QAction::triggered, this, &MDNWarningWidget::slotIgnore);
    addAction(mIgnoreAction);
    connect(mSendAction, &QAction::triggered, this, &MDNWarningWidget::slotSend);
    addAction(mSendAction);
    mIgnoreAction->setObjectName(QStringLiteral("mIgnoreAction"));
    mSendAction->setObjectName(QStringLiteral("mSendAction"));
}

MDNWarningWidget::~MDNWarningWidget() = default;

void MDNWarningWidget::slotSend()
{
    Q_EMIT sendMdn();
}

void MDNWarningWidget::slotIgnore()
{
    Q_EMIT ignoreMdn();
}
