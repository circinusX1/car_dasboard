/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     egg, antico
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
egg:        @author José Expósito
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com

*/

#ifndef __TASKSWITCH_H
#define __TASKSWITCH_H

#include "qwidgeticon.h"
#include "client.h"

class TheApp;

class XcliSwitchIcon : public WidgetIcon
{
public:
    XcliSwitchIcon(XClient *client, int set_size, QWidget *parent = 0);
    ~XcliSwitchIcon();

    XClient *m_Client;
};

class TaskSwitch : public QDialog
{
    Q_OBJECT
public:
    TaskSwitch(XClient *active, ClientList *_x_clients, QWidget *parent = 0);
    ~TaskSwitch();

    XClient *selectedIcon();

    int current;
    QList <XcliSwitchIcon *> icons;
    QHBoxLayout *layout;

public slots:
    void reject();

protected:
    virtual void keyPressEvent(QKeyEvent *);
    //virtual void keyReleaseEvent(QKeyEvent *);

};

#endif

