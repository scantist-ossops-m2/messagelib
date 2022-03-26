/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KTextToHTML>
#include <QString>
#include <QVariantHash>

#include "messageviewer/spamheaderanalyzer.h"
#include <KMime/DateFormatter>
#include <KMime/KMimeMessage>

#include "messageviewer/headerstyle.h"
#include "messageviewer/viewer.h"
#include "messageviewer_export.h"
namespace MessageViewer
{
/**
 * @brief The HeaderStyleUtil class
 * @author Laurent Montel <montel@kde.org>
 */
class MESSAGEVIEWER_EXPORT HeaderStyleUtil
{
public:
    HeaderStyleUtil();
    struct xfaceSettings {
        xfaceSettings()
            : photoWidth(60)
            , photoHeight(60)
        {
        }

        QString photoURL;
        int photoWidth;
        int photoHeight;
    };
    enum HeaderStyleUtilDateFormat {
        ShortDate, /**< Locale Short date format, e.g. 08-04-2007 */
        LongDate, /**< Locale Long date format, e.g. Sunday 08 April 2007 */
        FancyShortDate, /**< Same as ShortDate for dates a week or more ago. For more
                             recent dates, it is represented as Today, Yesterday, or
                             the weekday name. */
        FancyLongDate, /**< Same as LongDate for dates a week or more ago. For more
                            recent dates, it is represented as Today, Yesterday, or
                            the weekday name. */
        CustomDate
    };

    Q_REQUIRED_RESULT QString directionOf(const QString &str) const;

    Q_REQUIRED_RESULT static QString strToHtml(const QString &str, KTextToHTML::Options flags = KTextToHTML::PreserveSpaces);
    Q_REQUIRED_RESULT static QString dateString(KMime::Message *message, HeaderStyleUtilDateFormat dateFormat);
    Q_REQUIRED_RESULT static QString dateString(const QDateTime &date, HeaderStyleUtilDateFormat dateFormat);

    Q_REQUIRED_RESULT QString subjectString(KMime::Message *message, KTextToHTML::Options flags = KTextToHTML::PreserveSpaces) const;

    Q_REQUIRED_RESULT QString subjectDirectionString(KMime::Message *message) const;

    Q_REQUIRED_RESULT QString spamStatus(KMime::Message *message) const;

    Q_REQUIRED_RESULT static QString dateStr(const QDateTime &dateTime);

    Q_REQUIRED_RESULT static QString dateShortStr(const QDateTime &dateTime);

    MESSAGEVIEWER_DEPRECATED Q_REQUIRED_RESULT static QSharedPointer<KMime::Headers::Generics::MailboxList> resentFromList(KMime::Message *message);

    MESSAGEVIEWER_DEPRECATED Q_REQUIRED_RESULT static QSharedPointer<KMime::Headers::Generics::MailboxList> resentToList(KMime::Message *message);

    Q_REQUIRED_RESULT xfaceSettings xface(const HeaderStyle *style, KMime::Message *message) const;

private:
    void updateXFaceSettings(QImage photo, xfaceSettings &settings) const;
    Q_REQUIRED_RESULT QString
    drawSpamMeter(SpamError spamError, double percent, double confidence, const QString &filterHeader, const QString &confidenceHeader) const;
    Q_REQUIRED_RESULT QString imgToDataUrl(const QImage &image) const;
};
}
