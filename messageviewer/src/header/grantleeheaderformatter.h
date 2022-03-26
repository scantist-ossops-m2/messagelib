/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messageviewer_export.h"
#include <GrantleeTheme/GrantleeTheme>
#include <QString>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <grantlee/templateloader.h>
#else
#include <KTextTemplate/templateloader.h>
#endif
namespace KMime
{
class Message;
}

namespace MessageViewer
{
class HeaderStyle;
/**
 * @brief The GrantleeHeaderFormatter class
 * @author Laurent Montel <montel@kde.org>
 */
class MESSAGEVIEWER_EXPORT GrantleeHeaderFormatter
{
public:
    struct GrantleeHeaderFormatterSettings {
        GrantleeHeaderFormatterSettings() = default;

        GrantleeTheme::Theme theme;
        bool isPrinting = false;
        mutable const MessageViewer::HeaderStyle *style = nullptr;
        KMime::Message *message = nullptr;
        bool showEmoticons = true;
    };

    explicit GrantleeHeaderFormatter();
    ~GrantleeHeaderFormatter();

    Q_REQUIRED_RESULT QString toHtml(const GrantleeHeaderFormatterSettings &settings) const;

    Q_REQUIRED_RESULT QString toHtml(const QStringList &displayExtraHeaders,
                                     const QString &absolutPath,
                                     const QString &filename,
                                     const MessageViewer::HeaderStyle *style,
                                     KMime::Message *message,
                                     bool isPrinting) const;

private:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Q_REQUIRED_RESULT QString format(const QString &absolutePath,
                                     const Grantlee::Template &headerTemplate,
                                     const QStringList &displayExtraHeaders,
                                     bool isPrinting,
                                     const MessageViewer::HeaderStyle *style,
                                     KMime::Message *message,
                                     bool showEmoticons = true) const;
#else
    Q_REQUIRED_RESULT QString format(const QString &absolutePath,
                                     const KTextTemplate::Template &headerTemplate,
                                     const QStringList &displayExtraHeaders,
                                     bool isPrinting,
                                     const MessageViewer::HeaderStyle *style,
                                     KMime::Message *message,
                                     bool showEmoticons = true) const;
#endif
    class GrantleeHeaderFormatterPrivate;
    std::unique_ptr<GrantleeHeaderFormatterPrivate> const d;
};
}
