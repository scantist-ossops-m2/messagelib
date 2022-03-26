/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "mimetreeparser_export.h"
#include <QObject>
#include <QStringList>
#include <memory>
namespace MimeTreeParser
{
class AttachmentTemporaryFilesDirsPrivate;
/**
 * @brief The AttachmentTemporaryFilesDirs class
 * @author Laurent Montel <montel@kde.org>
 */
class MIMETREEPARSER_EXPORT AttachmentTemporaryFilesDirs : public QObject
{
    Q_OBJECT
public:
    explicit AttachmentTemporaryFilesDirs(QObject *parent = nullptr);
    ~AttachmentTemporaryFilesDirs() override;

    void addTempFile(const QString &file);
    void addTempDir(const QString &dir);
    Q_REQUIRED_RESULT QStringList temporaryFiles() const;
    void removeTempFiles();
    void forceCleanTempFiles();

    Q_REQUIRED_RESULT QStringList temporaryDirs() const;

    void setDelayRemoveAllInMs(int ms);

private Q_SLOTS:
    void slotRemoveTempFiles();

private:
    std::unique_ptr<AttachmentTemporaryFilesDirsPrivate> const d;
};
}
