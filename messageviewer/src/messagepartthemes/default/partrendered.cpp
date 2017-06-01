/*
   Copyright (c) 2017 Sandro Knauß <sknauss@kde.org>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
   License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.
*/

#include "partrendered.h"

#include "defaultrenderer.h"
#include "defaultrenderer_p.h"
#include "htmlblock.h"
#include "autotests/util.h"
#include "autotests/setupenv.h"

#include "messagepartrenderermanager.h"

#include <MimeTreeParser/HtmlWriter>
#include <MimeTreeParser/MessagePart>

#include <MessageCore/StringUtil>

#include <grantlee/context.h>
#include <grantlee/template.h>

#include <QApplication>

#include <QDebug>

using namespace MessageViewer;

class CacheHtmlWriter : public MimeTreeParser::HtmlWriter
{
public:
    explicit CacheHtmlWriter() {}
    virtual ~CacheHtmlWriter() {}

    void begin(const QString &text) Q_DECL_OVERRIDE {}
    void write(const QString &str) Q_DECL_OVERRIDE {html.append(str);}
    void end() Q_DECL_OVERRIDE {}
    void reset() Q_DECL_OVERRIDE {}
    void queue(const QString &str) Q_DECL_OVERRIDE {html.append(str);}
    void flush() Q_DECL_OVERRIDE {}
    void embedPart(const QByteArray &contentId, const QString &url) Q_DECL_OVERRIDE {embedParts.insert(contentId, url);}
    void extraHead(const QString &extra) Q_DECL_OVERRIDE {head.append(extra);}

    QString html;
    QString head;
    QMap<QByteArray, QString> embedParts;
};

PartRendered::PartRendered()
{
}

PartRendered::~PartRendered()
{
}

inline QString PartRendered::alignText()
{
    return QApplication::isRightToLeft() ? QStringLiteral("rtl") : QStringLiteral("ltr");
}

QVector<QSharedPointer<PartRendered>> PartRendered::renderSubParts(MimeTreeParser::MessagePart::Ptr mp)
{
    Test::CSSHelper testCSSHelper;

    QVector<QSharedPointer<PartRendered>> ret;
    foreach (const auto &_m, mp->subParts()) {
        CacheHtmlWriter cacheWriter;
        DefaultRenderer::Ptr renderer = mp->source()->messagePartTheme(_m);
        cacheWriter.queue(renderer->html());
        ret.append(QSharedPointer<WrapperPartRendered>(new WrapperPartRendered(&cacheWriter)));
    }
    return ret;
}

WrapperPartRendered::WrapperPartRendered(CacheHtmlWriter *htmlWriter)
    : PartRendered()
{
    mHtml = htmlWriter->html;
    mHead = htmlWriter->head;
    mEmbeded = htmlWriter->embedParts;
}

WrapperPartRendered::~WrapperPartRendered()
{
}

QString WrapperPartRendered::html()
{
    return mHtml;
}

QString WrapperPartRendered::extraHeader()
{
    return mHead;
}

QMap<QByteArray, QString> WrapperPartRendered::embededParts()
{
    return mEmbeded;
}
