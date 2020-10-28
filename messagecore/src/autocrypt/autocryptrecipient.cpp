/*
   SPDX-FileCopyrightText: 2020 Sandro Kanuß <sknauss@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autocryptrecipient.h"
#include "messagecore_debug.h"

#include<KMime/Headers>

#include <QJsonDocument>
#include <QJsonObject>
#include <gpgme++/data.h>
#include <QGpgME/DataProvider>
#include <QGpgME/Protocol>
#include <KCodecs>

using namespace MessageCore;

HeaderMixupNodeHelper::HeaderMixupNodeHelper (MimeTreeParser::NodeHelper* n, KMime::Content* m)
    : message(m)
    , nodeHelper(n)
{
}

QDateTime HeaderMixupNodeHelper::dateHeader() const
{
    return nodeHelper->dateHeader(message);
}

bool HeaderMixupNodeHelper::hasMailHeader (const char* header) const
{
    return nodeHelper->hasMailHeader(header, message);
}

QSharedPointer<KMime::Headers::Generics::AddressList> HeaderMixupNodeHelper::mailHeaderAsAddressList (const char* header) const
{
    return nodeHelper->mailHeaderAsAddressList(header, message);
}

const KMime::Headers::Base * HeaderMixupNodeHelper::mailHeaderAsBase (const char* header) const
{
    return nodeHelper->mailHeaderAsBase(header, message);
}


class AutocryptRecipientPrivate
{
public:
    AutocryptRecipientPrivate();
    QByteArray toJson(QJsonDocument::JsonFormat format) const;

public:
    QByteArray addr;
    QByteArray bad_user_agent;
    QByteArray keydata;
    QDateTime last_seen;
    QDateTime autocrypt_timestamp;
    QDateTime counting_since;
    int count_have_ach;
    int count_no_ach;
    bool prefer_encrypt;
};

AutocryptRecipientPrivate::AutocryptRecipientPrivate()
    : count_have_ach(0)
    , count_no_ach(0)
    , prefer_encrypt(false)
{
}

QByteArray AutocryptRecipientPrivate::toJson (QJsonDocument::JsonFormat format) const
{
    QJsonObject entry;
    entry.insert(QStringLiteral("addr"), QString::fromLatin1(addr));
    entry.insert(QStringLiteral("prefer_encrypt"), prefer_encrypt);
    entry.insert(QStringLiteral("keydata"), QString::fromLatin1(keydata));
    entry.insert(QStringLiteral("autocrypt_timestamp"), autocrypt_timestamp.toString(Qt::ISODate));
    entry.insert(QStringLiteral("count_have_ach"), count_have_ach);
    entry.insert(QStringLiteral("count_no_ach"), count_no_ach);
    if (last_seen.isValid()) {
        entry.insert(QStringLiteral("last_seen"), last_seen.toString(Qt::ISODate));
    }
    if (counting_since.isValid()) {
        entry.insert(QStringLiteral("counting_since"), counting_since.toString(Qt::ISODate));
    }
    if (!bad_user_agent.isEmpty()) {
        entry.insert(QStringLiteral("bad_user_agent"), QString::fromLatin1(bad_user_agent));
    }
    QJsonDocument document;
    document.setObject( entry );
    return document.toJson(format);
}


AutocryptRecipient::AutocryptRecipient()
    : d_ptr(new AutocryptRecipientPrivate())
{
}

void AutocryptRecipient::updateFromMessage ( const HeaderMixupNodeHelper& mixup )
{
    Q_D(AutocryptRecipient);
    QDateTime effectiveDate = mixup.dateHeader();

    if (effectiveDate > QDateTime::currentDateTime()) {
        return;
    }

    if (d->autocrypt_timestamp.isValid() && effectiveDate <= d->autocrypt_timestamp) {
        return;
    }
    d->autocrypt_timestamp = effectiveDate;

    if (!d->counting_since.isValid()) {
        d->counting_since = effectiveDate;
        d->count_have_ach = 0;
        d->count_no_ach = 0;
    }

    if (mixup.hasMailHeader("Autocrypt")) {
        const auto &header = mixup.mailHeaderAsBase("Autocrypt")->as7BitString(false);
        const auto &parts = header.split(';');
        QHash<QByteArray,QByteArray> params;
        for(const auto &part: parts) {
            const auto &i = part.split('=');
            params[i[0].trimmed()] = i[1].trimmed();
        }
        if (d->addr.isEmpty()) {
            d->addr = params["addr"];
        }
        d->prefer_encrypt = params.contains("prefer-encrypt");
        d->keydata = params["keydata"].replace(' ', QByteArray());

        d->last_seen = effectiveDate;
        d->count_have_ach += 1;
    } else {
        d->count_no_ach += 1;
        if (mixup.hasMailHeader("User-Agent")) {
            d->bad_user_agent = mixup.mailHeaderAsBase("User-Agent")->as7BitString(false);
        }
    }
}

QByteArray AutocryptRecipient::toJson ( QJsonDocument::JsonFormat format ) const
{
    const Q_D(AutocryptRecipient);
    return d->toJson(format);
}

GpgME::Key MessageCore::AutocryptRecipient::gpgKey()
{
    const Q_D(AutocryptRecipient);
    assert(QGpgME::openpgp());      // Make sure, that openpgp backend is loaded
    auto context = GpgME::Context::create(GpgME::OpenPGP);
    QGpgME::QByteArrayDataProvider dp(KCodecs::base64Decode(d->keydata));
    GpgME::Data data(&dp);
    auto keys = data.toKeys();
    if (keys.size() == 0) {
        return GpgME::Key();
    }
    return keys[0];
}
