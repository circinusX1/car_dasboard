/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     matchbox, egg, antico, xfwm4, metacity
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
metacity:   Copyright (C) 2001 Havoc Pennington
xfwm4:      here and there source code
matchbox:   here and there source code
*/


#include <stdint.h>
#include "x11atoms.h"

bool x11atoms::_initialized = false;
Atom x11atoms::_atoms[ATOM_COUNT];

//-----------------------------------------------------------------------------
bool x11atoms::resolve_atoms(Display *d)
{
    if (_initialized)
        return true;

    static const char *ans[] =
    {
        "COMPOSITING_MANAGER",
        "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR",
        "KWM_WIN_ICON",
        "_MOTIF_WM_HINTS",
        "_MOTIF_WM_INFO",
        "_NET_ACTIVE_WINDOW",
        "_NET_CLIENT_LIST",
        "_NET_CLIENT_LIST_STACKING",
        "_NET_CLOSE_WINDOW",
        "_NET_CURRENT_DESKTOP",
        "_NET_DESKTOP_GEOMETRY",
        "_NET_DESKTOP_LAYOUT",
        "_NET_DESKTOP_NAMES",
        "_NET_DESKTOP_VIEWPORT",
        "_NET_FRAME_EXTENTS",
        "_NET_MOVERESIZE_WINDOW",
        "_NET_NUMBER_OF_DESKTOPS",
        "_NET_REQUEST_FRAME_EXTENTS",
        "_NET_SHOWING_DESKTOP",
        "_NET_STARTUP_ID",
        "_NET_SUPPORTED",
        "_NET_SUPPORTING_WM_CHECK",
        "_NET_SYSTEM_TRAY_OPCODE",
        "_NET_WM_ACTION_ABOVE",
        "_NET_WM_ACTION_BELOW",
        "_NET_WM_ACTION_CHANGE_DESKTOP",
        "_NET_WM_ACTION_CLOSE",
        "_NET_WM_ACTION_FULLSCREEN",
        "_NET_WM_ACTION_MAXIMIZE_HORZ",
        "_NET_WM_ACTION_MAXIMIZE_VERT",
        "_NET_WM_ACTION_MINIMIZE",
        "_NET_WM_ACTION_MOVE",
        "_NET_WM_ACTION_RESIZE",
        "_NET_WM_ACTION_SHADE",
        "_NET_WM_ACTION_STICK",
        "_NET_WM_ALLOWED_ACTIONS",
        "_NET_WM_CONTEXT_HELP",
        "_NET_WM_DESKTOP",
        "_NET_WM_ICON",
        "_NET_WM_ICON_GEOMETRY",
        "_NET_WM_ICON_NAME",
        "_NET_WM_MOVERESIZE",
        "_NET_WM_NAME",
        "_NET_WM_PID",
        "_NET_WM_PING",
        "_NET_WM_WINDOW_OPACITY",
        "_NET_WM_WINDOW_OPACITY_LOCKED",
        "_NET_WM_STATE",
        "_NET_WM_STATE_ABOVE",
        "_NET_WM_STATE_BELOW",
        "_NET_WM_STATE_DEMANDS_ATTENTION",
        "_NET_WM_STATE_FULLSCREEN",
        "_NET_WM_STATE_HIDDEN",
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_MAXIMIZED_VERT",
        "_NET_WM_STATE_MODAL",
        "_NET_WM_STATE_SHADED",
        "_NET_WM_STATE_SKIP_PAGER",
        "_NET_WM_STATE_SKIP_TASKBAR",
        "_NET_WM_STATE_STICKY",
        "_NET_WM_STRUT",
        "_NET_WM_STRUT_PARTIAL",
        "_NET_WM_SYNC_REQUEST",
        "_NET_WM_SYNC_REQUEST_COUNTER",
        "_NET_WM_USER_TIME",
        "_NET_WM_USER_TIME_WINDOW",
        "_NET_WM_WINDOW_TYPE",
        "_NET_WM_WINDOW_TYPE_DESKTOP",
        "_NET_WM_WINDOW_TYPE_DIALOG",
        "_NET_WM_WINDOW_TYPE_DOCK",
        "_NET_WM_WINDOW_TYPE_MENU",
        "_NET_WM_WINDOW_TYPE_NORMAL",
        "_NET_WM_WINDOW_TYPE_SPLASH",
        "_NET_WM_WINDOW_TYPE_TOOLBAR",
        "_NET_WM_WINDOW_TYPE_UTILITY",
        "_NET_WORKAREA",
        "SM_CLIENT_ID",
        "UTF8_STRING",
        "WM_CHANGE_STATE",
        "WM_CLIENT_LEADER",
        "WM_CLIENT_MACHINE",
        "WM_COLORMAP_WINDOWS",
        "WM_DELETE_WINDOW",
        "WM_HINTS",
        "WM_NORMAL_HINTS",
        "WM_PROTOCOLS",
        "WM_STATE",
        "WM_TAKE_FOCUS",
        "WM_TRANSIENT_FOR",
        "WM_WINDOW_ROLE",
        "XFWM4_COMPOSITING_MANAGER",
        "XFWM4_TIMESTAMP_PROP",
        "_XROOTPMAP_ID",
        "_XSETROOT_ID"
    };
    return (::XInternAtoms (d, (char **) ans, ATOM_COUNT, FALSE, _atoms) != 0);
}

