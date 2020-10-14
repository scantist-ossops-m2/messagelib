/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "templateparseremailaddressrequesterlineedit.h"
#include <QHBoxLayout>
#include <QLineEdit>

using namespace TemplateParser;
TemplateParserEmailAddressRequesterLineEdit::TemplateParserEmailAddressRequesterLineEdit(QWidget *parent)
    : TemplateParser::TemplateParserEmailAddressRequesterBase(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins({});
    mLineEdit = new QLineEdit(this);
    mLineEdit->setObjectName(QStringLiteral("lineedit"));
    mainLayout->addWidget(mLineEdit);
    connect(mLineEdit, &QLineEdit::textChanged, this, &TemplateParserEmailAddressRequesterLineEdit::textChanged);
}

TemplateParserEmailAddressRequesterLineEdit::~TemplateParserEmailAddressRequesterLineEdit()
{
    disconnect(mLineEdit, &QLineEdit::textChanged, this, &TemplateParserEmailAddressRequesterLineEdit::textChanged);
}

QString TemplateParserEmailAddressRequesterLineEdit::text() const
{
    return mLineEdit->text();
}

void TemplateParserEmailAddressRequesterLineEdit::setText(const QString &str)
{
    mLineEdit->setText(str);
}

void TemplateParserEmailAddressRequesterLineEdit::clear()
{
    mLineEdit->clear();
}
