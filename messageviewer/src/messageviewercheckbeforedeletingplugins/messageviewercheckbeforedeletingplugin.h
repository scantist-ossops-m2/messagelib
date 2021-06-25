/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include "messageviewer_export.h"
namespace MessageViewer
{
class MessageViewerCheckBeforeDeletingPluginPrivate;
/**
 * @brief The MessageViewerCheckBeforeDeletingPlugin class
 * @author Laurent Montel <montel@kde.org>
 */
class MESSAGEVIEWER_EXPORT MessageViewerCheckBeforeDeletingPlugin : public QObject
{
    Q_OBJECT
public:
    explicit MessageViewerCheckBeforeDeletingPlugin(QObject *parent = nullptr);
    ~MessageViewerCheckBeforeDeletingPlugin();

    virtual void showConfigureDialog(QWidget *parent);

Q_SIGNALS:
    void configChanged();

private:
    MessageViewerCheckBeforeDeletingPluginPrivate *const d;
};
}
