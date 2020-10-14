/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "shownextmessagewidgettest.h"
#include "widgets/shownextmessagewidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTest>
QTEST_MAIN(ShowNextMessageWidgetTest)
ShowNextMessageWidgetTest::ShowNextMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ShowNextMessageWidgetTest::shouldHaveDefaultValue()
{
    MessageViewer::ShowNextMessageWidget w;

    QHBoxLayout *mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    //mainLayout->setContentsMargins({});

    QPushButton *mPreviousMessage = w.findChild<QPushButton *>(QStringLiteral("previous_message"));
    QVERIFY(mPreviousMessage);
    QVERIFY(!mPreviousMessage->text().isEmpty());
    QVERIFY(!mPreviousMessage->isEnabled());

    QPushButton *mNextMessage = w.findChild<QPushButton *>(QStringLiteral("next_message"));
    QVERIFY(mNextMessage);
    QVERIFY(!mNextMessage->text().isEmpty());
    QVERIFY(!mNextMessage->isEnabled());
}
