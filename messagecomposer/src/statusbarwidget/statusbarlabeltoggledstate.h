/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef STATUSBARLABELTOGGLEDSTATE_H
#define STATUSBARLABELTOGGLEDSTATE_H

#include <QLabel>
#include "messagecomposer_export.h"
namespace MessageComposer {
/**
 * @brief The StatusBarLabelToggledState class
 * @author Laurent Montel <montel@kde.org>
 */
class MESSAGECOMPOSER_EXPORT StatusBarLabelToggledState : public QLabel
{
    Q_OBJECT
public:
    explicit StatusBarLabelToggledState(QWidget *parent = nullptr);
    ~StatusBarLabelToggledState() override;

    void setToggleMode(bool state);

    Q_REQUIRED_RESULT bool toggleMode() const;

    void setStateString(const QString &toggled, const QString &untoggled);
Q_SIGNALS:
    void toggleModeChanged(bool state);

protected:
    void mousePressEvent(QMouseEvent *ev) override;

private:
    void updateLabel();
    QString mToggled;
    QString mUnToggled;
    bool mToggleMode = false;
};
}
#endif // STATUSBARLABELTOGGLEDSTATE_H
