/*
  SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VIEWERPURPOSEMENUWIDGET_H
#define VIEWERPURPOSEMENUWIDGET_H

#include <PimCommon/PurposeMenuWidget>
namespace MessageViewer {
class MailWebEngineView;
class MailfilterPurposeMenuWidget : public PimCommon::PurposeMenuWidget
{
    Q_OBJECT
public:
    explicit MailfilterPurposeMenuWidget(QWidget *parentWidget, QObject *parent = nullptr);
    ~MailfilterPurposeMenuWidget() override;

    Q_REQUIRED_RESULT QByteArray text() override;
    void setViewer(MailWebEngineView *viewer);

private:
    MailWebEngineView *mViewer = nullptr;
};
}

#endif