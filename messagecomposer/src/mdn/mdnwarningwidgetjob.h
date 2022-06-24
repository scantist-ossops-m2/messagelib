/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messagecomposer_export.h"
#include <Akonadi/Item>
#include <QObject>
namespace MessageViewer
{
/**
 * @brief The MDNWarningWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class MESSAGECOMPOSER_EXPORT MDNWarningWidgetJob : public QObject
{
    Q_OBJECT
public:
    explicit MDNWarningWidgetJob(QObject *parent = nullptr);
    ~MDNWarningWidgetJob() override;

    void start();

    Q_REQUIRED_RESULT const Akonadi::Item &item() const;
    void setItem(const Akonadi::Item &newItem);

    Q_REQUIRED_RESULT bool canStart() const;

Q_SIGNALS:
    void showMdnInfo(const QPair<QString, bool> &mdnInfo);

private:
    Akonadi::Item mItem;
};
}
