/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEMPLATEPARSEREMAILADDRESSREQUESTERBASE_H
#define TEMPLATEPARSEREMAILADDRESSREQUESTERBASE_H

#include <QWidget>
#include "templateparser_export.h"
namespace TemplateParser {
/**
 * @brief The TemplateParserEmailAddressRequesterBase class
 * @author Laurent Montel <montel@kde.org>
 */
class TEMPLATEPARSER_EXPORT TemplateParserEmailAddressRequesterBase : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateParserEmailAddressRequesterBase(QWidget *parent = nullptr);
    ~TemplateParserEmailAddressRequesterBase() override;

    virtual Q_REQUIRED_RESULT QString text() const;
    virtual void setText(const QString &str);
    virtual void clear();
Q_SIGNALS:
    void textChanged();
};
}

#endif // TEMPLATEPARSEREMAILADDRESSREQUESTERBASE_H
