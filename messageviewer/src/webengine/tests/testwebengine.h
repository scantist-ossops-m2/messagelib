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

#ifndef TESTWEBENGINE_H
#define TESTWEBENGINE_H

#include <QWidget>
#include <qwebengineview.h>
namespace MessageViewer
{
class WebEnginePage;
}

class TestWebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit TestWebEngineView(QWidget *parent = Q_NULLPTR);
protected:
    void contextMenuEvent(QContextMenuEvent *e);
};

class TestWebEngine : public QWidget
{
    Q_OBJECT
public:
    explicit TestWebEngine(QWidget *parent = Q_NULLPTR);
    ~TestWebEngine();
private:
    MessageViewer::WebEnginePage *mEnginePage;
};

#endif // TESTWEBENGINE_H
