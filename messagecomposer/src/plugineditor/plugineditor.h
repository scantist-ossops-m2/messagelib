/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PLUGINEDITOR_H
#define PLUGINEDITOR_H
#include <QObject>
#include "messagecomposer_export.h"
#include <PimCommon/AbstractGenericPlugin>
namespace MessageComposer {
class PluginEditorPrivate;
/**
 * @brief The PluginEditor class
 * @author Laurent Montel <montel@kde.org>
 */
class MESSAGECOMPOSER_EXPORT PluginEditor : public PimCommon::AbstractGenericPlugin
{
    Q_OBJECT
public:
    explicit PluginEditor(QObject *parent = nullptr);
    ~PluginEditor() override;
    void setOrder(int order);
    Q_REQUIRED_RESULT int order() const;

    virtual Q_REQUIRED_RESULT bool canProcessKeyEvent() const;
private:
    PluginEditorPrivate *const d;
};
}
#endif // PLUGINEDITOR_H
