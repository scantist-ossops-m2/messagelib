/*
   Copyright (C) 2016 Laurent Montel <montel@kde.org>

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

#ifndef TestDndWebEngine_H
#define TestDndWebEngine_H

#include <QWebEngineView>
#include <QWidget>

namespace WebEngineViewer
{
class WebEngineView;
}

class WebEngineViewBase : public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebEngineViewBase(QWidget *parent = nullptr);
protected:
    void dragEnterEvent(QDragEnterEvent *e) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *e) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *e) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *e) Q_DECL_OVERRIDE;
};

class TestDndWebEngine : public QWidget
{
    Q_OBJECT
public:
    explicit TestDndWebEngine(QWidget *parent = nullptr);
    ~TestDndWebEngine();
private:
    WebEngineViewer::WebEngineView *pageView;
    WebEngineViewBase *webEngineView;
};

#endif // TestDndWebEngine_H
