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

#include "loadexternalreferencesurlinterceptor.h"

#include <QWebEngineUrlRequestInfo>

using namespace MessageViewer;

class MessageViewer::LoadExternalReferencesUrlInterceptorPrivate
{
public:
    LoadExternalReferencesUrlInterceptorPrivate()
        : mAllowLoadExternalReference(false)
    {

    }
    bool mAllowLoadExternalReference;
};

LoadExternalReferencesUrlInterceptor::LoadExternalReferencesUrlInterceptor(QObject *parent)
    : WebEngineViewer::NetworkPluginUrlInterceptorInterface(parent),
      d(new LoadExternalReferencesUrlInterceptorPrivate)
{

}

LoadExternalReferencesUrlInterceptor::~LoadExternalReferencesUrlInterceptor()
{
    delete d;
}

bool LoadExternalReferencesUrlInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    if (d->mAllowLoadExternalReference) {
        return false;
    } else {
        if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeImage
                && !info.requestUrl().isLocalFile()
                && (info.requestUrl().scheme() != QLatin1String("cid"))) {
            return true;
        } else if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMedia) {
            return true;
        }
    }
    return false;
}

void LoadExternalReferencesUrlInterceptor::setAllowExternalContent(bool b)
{
    d->mAllowLoadExternalReference = b;
}

bool LoadExternalReferencesUrlInterceptor::allowExternalContent() const
{
    return d->mAllowLoadExternalReference;
}
