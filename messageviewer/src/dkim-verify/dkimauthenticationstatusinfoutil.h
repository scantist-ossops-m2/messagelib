/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messageviewer_private_export.h"
#include <QString>

namespace MessageViewer
{
namespace DKIMAuthenticationStatusInfoUtil
{
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString wsp_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString vchar_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString letDig_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString ldhStr_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString keyword_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString subDomain_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString obsFws_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString quotedPair_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString fws_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString fws_op();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString ctext_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString ccontent_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString comment_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString cfws_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString cfws_op();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString atext();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString dotAtomText_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString dotAtom_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString qtext_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString qcontent_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString quotedString_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString quotedString_cp();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString localPart_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString token_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString value_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString value_cp();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString domainName_p();
MESSAGEVIEWER_TESTS_EXPORT Q_REQUIRED_RESULT QString regexMatchO(const QString &regularExpressionStr);
}
}
