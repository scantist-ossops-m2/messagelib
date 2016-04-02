/*
  Copyright (c) 2016 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "webenginescript.h"
#include <QDebug>

using namespace MessageViewer;

QString WebEngineScript::findAllImages()
{
    const QString source = QLatin1String("(function() {"
                                         "var out = [];"
                                         "var imgs = document.getElementsByTagName('img');"
                                         "for (var i = 0; i < imgs.length; ++i) {"
                                         "    var e = imgs[i];"
                                         "    out.push({"
                                         "        src: e.src"
                                         "    });"
                                         "}"
                                         "return out;"
                                         "})()");

    return source;
}

QString WebEngineScript::findAllScripts()
{
    const QString source = QLatin1String("(function() {"
                                         "var out = [];"
                                         "var scripts = document.getElementsByTagName('script');"
                                         "for (var i = 0; i < scripts.length; ++i) {"
                                         "    var e = scripts[i];"
                                         "    out.push({"
                                         "        src: e.src"
                                         "    });"
                                         "}"
                                         "return out;"
                                         "})()");

    return source;
}

QString WebEngineScript::findAllAnchors()
{
    const QString source = QLatin1String("(function() {"
                                         "var out = [];"
                                         "var anchors = document.getElementsByTagName('a');"
                                         "for (var i = 0; i < anchors.length; ++i) {"
                                         "    var r = anchors[i].getBoundingClientRect();"
                                         "    out.push({"
                                         "        src: anchors[i].href,"
                                         "        title: anchors[i].title,"
                                         "        boudingRect: [r.top, r.left, r.width, r.height]"
                                         "    });"
                                         "}"
                                         "return out;"
                                         "})()");

    return source;
}

QString WebEngineScript::findAllAnchorsAndForms()
{
    const QString source = QLatin1String("(function() {"
                                         "var res = [];"
                                         "var out = [];"
                                         "var anchors = document.getElementsByTagName('a');"
                                         "for (var i = 0; i < anchors.length; ++i) {"
                                         "    out.push({"
                                         "        src: anchors[i].href,"
                                         "        title: anchors[i].title"
                                         "    });"
                                         "}"
                                         "var forms = document.getElementsByTagName('form');"
                                         "res.push({"
                                         "    anchors: out,"
                                         "    forms:  forms.length"
                                         "    });"
                                         "return res;"
                                         "})()");

    return source;
}

QString WebEngineScript::setElementByIdVisible(const QString &elementStr, bool visibility)
{
    if (visibility) {
        const QString source = QString::fromLatin1("var element = document.getElementById('%1'); "
                               "if (element) { "
                               "    element.style.removeProperty( 'display' );"
                               "}").arg(elementStr);
        return source;
    } else {
        const QString source = QString::fromLatin1("var element = document.getElementById('%1'); "
                               "if (element) { "
                               "    element.style.display = \"none\";"
                               "}").arg(elementStr);
        return source;
    }
}

QString WebEngineScript::searchElementPosition(const QString &elementStr)
{
    const QString source = QString::fromLatin1("var element = document.getElementById('%1'); "
                           "if (element) { "
                           "    var geometry = element.getBoundingClientRect(); "
                           "    [(geometry.left + window.scrollX), (geometry.top + window.scrollY)]; "
                           "}").arg(elementStr);
    //qDebug()<<" source "<<source;
    return source;
}

QString WebEngineScript::scrollPercentage(int percent)
{
    //TODO
    return QString();
}

QString WebEngineScript::scrollUp(int pixel)
{
    const QString source = QString::fromLatin1("window.scrollBy(0, %1);").arg(-pixel);
    return source;
}

QString WebEngineScript::scrollDown(int pixel)
{
    const QString source = QString::fromLatin1("window.scrollBy(0, %1);").arg(pixel);
    return source;
}

QString WebEngineScript::scrollToPosition(const QPoint &pos)
{
    const QString source = QString::fromLatin1("window.scrollTo(%1, %2); [window.scrollX, window.scrollY];").arg(pos.x()).arg(pos.y());
    //qDebug()<<" source "<<source;
    return source;
}

QString WebEngineScript::removeStyleToElement(const QString &elementStr)
{
    const QString source = QString::fromLatin1("var element = document.getElementById('%1'); "
                                               "if (element) { "
                                               "    element.removeAttribute(\"style\");"
                                               "}").arg(elementStr);
    return source;
}

QString WebEngineScript::setStyleToElement(const QString &elementStr, const QString &style)
{
    const QString source = QString::fromLatin1("var element = document.getElementById('%1'); "
                           "if (element) { "
                           "    element.style = '%2';"
                           "}").arg(elementStr).arg(style);
    return source;
}

QString WebEngineScript::scrollToRelativePosition(int pos)
{
    //TODO verify it
    const QString source = QString::fromLatin1("window.scrollTo(window.scrollX, %1); [window.scrollX, window.scrollY];").arg(pos);
    return source;
}

QString WebEngineScript::replaceInnerHtml()
{
#if 0
    // First inject the correct icon
    if (mViewer->replaceInnerHtml(QLatin1String("iconFull") + field + QLatin1String("AddressList"),
                                  bind(&ViewerPrivate::recipientsQuickListLinkHtml, this, doShow, field))) {
        // Then show/hide the full address list
        mViewer->setElementByIdVisible(QLatin1String("dotsFull")   + field + QLatin1String("AddressList"), !doShow);
        mViewer->setElementByIdVisible(QLatin1String("hiddenFull") + field + QLatin1String("AddressList"),  doShow);
    }
#endif
    //TODO
    return {};
}

QString WebEngineScript::injectAttachments(const QString &delayedHtml, const QString &elementStr)
{
    const QString source = QString::fromLatin1("var element = document.getElementById('%1'); "
                           "if (element) { "
                           "    element.innerHTML += '%2';"
                           "}").arg(elementStr).arg(delayedHtml);
    qDebug() << "QString WebEngineScript::injectAttachments(const QString &delayedHtml, const QString &elementStr) :"<<source;
    return source;
}

QString WebEngineScript::toggleFullAddressList(const QString &field, const QString &html, bool doShow)
{
#if 0
    if (mViewer->replaceInnerHtml(QLatin1String("iconFull") + field + QLatin1String("AddressList"),
                                  bind(&ViewerPrivate::recipientsQuickListLinkHtml, this, doShow, field))) {
        // Then show/hide the full address list
        mViewer->setElementByIdVisible(QLatin1String("dotsFull")   + field + QLatin1String("AddressList"), !doShow);
        mViewer->setElementByIdVisible(QLatin1String("hiddenFull") + field + QLatin1String("AddressList"),  doShow);
    }
    qDebug() << "bool MailWebEngineView::replaceInnerHtml(const QString &id, const boost::function<QString()> &delayedHtml)";
    QWebElement doc = page()->currentFrame()->documentElement();
    QWebElement tag = doc.findFirst(QLatin1String("*#") + id);
    if (tag.isNull()) {
        return false;
    }
    tag.setInnerXml(delayedHtml());
    return true;
#endif
    //TODO
    return {};
}
