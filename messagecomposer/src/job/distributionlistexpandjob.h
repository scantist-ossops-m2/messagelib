/*
 * This file is part of KMail.
 *
 * Copyright (c) 2010 KDAB
 *
 * Author: Tobias Koenig <tokoe@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef DISTRIBUTIONLISTEXPANDJOB_H
#define DISTRIBUTIONLISTEXPANDJOB_H

#include <KJob>
#include <QStringList>
namespace MessageComposer {
/**
 * @short A job to expand a distribution list to its member email addresses.
 */
class DistributionListExpandJob : public KJob
{
    Q_OBJECT

public:
    /**
     * Creates a new distribution list expand job.
     *
     * @param name The name of the distribution list to expand.
     * @param parent The parent object.
     */
    explicit DistributionListExpandJob(const QString &name, QObject *parent = nullptr);

    /**
     * Destroys the distribution list expand job.
     */
    ~DistributionListExpandJob() override;

    /**
     * Starts the job.
     */
    void start() override;

    /**
     * Returns the email addresses of the list members.
     */
    Q_REQUIRED_RESULT QString addresses() const;

    /**
     * Returns whether the list of email addresses is empty.
     */
    Q_REQUIRED_RESULT bool isEmpty() const;

private Q_SLOTS:
    void slotSearchDone(KJob *);
    void slotExpansionDone(KJob *);

private:
    QString mListName;
    QStringList mEmailAddresses;
    bool mIsEmpty = false;
};
}
#endif
