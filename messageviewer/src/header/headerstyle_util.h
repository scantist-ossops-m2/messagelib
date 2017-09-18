/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef HEADERSTYLE_UTIL_H
#define HEADERSTYLE_UTIL_H

#include <QString>
#include <QVariantHash>
#include <KTextToHTML>

#include <kmime/kmime_message.h>
#include <kmime/kmime_dateformatter.h>
#include "messageviewer/spamheaderanalyzer.h"

#include "messageviewer/headerstyle.h"
#include "messageviewer/viewer.h"
#include "messageviewer_export.h"
namespace MessageViewer {
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
        ShortDate,        /**< Locale Short date format, e.g. 08-04-2007 */
        LongDate,         /**< Locale Long date format, e.g. Sunday 08 April 2007 */
        FancyShortDate,   /**< Same as ShortDate for dates a week or more ago. For more
                               recent dates, it is represented as Today, Yesterday, or
                               the weekday name. */
        FancyLongDate,    /**< Same as LongDate for dates a week or more ago. For more
                               recent dates, it is represented as Today, Yesterday, or
                               the weekday name. */
        CustomDate
    };

    QString directionOf(const QString &str) const;

    QString strToHtml(const QString &str, KTextToHTML::Options flags = KTextToHTML::PreserveSpaces) const;

    QString dateString(KMime::Message *message, bool printing, HeaderStyleUtilDateFormat dateFormat) const;

    QString subjectString(KMime::Message *message, KTextToHTML::Options flags = KTextToHTML::PreserveSpaces) const;

    QString subjectDirectionString(KMime::Message *message) const;

    QString spamStatus(KMime::Message *message) const;

    QString dateStr(const QDateTime &dateTime) const;

    QString dateShortStr(const QDateTime &dateTime) const;

    QVector<KMime::Types::Mailbox> resentFromList(KMime::Message *message) const;
    QVector<KMime::Types::Mailbox> resentToList(KMime::Message *message) const;

    xfaceSettings xface(const HeaderStyle *style, KMime::Message *message) const;
private:
    void updateXFaceSettings(QImage photo, xfaceSettings &settings) const;
    QString drawSpamMeter(SpamError spamError, double percent, double confidence, const QString &filterHeader, const QString &confidenceHeader) const;
    QString imgToDataUrl(const QImage &image) const;
    int mIconSize;
};
}

#endif // HEADERSTYLE_UTIL_H
