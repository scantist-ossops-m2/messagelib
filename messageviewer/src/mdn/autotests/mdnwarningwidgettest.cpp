/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "mdnwarningwidgettest.h"
#include "mdn/mdnwarningwidget.h"

#include <QTest>

QTEST_MAIN(MDNWarningWidgetTest)

MDNWarningWidgetTest::MDNWarningWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MDNWarningWidgetTest::shouldHaveDefaultValues()
{
    MessageViewer::MDNWarningWidget w;
    QVERIFY(!w.isVisible());
    QVERIFY(w.wordWrap());

    QCOMPARE(w.messageType(), KMessageWidget::Information);
    // mIgnoreAction->setObjectName(QStringLiteral("mIgnoreAction"));
    //  mSendAction->setObjectName(QStringLiteral("mSendAction"));
    //  TODO
}
