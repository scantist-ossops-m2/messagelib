/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "dkimmanagerulescombobox.h"
#include "dkimmanageruleswidget.h"
#include "dkimruledialog.h"
#include "messageviewer_dkimcheckerdebug.h"
#include <MessageViewer/DKIMManagerRules>
#include <QVBoxLayout>
#include <QHeaderView>
#include <KLocalizedString>
#include <QTreeWidget>
#include <KTreeWidgetSearchLine>
#include <KMessageBox>
#include <QMenu>
#include <QPointer>
using namespace MessageViewer;
DKIMManageRulesWidgetItem::DKIMManageRulesWidgetItem(QTreeWidget *parent)
    : QTreeWidgetItem(parent)
{
    mRuleTypeCombobox = new DKIMManageRulesComboBox;
    treeWidget()->setItemWidget(this, ColumnType::RuleType, mRuleTypeCombobox);
}

DKIMManageRulesWidgetItem::~DKIMManageRulesWidgetItem()
{
}

MessageViewer::DKIMRule DKIMManageRulesWidgetItem::rule() const
{
    return mRule;
}

void DKIMManageRulesWidgetItem::setRule(const MessageViewer::DKIMRule &rule)
{
    if (mRule != rule) {
        mRule = rule;
        updateInfo();
    }
}

void DKIMManageRulesWidgetItem::updateInfo()
{
    setCheckState(ColumnType::Enabled, mRule.enabled() ? Qt::Checked : Qt::Unchecked);
    setText(ColumnType::Domain, mRule.domain());
    setText(ColumnType::ListId, mRule.listId());
    setText(ColumnType::From, mRule.from());
    setText(ColumnType::SDid, mRule.signedDomainIdentifier().join(QLatin1Char(' ')));
    setText(ColumnType::Priority, QString::number(mRule.priority()));
    mRuleTypeCombobox->setRuleType(mRule.ruleType());
}

DKIMManageRulesWidget::DKIMManageRulesWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mTreeWidget = new QTreeWidget(this);
    mTreeWidget->setObjectName(QStringLiteral("treewidget"));
    mTreeWidget->setRootIsDecorated(false);
    mTreeWidget->setHeaderLabels({i18n("Active"), i18n("Domain"), i18n("List-ID"), i18n("From"), i18n("SDID"), i18n("Rule type"), i18n("Priority")});
    mTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    mTreeWidget->setAlternatingRowColors(true);

    KTreeWidgetSearchLine *searchLineEdit = new KTreeWidgetSearchLine(this, mTreeWidget);
    searchLineEdit->setObjectName(QStringLiteral("searchlineedit"));
    searchLineEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(searchLineEdit);

    mainLayout->addWidget(mTreeWidget);
    connect(mTreeWidget, &QTreeWidget::customContextMenuRequested, this, &DKIMManageRulesWidget::customContextMenuRequested);
    connect(mTreeWidget, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem *item) {
        if (item) {
            DKIMManageRulesWidgetItem *rulesItem = dynamic_cast<DKIMManageRulesWidgetItem *>(item);
            modifyRule(rulesItem);
        }
    });
}

DKIMManageRulesWidget::~DKIMManageRulesWidget()
{
}

void DKIMManageRulesWidget::loadSettings()
{
    const QVector<MessageViewer::DKIMRule> rules = MessageViewer::DKIMManagerRules::self()->rules();

    for (const MessageViewer::DKIMRule &rule : rules) {
        DKIMManageRulesWidgetItem *item = new DKIMManageRulesWidgetItem(mTreeWidget);
        item->setRule(rule);
    }
}

void DKIMManageRulesWidget::saveSettings()
{
    QVector<MessageViewer::DKIMRule> rules;
    for (int i = 0, total = mTreeWidget->topLevelItemCount(); i < total; ++i) {
        QTreeWidgetItem *item = mTreeWidget->topLevelItem(i);
        DKIMManageRulesWidgetItem *ruleItem = static_cast<DKIMManageRulesWidgetItem *>(item);
        rules.append(ruleItem->rule());
    }
    MessageViewer::DKIMManagerRules::self()->saveRules(rules);
}

QByteArray DKIMManageRulesWidget::saveHeaders() const
{
    return mTreeWidget->header()->saveState();
}

void DKIMManageRulesWidget::restoreHeaders(const QByteArray &header)
{
    mTreeWidget->header()->restoreState(header);
}

void DKIMManageRulesWidget::addRule()
{
    QPointer<DKIMRuleDialog> dlg = new DKIMRuleDialog(this);
    if (dlg->exec()) {
        const MessageViewer::DKIMRule rule = dlg->rule();
        if (rule.isValid()) {
            DKIMManageRulesWidgetItem *item = new DKIMManageRulesWidgetItem(mTreeWidget);
            item->setRule(rule);
        } else {
            qCDebug(MESSAGEVIEWER_DKIMCHECKER_LOG) << "Rule is not valid";
        }
    }
    delete dlg;
}

void DKIMManageRulesWidget::duplicateRule(DKIMManageRulesWidgetItem *rulesItem)
{
    QPointer<DKIMRuleDialog> dlg = new DKIMRuleDialog(this);
    dlg->loadRule(rulesItem->rule());
    if (dlg->exec()) {
        const MessageViewer::DKIMRule rule = dlg->rule();
        if (rule.isValid()) {
            DKIMManageRulesWidgetItem *item = new DKIMManageRulesWidgetItem(mTreeWidget);
            item->setRule(rule);
        }
    }
    delete dlg;
}

void DKIMManageRulesWidget::modifyRule(DKIMManageRulesWidgetItem *rulesItem)
{
    QPointer<DKIMRuleDialog> dlg = new DKIMRuleDialog(this);
    dlg->loadRule(rulesItem->rule());
    if (dlg->exec()) {
        const MessageViewer::DKIMRule rule = dlg->rule();
        if (rule.isValid()) {
            rulesItem->setRule(rule);
        }
    }
    delete dlg;
}

void DKIMManageRulesWidget::customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QTreeWidgetItem *item = mTreeWidget->currentItem();
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("list-add")), i18n("Add..."), this, [this]() {
        addRule();
    });
    DKIMManageRulesWidgetItem *rulesItem = dynamic_cast<DKIMManageRulesWidgetItem *>(item);
    if (rulesItem) {
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, rulesItem]() {
            modifyRule(rulesItem);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("edit-duplicate")), i18n("Duplicate Rule"), this, [this, rulesItem]() {
            duplicateRule(rulesItem);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("edit-delete")), i18n("Remove Rule"), this, [this, item]() {
            if (KMessageBox::Yes == KMessageBox::warningYesNo(this, i18n("Do you want to delete this rule?"), i18n("Delete Rule"))) {
                delete item;
            }
        });
    }
    if (mTreeWidget->topLevelItemCount() > 0) {
        menu.addSeparator();
        menu.addAction(i18n("Delete All"), this, [this]() {
            if (KMessageBox::Yes == KMessageBox::warningYesNo(this, i18n("Do you want to delete all the rules?"), i18n("Delete Rules"))) {
                mTreeWidget->clear();
            }
        });
    }
    menu.exec(QCursor::pos());
}