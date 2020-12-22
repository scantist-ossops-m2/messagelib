/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MAILNETWORKURLINTERCEPTORPLUGINMANAGER_H
#define MAILNETWORKURLINTERCEPTORPLUGINMANAGER_H

#include <QObject>
#include <QVector>
#include <PimCommon/PluginUtil>
#include "webengineviewer_export.h"
namespace WebEngineViewer {
class NetworkUrlInterceptorPluginManagerPrivate;
class NetworkPluginUrlInterceptor;
/**
 * @brief The NetworkUrlInterceptorPluginManager class
 * @author Laurent Montel <montel@kde.org>
 */
class WEBENGINEVIEWER_EXPORT NetworkUrlInterceptorPluginManager : public QObject
{
    Q_OBJECT
public:
    static NetworkUrlInterceptorPluginManager *self();
    explicit NetworkUrlInterceptorPluginManager(QObject *parent = nullptr);
    ~NetworkUrlInterceptorPluginManager() override;

    Q_REQUIRED_RESULT QVector<WebEngineViewer::NetworkPluginUrlInterceptor *> pluginsList() const;
    Q_REQUIRED_RESULT QString configGroupName() const;
    Q_REQUIRED_RESULT QString configPrefixSettingKey() const;
    Q_REQUIRED_RESULT QVector<PimCommon::PluginUtilData> pluginsDataList() const;
    Q_REQUIRED_RESULT WebEngineViewer::NetworkPluginUrlInterceptor *pluginFromIdentifier(const QString &id);
private:
    NetworkUrlInterceptorPluginManagerPrivate *const d;
};
}

#endif // MAILNETWORKURLINTERCEPTORPLUGINMANAGER_H
