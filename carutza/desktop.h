/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     antico
--------------------------------------------------------------------------------
 This file is part of CARUTZA dashboard

    CARUTZA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CARUTZA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CARUTZA.  If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
Credits
--------------------------------------------------------------------------------
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com

*/
//-----------------------------------------------------------------------------
// Created: July 2013
// Author:  Marius C. @
//-----------------------------------------------------------------------------

#ifndef XESKTOP_H
#define XESKTOP_H

#include "defs.h"
#include <qwidget2.h>

//-----------------------------------------------------------------------------
class Desktop : public QWidget2
{
    Q_OBJECT

public:
    Desktop(QWidget *parent=0);
    ~Desktop();
    void init();
    void load_wall_paper(const QString &path);

public slots:
    void run_menu(QAction *);

private:
    QBoxLayout *_boxlayout;
    QMenu*      _menu;
};

#endif

