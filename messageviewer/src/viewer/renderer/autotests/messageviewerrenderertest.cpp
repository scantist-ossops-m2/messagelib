/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

   Code based on ARHParser.jsm from dkim_verifier (Copyright (c) Philippe Lieser)
   (This software is licensed under the terms of the MIT License.)

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

#include "messageviewerrenderertest.h"
#include "viewer/renderer/messageviewerrenderer.h"
#include <QTest>
QTEST_MAIN(MessageViewerRendererTest)

MessageViewerRendererTest::MessageViewerRendererTest(QObject *parent)
    : QObject(parent)
{
}

void MessageViewerRendererTest::shouldHaveDefaultValue()
{
    MessageViewer::MessageViewerRenderer w;
    QVERIFY(!w.currentWidget());
    QVERIFY(!w.cssHelper());
}