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
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString wsp_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString vchar_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString letDig_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString ldhStr_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString keyword_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString subDomain_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString obsFws_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString quotedPair_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString fws_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString fws_op();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString ctext_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString ccontent_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString comment_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString cfws_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString cfws_op();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString atext();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString dotAtomText_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString dotAtom_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString qtext_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString qcontent_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString quotedString_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString quotedString_cp();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString localPart_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString token_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString value_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString value_cp();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString domainName_p();
Q_REQUIRED_RESULT MESSAGEVIEWER_TESTS_EXPORT QString regexMatchO(const QString &regularExpressionStr);
}
}
