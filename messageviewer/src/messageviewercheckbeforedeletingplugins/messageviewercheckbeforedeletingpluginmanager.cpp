/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageviewercheckbeforedeletingpluginmanager.h"
#include "messageviewer_debug.h"
#include "messageviewercheckbeforedeletingplugin.h"
#include <KPluginFactory>
#include <KPluginLoader>
#include <KPluginMetaData>
#include <QFileInfo>
#include <QSet>

using namespace MessageViewer;

class ConfigureSettingsPluginInfo
{
public:
    ConfigureSettingsPluginInfo()
    {
    }

    PimCommon::PluginUtilData pluginData;
    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    MessageViewer::MessageViewerCheckBeforeDeletingPlugin *plugin = nullptr;
};

class MessageViewer::MessageViewerCheckBeforeDeletingPluginManagerPrivate
{
public:
    explicit MessageViewerCheckBeforeDeletingPluginManagerPrivate(MessageViewerCheckBeforeDeletingPluginManager *qq)
        : q(qq)
    {
    }

    QVector<MessageViewer::MessageViewerCheckBeforeDeletingPlugin *> pluginsList() const;
    QVector<PimCommon::PluginUtilData> pluginDataList() const;
    void initializePluginList();
    void loadPlugin(ConfigureSettingsPluginInfo *item);
    QString configGroupName() const;
    QString configPrefixSettingKey() const;
    MessageViewerCheckBeforeDeletingPlugin *pluginFromIdentifier(const QString &id);

private:
    QVector<PimCommon::PluginUtilData> mPluginDataList;
    QVector<ConfigureSettingsPluginInfo> mPluginList;
    MessageViewerCheckBeforeDeletingPluginManager *const q;
};

namespace
{
QString pluginVersion()
{
    return QStringLiteral("1.0");
}
}

QVector<PimCommon::PluginUtilData> MessageViewerCheckBeforeDeletingPluginManagerPrivate::pluginDataList() const
{
    return mPluginDataList;
}

QString MessageViewerCheckBeforeDeletingPluginManagerPrivate::configGroupName() const
{
    return QStringLiteral("MessageViewerCheckBeforeDeletingPlugins");
}

QString MessageViewerCheckBeforeDeletingPluginManagerPrivate::configPrefixSettingKey() const
{
    return QStringLiteral("MessageViewerCheckBeforeDeletingPlugin");
}

void MessageViewerCheckBeforeDeletingPluginManagerPrivate::initializePluginList()
{
    const QVector<KPluginMetaData> plugins = KPluginLoader::findPlugins(QStringLiteral("messageviewer/checkbeforedeleting"));

    QVectorIterator<KPluginMetaData> i(plugins);
    i.toBack();
    const QPair<QStringList, QStringList> pair = PimCommon::PluginUtil::loadPluginSetting(configGroupName(), configPrefixSettingKey());
    QVector<int> listOrder;
    while (i.hasPrevious()) {
        ConfigureSettingsPluginInfo info;

        const KPluginMetaData data = i.previous();

        // 1) get plugin data => name/description etc.
        info.pluginData = PimCommon::PluginUtil::createPluginMetaData(data);
        // 2) look at if plugin is activated
        info.metaDataFileNameBaseName = QFileInfo(data.fileName()).baseName();
        info.metaDataFileName = data.fileName();
        const QString version = data.version();
        if (pluginVersion() == version) {
            const QVariant p = data.rawData().value(QStringLiteral("X-KDE-MessageViewer-Configure-Order")).toVariant();
            int order = -1;
            if (p.isValid()) {
                order = p.toInt();
            }
            int pos = 0;
            for (; pos < listOrder.count(); ++pos) {
                if (listOrder.at(pos) > order) {
                    pos--;
                    break;
                }
            }
            pos = qMax(0, pos);
            listOrder.insert(pos, order);
            info.plugin = nullptr;
            mPluginList.insert(pos, info);
        } else {
            qCWarning(MESSAGEVIEWER_LOG) << "Plugin " << data.name() << " doesn't have correction plugin version. It will not be loaded.";
        }
    }
    QVector<ConfigureSettingsPluginInfo>::iterator end(mPluginList.end());
    for (QVector<ConfigureSettingsPluginInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
}

QVector<MessageViewer::MessageViewerCheckBeforeDeletingPlugin *> MessageViewerCheckBeforeDeletingPluginManagerPrivate::pluginsList() const
{
    QVector<MessageViewer::MessageViewerCheckBeforeDeletingPlugin *> lst;
    QVector<ConfigureSettingsPluginInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<ConfigureSettingsPluginInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

void MessageViewerCheckBeforeDeletingPluginManagerPrivate::loadPlugin(ConfigureSettingsPluginInfo *item)
{
    KPluginLoader pluginLoader(item->metaDataFileName);
    if (pluginLoader.factory()) {
        item->plugin =
            pluginLoader.factory()->create<MessageViewer::MessageViewerCheckBeforeDeletingPlugin>(q, QVariantList() << item->metaDataFileNameBaseName);
        // By default it's true
        item->pluginData.mHasConfigureDialog = true;
        mPluginDataList.append(item->pluginData);
    }
}

MessageViewerCheckBeforeDeletingPlugin *MessageViewerCheckBeforeDeletingPluginManagerPrivate::pluginFromIdentifier(const QString &id)
{
    QVector<ConfigureSettingsPluginInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<ConfigureSettingsPluginInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if ((*it).pluginData.mIdentifier == id) {
            return (*it).plugin;
        }
    }
    return {};
}

MessageViewerCheckBeforeDeletingPluginManager *MessageViewerCheckBeforeDeletingPluginManager::self()
{
    static MessageViewerCheckBeforeDeletingPluginManager s_self;
    return &s_self;
}

MessageViewerCheckBeforeDeletingPluginManager::MessageViewerCheckBeforeDeletingPluginManager(QObject *parent)
    : QObject(parent)
    , d(new MessageViewer::MessageViewerCheckBeforeDeletingPluginManagerPrivate(this))
{
    d->initializePluginList();
}

MessageViewerCheckBeforeDeletingPluginManager::~MessageViewerCheckBeforeDeletingPluginManager()
{
    delete d;
}

QVector<MessageViewer::MessageViewerCheckBeforeDeletingPlugin *> MessageViewerCheckBeforeDeletingPluginManager::pluginsList() const
{
    return d->pluginsList();
}

QString MessageViewerCheckBeforeDeletingPluginManager::configGroupName() const
{
    return d->configGroupName();
}

QString MessageViewerCheckBeforeDeletingPluginManager::configPrefixSettingKey() const
{
    return d->configPrefixSettingKey();
}

QVector<PimCommon::PluginUtilData> MessageViewerCheckBeforeDeletingPluginManager::pluginsDataList() const
{
    return d->pluginDataList();
}

MessageViewerCheckBeforeDeletingPlugin *MessageViewerCheckBeforeDeletingPluginManager::pluginFromIdentifier(const QString &id)
{
    return d->pluginFromIdentifier(id);
}
