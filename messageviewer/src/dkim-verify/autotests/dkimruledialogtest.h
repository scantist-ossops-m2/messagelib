/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DKIMRuleDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit DKIMRuleDialogTest(QObject *parent = nullptr);
    ~DKIMRuleDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
