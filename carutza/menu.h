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

#ifndef __MENUX_H
#define __MENUX_H

#include "defs.h"
#include "button.h"

class panel;
class MenuButton ;
class DskMenu : public QAction
{
	Q_OBJECT
public:
	DskMenu(const QString &text, const QString &comment, QObject *parent = 0);
	DskMenu(const QIcon &icon, const QString &text, const QString &comment, QObject *parent = 0);

	void set_cmd(const QString &cmd, bool runAsRoot = false);

public slots:
	void run_cmd();

private:
	QString     _run_cmd;
	bool        _as_sudo;
};

class MenuButton : public OdButton
{
public:
	explicit MenuButton(CtrlHolder *p, const QPoint& size,const XwnSet& set,  QWidget *parent=0);
	virtual ~MenuButton();

	void create();
	void create_sys(const QString &path);

protected:
	QMap <QString, QIcon* >         _sc_catalog;
	QMultiMap <QString, DskMenu*>   _sc_menu_entry;
};


#endif
