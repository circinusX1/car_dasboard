/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @ zirexix.com July-Sept 2013
Project:    CAROWA
Credits     matchbox, egg, antico, xfwm4, metacity
--------------------------------------------------------------------------------
 This file is part of CAROWA dashboard

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
Credits
--------------------------------------------------------------------------------
egg:        @author José Expósito
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com
metacity:   Copyright (C) 2001 Havoc Pennington
xfwm4:      here and there source code
matchbox:   here and there source code
*/

#ifndef __THEAPP_H
#define __THEAPP_H

#include "defs.h"
#include "client.h"
#include "desktop.h"
#include "dockbar.h"
#include "panel.h"
#include "mysett.h"
#include <set>
#include <vector>
#include "qslunchbutt.h"
#include "dbusipc.h"

class TaskSwitch;
struct XwnSet;
class XClient;
class Appman;
class Container;
class TheApp : public QApplication
{
	Q_OBJECT

public:
    static TheApp* PAPP;
    friend class panel;
	friend class Desktop;
	friend class XClient;

	enum eAPP_ONGOING {
		PS_STARTING,
		PS_NORMAL,
		PS_MINIMIZING,
		PS_RESTORING,
		PS_WAITING,
		PS_CREATING,
		PS_CHANGING,
	};

    static bool x11_event_filter(void *message, long *result);

	TheApp(int &argc, char **argv);
	~TheApp();

    Appman* appman(){return _papp_man;}
    void set_appman(Appman* papp_man){ _papp_man = papp_man;};
    void init(MySett& s);
	void app_about();
	void app_set_hilhight_color(const QColor &);
	void app_set_click_rule(bool);
    void runapp(const XwnSet& appname);
    void runintern(const XwnSet& xset);
    bool is_process( const XwnSet& xset);
    bool is_process( const QString& pname);
    void sys_menu_logout();
	void sys_menu_sleep();
	void sys_menu_restart();
	void sys_menu_shutdown();

    bool x_create(Window w);
	bool x_destroy(XEvent *);
	bool x_unmap(XEvent *);
	bool x_configure(XEvent *);
	bool x_focus_in(XEvent *);
	bool x_button_press(XEvent *);
    bool x_button_release(XEvent *);
    bool x_mouse_move(XEvent *);
	bool x_hotkey(KeySym sym, uint mod);
	bool x_key_press(XEvent *);
	bool x_key_release(XEvent *);
	void x_send_key(XEvent *, long);
	bool x_property(XEvent *);
	bool x_colormap_notify(XEvent *);
	bool is_dash_widget(Window);
	void restack(XClient *);
	void wm_minimize_all();
	void wm_show_all();
	void wm_dock_icon_removed(XClient *);
	void wm_killwm();
    void wm_add_ignorewid(Window );
    void wm_remove_ignoredwid(Window );
    Qt::ButtonState keyboard_modifiers();
    Qt::ButtonState mouse_buttons();
    XClient* is_running(const QString& pname);
    XClient* is_running(pid_t pid);
    void process_key(int key);
    void show_panels(bool show);
    void show_panel(const QString& panel, bool show);
    void refresh_buts();
    Panel* get_panel(const QString& p);
private:
    bool    _is_running(const XwnSet& appname);
    QString _popen(const char* cmd, const char* filter);
    bool    _is_key_mapped(const KeySym& sym);

public slots:
	void slot_alt_tab();
	void slot_focus_topmost();
	void slot_namage_apps();
	void slot_unmanage_apps();
    void slot_refresh();
    void slot_normal_cursor();
    void slot_bring_to_top();
    void slot_redirect_to_lancer(int sym);
public:
	bool            _bkey_grab;
	int             _wm_wins_state;
	bool            _bdbl_clk_rule;

	TaskSwitch *    _ptask_switch;
	QColor          _prev_color;
    Imagez          _runicon;
    QObject         _p_db_obj;
	ClientList      _x_wins;
	ClientList      _x_frames;

private:
    std::vector<Panel*> _panels;
    std::vector<Container*> _containers;
    Desktop*            _dskwidget;
    Window              _dashwid;
    Appman*             _papp_man;
    std::set<Window>    _ignored_xids;
    static bool         _replay;
    static bool         _resend;
    bool                _ignorekey;
    MySett::ShowHide    _sshow;
    bool                _bshow;
    DbusIpc             _ipc;
public:
    static Desktop*     __Dsk();
    static QApplication::EventFilter _default_x11;
};


#define PA          TheApp::PAPP
#define DESKTOP     TheApp::__Dsk();
#define XPannel     TheApp::__Lancer();
#define DOCK        TheApp::__Dock();
#endif
