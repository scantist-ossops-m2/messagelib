/*
  SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "printmessagetest.h"
#include "viewer/printmessage.h"

#include <QTest>

QTEST_MAIN(PrintMessageTest)
PrintMessageTest::PrintMessageTest(QObject *parent)
    : QObject{parent}
{
}

void PrintMessageTest::shouldHaveDefaultValues()
{
    MessageViewer::PrintMessage w;
    QVERIFY(w.documentName().isEmpty());
    QVERIFY(!w.parentWidget());
}