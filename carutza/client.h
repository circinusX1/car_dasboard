/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     egg, antico, xfwm4, metacity
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

egg:        @author José Expósito
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com
metacity:   Copyright (C) 2001 Havoc Pennington
xfwm4:      here and there source code
matchbox:   here and there source code
--------------------------------------------------------------------------------
*/


#ifndef __CLIENT_H
#define __CLIENT_H

#include "imagez.h"
#include "defs.h"
#include "mysett.h"

#define CLIENT_EVENT_MASK \
    ColormapChangeMask|\
    PropertyChangeMask|\
    StructureNotifyMask|\
    FocusChangeMask

#define DECOR_EVENT_MASK \
    ButtonPressMask|ButtonReleaseMask|\
    KeymapStateMask|\
    ButtonMotionMask|PointerMotionMask|\
    EnterWindowMask|LeaveWindowMask|FocusChangeMask|\
    ExposureMask|StructureNotifyMask|SubstructureRedirectMask|SubstructureNotifyMask

class XClient;
class SysButsBar;
class XFrameBar;
class Dockbar;
class Desktop;

typedef QHash<int, XClient *> ClientList;

class XClient : public QWidget
{
    Q_OBJECT
public:
    XClient(Window w,  bool dblClick=true, QWidget *parent = 0);
    ~XClient();

    void    init();
    void    make_frame();
    void    set_frame_status(bool);

    void    keep_hidden(){_hidden=true;};
    void    bring_to_top();
    void    get_wm_normal_hints();
    void    get_wm_hints();
    void    get_wm_name();
    void    get_app_name();
    void    get_protocols();
    void    get_x11_icon(Pixmap icon, Pixmap mask);
    void    get_color_maps(void);
    void    set_color_maps(Colormap cmap);
    void    set_xcli_state(int state);
    void    fixed_sizes();
    void    resize_request(int cx, int cy, int cw, int ch);
    void    set_subwin_focus(Window subwindow, long timestamp);
    void    late_grab(int);
    pid_t   get_proc_uid();
    void    set_font();

    void move_out(const QPoint& steps);

public slots:
    void    slot_minimize();
    void    slot_minimize_now();
    void    slot_maximize();
    void    slot_map_win();
    void    slot_map_now();
    void    slot_unmap();
    void    slot_remove_dock_widget();
    void    slot_destroy_xcli();
    // Window movement & resize
    void    slot_caption_ldown(QMouseEvent *);
    void    slot_caption_move(QMouseEvent *);
    void    slot_bottom_ldown(QMouseEvent *);
    void    slot_bottom_move(QMouseEvent *);
    void    slot_bottmo_release();
    void    slot_show_top();
    void    slot_grab();
    void    slot_dblclk_mini(bool);

public:
    Window          _x_wid;
    pid_t           _pid;
    QGridLayout*    _p_around_grid;
    SysButsBar*     _p_sysmenubar;
    XFrameBar*      _p_bottombar;
    QString     _xcli_name;
    QString	    _app_name;
    QString     _icon_name;
    QString     _proc_name;
    Imagez  _app_imagez;
    int     _xwclass;
    int     _xw_state;
    bool    _hidden;
    bool    _has_prot_delete;
    bool    _has_proto_tk_focus;
    bool    _wannafocus;
    bool    _urgent;
    Colormap    _colormap;
    long        _wm_hitflags;
    int         _gravity;
    int     base_w, base_h;
    int     inc_w, inc_h;
    int     max_w, max_h;
    int     min_w, min_h;
    int     client_w, client_h;
    int     m_px, m_py, m_pw, m_ph;
    int     n_px, n_py, n_pw, n_ph;
    bool    _maximized;
    bool    _kb_captured;
    bool    _min_on_dblclk;
    int     _captionh;
    QPoint  _mousexy;
    bool    _resizeds;
    XwnSet  _set;
};

#endif