//-----------------------------------------------------------------------------
Atom x11atoms::atom(int n)
{
    if ((n >= 0) || (n <= ATOM_COUNT))
        return _atoms[n];
    else
        return X11_None;
}


//-----------------------------------------------------------------------------
int x11atoms::get_wxclass(Window w)
{
    int     ntypes = 3;
    Atom*   xprop = (Atom *) x11_get_props(w, _atoms[NET_WM_WINDOW_TYPE], XA_ATOM, &ntypes);

    if (0==xprop)
        return Unknown;
    int     wc = Unknown;
    while (--ntypes >= 0)
    {
        if (xprop[ntypes] == _atoms[NET_WM_WINDOW_TYPE_DESKTOP])
            wc = Desktop;
        else if (xprop[ntypes] == _atoms[NET_WM_WINDOW_TYPE_DOCK])
            wc = Dock;
        else if (xprop[ntypes] == _atoms[NET_WM_WINDOW_TYPE_TOOLBAR])
            wc = Toolbar;
        else if (xprop[ntypes] == _atoms[NET_WM_WINDOW_TYPE_MENU])
            wc = Menu;
        else if (xprop[ntypes] == _atoms[NET_WM_WINDOW_TYPE_UTILITY])
            wc = Utility;
        else if (xprop[ntypes] == _atoms[NET_WM_WINDOW_TYPE_SPLASH])
            wc = Splash;
        else if (xprop[ntypes] == _atoms[NET_WM_WINDOW_TYPE_DIALOG])
            wc = Dialog;
        else if (xprop[ntypes] == _atoms[NET_WM_WINDOW_TYPE_NORMAL])
            wc = Normal;
    }
    ::XFree(xprop);
    return wc;
}

//-----------------------------------------------------------------------------
void *x11atoms::x11_get_props(Window win, Atom prop, Atom type, int *nitems)
{
    Atom            rv;
    int             rf;
    ulong           ucount;
    ulong           na1;
    u_char*         vrval = 0;

    if (::XGetWindowProperty(DPY(), win, prop, 0, 0x7fffffff, False,
                           type, &rv, &rf, &ucount,
                           &na1, &vrval) != Success)
        return NULL;

    if (nitems)
        *nitems = ucount;
    return vrval;
}

//-----------------------------------------------------------------------------
void x11atoms::send_wm_event(Window w, long lp0, long lp1)
{
    XClientMessageEvent ev;

    ev.type         = ClientMessage;
    ev.window       = w;
    ev.message_type = atom(WM_PROTOCOLS);
    ev.format       = 32;
    ev.send_event   = TRUE;
    ev.data.l[0]    = lp0;
    ev.data.l[1]    = lp1;
    ::XSendEvent(DPY(), w, FALSE, 0L, (XEvent*) &ev);
}
