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

#include "adblockautomaticruleslistwidget_gui.h"

#include <KAboutData>
#include <QCommandLineParser>

AdBlockAutomaticRulesListWidgetTest::AdBlockAutomaticRulesListWidgetTest(const QString &filename, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *lay = new QVBoxLayout;

}

AdBlockAutomaticRulesListWidgetTest::~AdBlockAutomaticRulesListWidgetTest()
{
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    KAboutData aboutData(QStringLiteral("adblockblockableitemtest_gui"), i18n("adblockblockableitemtest_Gui"), QStringLiteral("1.0"));
    aboutData.setShortDescription(i18n("Test for adblockblokabledialog"));
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("+[url]"), i18n("URL of an html file to be opened")));

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    QString fileName;
    if (parser.positionalArguments().count()) {
        fileName = parser.positionalArguments().at(0);
    } else {
        fileName = QFileDialog::getOpenFileName(0, QString(), QString(), i18n("HTML File (*.html)"));
    }
    if (fileName.isEmpty()) {
        return 0;
    }

    AdBlockAutomaticRulesListWidgetTest *w = new AdBlockAutomaticRulesListWidgetTest(fileName);

    w->resize(800, 600);
    w->show();
    app.exec();
    delete w;
    return 0;
}

