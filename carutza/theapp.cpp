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
metacity:   Copyright (C) 2001 Havoc Pennington
xfwm4:      here and there source code
matchbox:   here and there source code
*/

#include <QtDBus>
#include "theapp.h"
#include "appman.h"
#include "x11atoms.h"
#include "dbusipc.h"
#include "taskswitch.h"

//-----------------------------------------------------------------------------
TheApp* TheApp::PAPP = 0;
bool    TheApp::_replay = false;
bool    TheApp::_resend = false;
QApplication::EventFilter TheApp::_default_x11=0; //anyway.. is 0 by stc++

//-----------------------------------------------------------------------------
TheApp::TheApp(int &argc, char **argv) :
                QApplication(argc, argv),_papp_man(0),_ignorekey(false)
{
    TheApp::PAPP=this;
}

//-----------------------------------------------------------------------------
TheApp::~TheApp()
{

    std::vector<Panel*>::iterator f =  _panels.begin();
    for(;f != _panels.begin(); ++f)
    {
        delete *f;
    }

    delete _dskwidget;
    TheApp::PAPP=0;
}

void TheApp::show_panels(bool show)
{
    std::vector<Panel*>::iterator f =  _panels.begin();
    for(;f != _panels.end(); ++f)
    {
        if(show)
            (*f)->show();
        else
            (*f)->hide();
    }
}

//-----------------------------------------------------------------------------
Panel* TheApp::get_panel(const QString& p)
{
    std::vector<Panel*>::iterator f =  _panels.begin();
    for(;f != _panels.end(); ++f)
    {
        const CfgPanel* cp = (*f)->conf();
        if(cp->_name==p)
        {
            return (*f);
        }
    }
    return 0;
}


void TheApp::show_panel(const QString& panel, bool show)
{
    std::vector<Panel*>::iterator f =  _panels.begin();
    for(;f != _panels.end(); ++f)
    {
        const CfgPanel* cp = (*f)->conf();
        if(cp->_name==panel)
        {
            if(show)
                (*f)->show();
            else
                (*f)->hide();
            break;
        }
    }

}


//-----------------------------------------------------------------------------
void TheApp::init(MySett& s)
{
    Q_UNUSED(s);
    _ptask_switch = NULL;
    _bkey_grab = false;
    _wm_wins_state = PS_STARTING;

    x11atoms::resolve_atoms(DPY());
    setOrganizationName("mylinuz");
    setApplicationName("dashdash");
    ::XGrabKeyboard(DPY(), ROOT_XWIN(), TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
    CFG(load_panels(_panels));
    _dskwidget  = new Desktop();
    if(_papp_man==0)
    {
        XwnSet dummy;
        _papp_man = new Appman(0, QPoint(32,32), dummy);
        _papp_man->move(0,0);
        //_papp_man->show();
    }
    _papp_man->set_active_xwin(0, false);

    QTimer::singleShot(1024, this, SLOT(slot_namage_apps()));
    _wm_wins_state = PS_NORMAL;
    MySett::config().finalize();
    _runicon.load_image(CFG(_theme),"run-icon.png");
    QTimer::singleShot(2048, this, SLOT(slot_refresh()));
    _ipc.start();
}

//-----------------------------------------------------------------------------
void TheApp::app_about()
{
}

//-----------------------------------------------------------------------------
void TheApp::wm_minimize_all()
{
    XClient*   pxc;

    _wm_wins_state = PS_MINIMIZING;
    foreach(pxc, _x_wins)
    {
        if (pxc->_xw_state == X11_NormalState)
            pxc->slot_minimize_now();
    }
    _papp_man->set_active_xwin(NULL, false);
	::XUngrabKeyboard(DPY(), CurrentTime);
	::XGrabKeyboard(DPY(), ROOT_XWIN(), TRUE, GrabModeAsync,
                    GrabModeAsync, CurrentTime);
	_wm_wins_state = PS_NORMAL;
}

//-----------------------------------------------------------------------------
void TheApp::wm_show_all()
{

}


//-----------------------------------------------------------------------------
void TheApp::wm_dock_icon_removed(XClient *pxc)
{
    XRaiseWindow(DPY(), pxc->winId());
	restack(pxc);
    _papp_man->set_active_xwin(pxc, false);
	pxc->set_subwin_focus(0, CurrentTime);
}

//-----------------------------------------------------------------------------
void TheApp::slot_namage_apps()
{

	Window              c_win, w1, w2, *wins;
	uint                nwins, cwin;
	XWindowAttributes   attr;
    XClient*            pxc;

	if (::XQueryTree(DPY(), ROOT_XWIN(), &w1, &w2, &wins, &nwins) == 0 || ! nwins)
	{
		return;
	}

	for (cwin=0; cwin < nwins; cwin++)
	{
		c_win = wins[cwin];
		if (is_dash_widget(c_win))
		{
			continue;
        }
		::XGetWindowAttributes(DPY(), c_win, &attr);

		if ((pxc = _x_wins.value(c_win)) != NULL)
		{
            pxc->slot_map_win();
		}
		else if (attr.map_state == IsViewable &&
                ! attr.override_redirect)
        {
			x_create(c_win);
		}
	}
}

//-----------------------------------------------------------------------------
void TheApp::slot_unmanage_apps()
{
}

//-----------------------------------------------------------------------------
void TheApp::slot_focus_topmost()
{
    XClient* pxc;
	Window  c_win, w1, w2, *wins;
	uint    nwins;

	if (XQueryTree(DPY(), ROOT_XWIN(),
                    &w1, &w2, &wins, &nwins) == 0 || ! nwins) {
		//qDebug() << "No running _x_wins ...";
		return;
	}

	for (int cwin=nwins-1; cwin >= 0; cwin--) {
		c_win = wins[cwin];
		//qDebug() << "run process:" << cwin << "name:" << c_win;

		if ((pxc = _x_frames.value(c_win)) != NULL ||
			(pxc = _x_wins.value(c_win)) != NULL) {
            if (pxc->_xw_state == X11_NormalState) {
                _papp_man->set_active_xwin(pxc, false);
                pxc->set_frame_status(true);
				XRaiseWindow(DPY(), pxc->_x_wid);
				restack(pxc);
				pxc->set_subwin_focus(0, CurrentTime);
				break;
			}
		}
	}
    refresh_buts();
}

void TheApp::refresh_buts()
{
    std::vector<Panel*>::iterator it =  _panels.begin();
    for(; it != _panels.end(); ++it)
    {
        (*it)->refresh_buts();
    }
}

//-----------------------------------------------------------------------------
void TheApp::sys_menu_logout()
{

}

//-----------------------------------------------------------------------------
void TheApp::sys_menu_sleep()
{

}

//-----------------------------------------------------------------------------
void TheApp::sys_menu_restart()
{

}

//-----------------------------------------------------------------------------
void TheApp::sys_menu_shutdown()
{

}

//-----------------------------------------------------------------------------
void TheApp::app_set_hilhight_color(const QColor &col)
{
	QPalette pal = QApplication::palette();
	pal.setColor(QPalette::Highlight, col);
	QApplication::setPalette(pal);
}


//-----------------------------------------------------------------------------
void TheApp::wm_killwm()
{
    XClient* pxc;
    foreach(pxc, _x_wins)
    {
        ::XRemoveFromSaveSet(DPY(), pxc->_x_wid);
        ::XReparentWindow(DPY(), pxc->_x_wid, ROOT_XWIN(), pxc->n_px, pxc->n_py);
        pxc->slot_destroy_xcli();
    }
    _x_wins.clear();
	_x_frames.clear();
    std::vector<Panel*>::iterator it =  _panels.begin();
    for(; it != _panels.end(); ++it)
    {
        (*it)->close();
    }
	::XSync(DPY(), FALSE);
	flush();
}

//-----------------------------------------------------------------------------
void TheApp::wm_add_ignorewid(Window wid)
{
    _ignored_xids.insert(wid);
}

//-----------------------------------------------------------------------------
void TheApp::wm_remove_ignoredwid(Window wid)
{
    _ignored_xids.erase(wid);
}

//-----------------------------------------------------------------------------
void TheApp::app_set_click_rule(bool active)
{
    _bdbl_clk_rule = active;

    if(_x_wins.size())
    {
        XClient* pxc;
        foreach(pxc, _x_wins)
        {
            pxc->slot_dblclk_mini(active);
        }
    }
}

//-----------------------------------------------------------------------------
QString TheApp::_popen(const char* cmd, const char* filter)
{
    QString out;
    FILE* pipe = ::popen(cmd, "r");
    if (!pipe) return QString("");

    char    buffer[512];
    while(!::feof(pipe))
    {
        ::memset(buffer,0,sizeof(buffer));
        if(::fgets(buffer, sizeof(buffer)-sizeof(buffer[0]), pipe) != NULL)
        {
            if(filter)
            {
                if(::strstr(buffer,filter))
                    out.append(buffer);
            }
            else
                out.append(buffer);
        }
    }
    ::pclose(pipe);
    return out;
}

//-----------------------------------------------------------------------------
XClient* TheApp::is_running(const QString& pname)
{
    Appman* pc = _papp_man;
    return pc->is_running(pname);
}

//-----------------------------------------------------------------------------
XClient* TheApp::is_running(pid_t pid)
{
    Appman* pc = _papp_man;
    return pc->is_running(pid);
}

//-----------------------------------------------------------------------------
bool TheApp::_is_running(const XwnSet& xset)
{
    Appman* pc = _papp_man;
    XClient* pxc = pc->is_running(xset._pname);
    if(pxc)
    {
        pc->rescan_visibilities(pxc);
        return true;
    }

    return is_process(xset);
}

//-----------------------------------------------------------------------------
bool TheApp::is_process( const XwnSet& xset)
{
    return is_process(xset._pname);

}


//-----------------------------------------------------------------------------
bool TheApp::is_process( const QString& pname)
{
    // find in proxcess list
    QString syscmd = "ps ax | grep " + pname +" | grep -v grep";
    QString cmdrv = _popen(syscmd, pname);
    if(!cmdrv.isEmpty())
    {
        //update our close button ?!?
        //activate by xdtool
        QStringList parts = cmdrv.split(" ", QString::SkipEmptyParts);
        return parts[0].toInt();
    }
    return 0;
}

void TheApp::runintern(const XwnSet& xset)
{
    QTimer::singleShot(512, this, SLOT(slot_normal_cursor()));
    setOverrideCursor(Qt::WaitCursor);
    if(xset._pname[0]=='@') //dbus
    {
        _ipc.widget_clicked(xset);
        return;
    }
}

//-----------------------------------------------------------------------------
void TheApp::runapp(const XwnSet& xset)
{
    _pset_torun=&xset;// safe
    if(_papp_man->kill_active_app())
    {
        QTimer::singleShot(CFG(_killdelay)+64, this, SLOT(slot_run_app()));
        return;
    }
    emit slot_run_app();
}

//-----------------------------------------------------------------------------
void TheApp::slot_run_app()
{
    cond_if(_pset_torun==nullptr,return);
    cond_if(_is_running(*_pset_torun), return);
/*
    QString binname = _pset_torun->_cmd;
    if(_pset_torun->_cmd.contains(" "))
    {
        QStringList parts = _pset_torun->_cmd.split(" ");
        binname = parts[0];
    }
    if(binname.contains("/"))
    {
        QStringList parts = _pset_torun->_cmd.split("/");
        binname = parts.back();
    }
    QString exist = "which "; exist += binname;
    exist = _popen(exist, binname);
    if(!exist.isEmpty())
    {
*/

        char cd[512];
        ::getcwd(cd,511);

        setOverrideCursor(Qt::WaitCursor);
        QTimer::singleShot(4096, this, SLOT(slot_normal_cursor()));

        QString cmd = "export DISPLAY=:";
        int d = CFG(_display);
        cmd += QString::number(d);
        cmd += " && ";

        std::string loco = "./"; loco += _pset_torun->_cmd.toStdString();
        if(::access(loco.c_str(),0)==0)
            cmd += "./";

        cmd += _pset_torun->_cmd;
        cond_if(!cmd.endsWith("&"),cmd += " &");
        qDebug() << "start: " << cmd;
        system(cmd.toUtf8());
/*
    }
    else
    {
        QApplication::beep();
    }
*/

    _pset_torun=0;
}

//-----------------------------------------------------------------------------
void TheApp::slot_normal_cursor()
{
    QApplication::restoreOverrideCursor();
    ///setCursor(Qt::ArrowCursor);
}

//-----------------------------------------------------------------------------
Desktop* TheApp::__Dsk(){return PA->_dskwidget;}


//-----------------------------------------------------------------------------
static Qt::ButtonState get_modifiers()
{
    int result = Qt::NoButton;

    Window root, child;
    int root_x, root_y;
    int win_x, win_y;
    uint keys_buttons;
    bool status = XQueryPointer(QPaintDevice::x11AppDisplay(),
    QPaintDevice::x11AppRootWindow(),
    &root, &child,
    &root_x, &root_y,
    &win_x, &win_y,
    &keys_buttons);

    if (status)
    {
        result |= keys_buttons & ShiftMask ? (int)Qt::ShiftButton : 0;
        result |= keys_buttons & Mod1Mask ? (int)Qt::AltButton : 0;
        result |= keys_buttons & Mod4Mask ? (int)Qt::MetaButton : 0;
        result |= keys_buttons & ControlMask ? (int)Qt::ControlButton : 0;

        result |= keys_buttons & Button1Mask ? (int)Qt::LeftButton : 0;
        result |= keys_buttons & Button2Mask ? (int)Qt::MidButton : 0;
        result |= keys_buttons & Button3Mask ? (int)Qt::RightButton : 0;
    }
    return Qt::ButtonState(result);
}

//-----------------------------------------------------------------------------
Qt::ButtonState TheApp::keyboard_modifiers()
{
    return Qt::ButtonState(get_modifiers() & Qt::KeyButtonMask);
}

//-----------------------------------------------------------------------------
Qt::ButtonState TheApp::mouse_buttons()
{
    return Qt::ButtonState(get_modifiers() & Qt::MouseButtonMask);
}

//-----------------------------------------------------------------------------
void TheApp::slot_refresh()
{
    //_launcher->show();
    //_launcher->refresh_buts();
    std::vector<Panel*>::iterator it =  _panels.begin();
    for(; it != _panels.end(); ++it)
    {
        (*it)->refresh_buts();
    }
}


//-----------------------------------------------------------------------------

bool TheApp::x11_event_filter(void *message, long *result)
{
    XEvent *event = reinterpret_cast<XEvent *>(message);
    switch (event->type)
    {
        case ClientMessage:
            qDebug()<<"client message............";
        default:
    /*
        {
                if( event->xclient.message_type == x11atoms::atom(x11atoms::NET_ACTIVE_WINDOW) &&
                        event->xclient.format == 32)
                {
                      qDebug()<<"raisgin";
                      XMapRaised(DPY(), event->xclient.window);
                      return true;
                }

            }
    */
            break;
        case KeyPress:
            if (!_resend) PA->x_key_press(event);
            _resend = false;
            return false;
            break;
        case KeyRelease:
            if (!_resend) PA->x_key_release(event);
            _resend = false;
            return false;
            break;
        case MotionNotify:
            break;
        case ButtonRelease:
            break;
        case ButtonPress:
            return PA->x_button_press(event);
            break;
        case ConfigureNotify:
            if (event->xconfigure.event != event->xconfigure.window)
                return true;
            return false;
            break;
        case ConfigureRequest:
            return PA->x_configure(event);
            break;
        case MapRequest:
            PA->x_create(event->xmaprequest.window);
            return false;
            break;
        case MapNotify:
            if (event->xmap.window == PA->_dashwid)
            {
                XSetInputFocus(DPY(), event->xmap.window, X11_None, CurrentTime);
                XRaiseWindow(DPY(), event->xmap.window);
            }
            if (event->xmap.event != event->xmap.window)
                return true;
            else
                return false;
            break;
        case UnmapNotify:
            return PA->x_unmap(event);
            break;
        case DestroyNotify:
            return PA->x_destroy(event);
            break;
        case PropertyNotify:
            return PA->x_property(event);
            break;
        case ColormapNotify:
            return PA->x_colormap_notify(event);
            break;
        case FocusIn:
            PA->x_focus_in(event);
            return false;
            break;
    }
    if (_default_x11)
        return _default_x11(message, result);
    return false;
}

//-----------------------------------------------------------------------------
void TheApp::x_send_key(XEvent *event, long mask)
{
    Window w = _papp_man->get_curent_wid();

    if ( w != 0)
    {
        event->xkey.window = w;
        _resend = true;
        ::XSendEvent(DPY(), w, TRUE, mask, event);
    }
    ::XAllowEvents(DPY(), SyncBoth, CurrentTime);
}

//-----------------------------------------------------------------------------
bool TheApp::x_key_press(XEvent *event)
{
    KeySym  sym = (int)::XLookupKeysym(&event->xkey, 0);
    uint    keymask1 = Mod1Mask & 0x0F;
    uint    mod = event->xkey.state & keymask1;

    bool    ishotkey = false;

    if (_bkey_grab)   // a hot key activation
    {
        ishotkey = x_hotkey(sym, mod);
        if (ishotkey)
            return true;
    }
    if (sym == XK_Alt_L && !_bkey_grab)
    {
        _bkey_grab = true;
        return true;
    }
    _is_key_mapped(sym);
    x_send_key(event, KeyPressMask);
    return false;
}

//-----------------------------------------------------------------------------
bool TheApp::_is_key_mapped(const KeySym& sym)
{
    if(_ignorekey)
        return false;

    MySett& ps = MySett::config();
    size_t     hotkeys = ps._showhide.size();

    for(size_t k = 0; k < hotkeys; ++k)
    {
        MySett::ShowHide& sh = ps._showhide[k];
        if(sh._show == (int)sym)
        {
            _ignorekey = true;
            _sshow     = sh;
            _bshow     = true;
            QTimer::singleShot(sh._timing+50, this, SLOT(slot_bring_to_top()));
        }
        else if(sh._hide == (int)sym)
        {
            _ignorekey = true;
            _sshow     = sh;
            _bshow     = false;
            QTimer::singleShot(sh._timing+50, this, SLOT(slot_bring_to_top()));
        }
    }

    switch(sym)
    {
        case XK_Home: // <
        case XK_Left: // >
        case XK_Right:  //home XK_Tab
        case XK_End:
        case 65293:
        case 32:
        case 65307:
            QTimer::singleShot(500, this, SLOT(slot_redirect_to_lancer(sym)));
        break;
        default:
            break;
    }
    return false;
}

//-----------------------------------------------------------------------------
void TheApp::slot_redirect_to_lancer(int sym)
{
    _ignorekey=false;
    process_key(sym);

}

//-----------------------------------------------------------------------------
void TheApp::slot_bring_to_top()
{
    _papp_man->make_visible(_sshow._pname, _bshow);
    _ignorekey=false;
}

//-----------------------------------------------------------------------------
bool TheApp::x_key_release(XEvent *event)
{
    KeySym  sym = (int)::XLookupKeysym(&event->xkey, 0);

    if (sym == XK_Alt_L && _bkey_grab)
    {
        _bkey_grab = false;
        if (_ptask_switch != NULL && _wm_wins_state == PS_CHANGING)
        {
            _ptask_switch->hide();
            XClient* pxc = _ptask_switch->selectedIcon();

            if (pxc)
            {
                if (pxc->_xw_state == X11_IconicState)
                {
                    pxc->slot_map_win();
                }
                _papp_man->set_active_xwin(pxc, false);
                pxc->set_frame_status(true);
                XRaiseWindow(DPY(), pxc->_x_wid);
                restack(pxc);
                pxc->set_subwin_focus(0, CurrentTime);
            }
            _wm_wins_state = PS_NORMAL;
        }
        return true;
    }
    x_send_key(event, KeyReleaseMask);
    return false;
}

//-----------------------------------------------------------------------------
bool TheApp::x_button_release(XEvent *)
{
    return true;
}

//-----------------------------------------------------------------------------
bool TheApp::x_mouse_move(XEvent *)
{
    return true;
}

//-----------------------------------------------------------------------------
bool TheApp::x_button_press(XEvent *event)
{
    _replay = false;
    if (event->xbutton.window == _dskwidget->winId())
    {
        ::XUngrabKeyboard(DPY(), CurrentTime);
        ::XSetInputFocus(DPY(), event->xbutton.window, X11_None, CurrentTime);
        _papp_man->set_active_xwin(NULL, false);
        ::XGrabKeyboard(DPY(), ROOT_XWIN(), TRUE, GrabModeAsync,
                        GrabModeAsync, CurrentTime);
    }
    else
    {
        XClient* pxc = _x_wins.value(event->xbutton.window);
        if(0 == pxc)
            pxc = _x_frames.value(event->xbutton.window);

        if (pxc)
        {
            ::XRaiseWindow(DPY(), pxc->winId());
            restack(pxc);
            pxc->set_subwin_focus(0, event->xbutton.time);
            _papp_man->set_active_xwin(pxc, false);
            _replay = true;

        }
        else
        {
            ::XRaiseWindow(DPY(), event->xbutton.window);
            if ((pxc = _papp_man->get_x_active_client()) != NULL)
            {
                ::XSetInputFocus(DPY(), pxc->_x_wid, X11_None, CurrentTime);
                restack(pxc);
                pxc->set_subwin_focus(0, event->xbutton.time);
                _papp_man->set_active_xwin(pxc, false);

            }
            else
            {
                ::XSetInputFocus(DPY(), event->xbutton.window, X11_None, CurrentTime);
                ::XUngrabKeyboard(DPY(), CurrentTime);
                ::XGrabKeyboard(DPY(), ROOT_XWIN(), TRUE, GrabModeAsync,
                              GrabModeAsync, CurrentTime);
            }
        }

        if(_replay)
        {
            pxc->_mousexy.setX(event->xbutton.x);
            pxc->_mousexy.setY(event->xbutton.y);
            qDebug() << "pressed on " << event->xbutton.x <<"," << event->xbutton.y;
        }
    }
    ::XAllowEvents (DPY(), _replay ? ReplayPointer : SyncPointer, CurrentTime);
    return false;
}

//-----------------------------------------------------------------------------
bool TheApp::x_focus_in(XEvent *event)
{
    XClient* pxc = _x_wins.value(event->xbutton.window);
    if (pxc)
    {
        pxc->slot_grab();
    }
    return false;
}

//------------------------------------------------------------------------------
bool TheApp::x_configure(XEvent *event)
{
    XWindowChanges  wc;
    XClient*        pxc = _x_wins.value(event->xconfigurerequest.window);

    if (pxc)
    {
        if (event->xconfigurerequest.value_mask & (CWWidth|CWHeight|CWX|CWY))
        {
            int cx,cy,cw,ch;
            if (event->xconfigurerequest.value_mask & CWWidth)
                cw = event->xconfigurerequest.width;
            else
                cw = pxc->base_w;

            if (event->xconfigurerequest.value_mask & CWHeight)
                ch = event->xconfigurerequest.height;
            else
                ch = pxc->base_h;

            if ((event->xconfigurerequest.value_mask & CWX))
                cx = event->xconfigurerequest.x;
            else
                cx = pxc->n_px;

            if ((event->xconfigurerequest.value_mask & CWY))
                cy = event->xconfigurerequest.y;
            else
                cy = pxc->n_py;

            pxc->resize_request(cx, cy, cw, ch);
            event->xconfigurerequest.value_mask &= ~(CWWidth|CWHeight|CWX|CWY);
        }

        if (! event->xconfigurerequest.value_mask)
            return true;
    }
    else     // never mapped window
    {
        wc.x = event->xconfigurerequest.x;
        wc.y = event->xconfigurerequest.y;
        wc.width = event->xconfigurerequest.width;
        wc.height = event->xconfigurerequest.height;
        event->xconfigurerequest.value_mask &= (CWX|CWY|CWWidth|CWHeight);
        ::XConfigureWindow(DPY(), event->xconfigurerequest.window,
                            event->xconfigurerequest.value_mask, &wc);
    }
    return true;
}

//------------------------------------------------------------------------------
bool TheApp::x_unmap(XEvent *event)
{
    XClient*    pxc = _x_wins.value(event->xunmap.window);
    if (pxc)
    {
        pxc->slot_unmap();
        return true;
    }

    if (event->xunmap.event != event->xunmap.window)
        return true;
    return false;
}

//------------------------------------------------------------------------------
bool TheApp::x_destroy(XEvent *event)
{
    XClient*    pxc = _x_wins.value(event->xdestroywindow.window);

    if (pxc)
    {
        _x_wins.remove(event->xdestroywindow.window);
        _x_frames.remove(pxc->winId());
        _papp_man->set_active_xwin(NULL, false);
        pxc->close();
        QTimer::singleShot(512, this, SLOT(slot_focus_topmost()));
        return true;
    }
    if (event->xdestroywindow.event != event->xdestroywindow.window)
        return true;
    return false;
}

//------------------------------------------------------------------------------
bool TheApp::x_property(XEvent *event)
{
    XClient*    pxc = _x_wins.value(event->xproperty.window);
    if(0 == pxc)
        pxc = _x_frames.value(event->xproperty.window);

    if (pxc)
    {
        if (event->xproperty.atom == x11atoms::atom(x11atoms::WM_NORMAL_HINTS))
        {
            pxc->get_wm_normal_hints();
        }
        else if (event->xproperty.atom == x11atoms::atom(x11atoms::WM_HINTS))
        {
            pxc->get_wm_hints();
        }
        else if (event->xproperty.atom == x11atoms::atom(x11atoms::NET_WM_NAME) ||
                 event->xproperty.atom == x11atoms::atom(x11atoms::NET_WM_ICON_NAME))
        {
            pxc->get_wm_name();
        }
        else if (event->xproperty.atom == x11atoms::atom(x11atoms::NET_WM_STATE) ||
                 event->xproperty.atom == x11atoms::atom(x11atoms::WM_STATE))
        {
            int state = pxc->_xw_state;
            if (state == X11_NormalState)
            {
                ::XSetInputFocus(DPY(), pxc->_x_wid, X11_None, CurrentTime);
            }
            else if (state == X11_IconicState)
            {
                //TODO _papp_man->client_is_iconized(pxc);
            }
        }
        else if (event->xproperty.atom == x11atoms::atom(x11atoms::WM_COLORMAP_WINDOWS))
        {
            pxc->get_color_maps();
        }
        else if (event->xproperty.atom == x11atoms::atom(x11atoms::WM_TRANSIENT_FOR))
        {
            ::XGrabServer(DPY());
        }
        else if (event->xproperty.atom == x11atoms::atom(x11atoms::NET_WM_USER_TIME))
        {
            return false;
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool TheApp::x_create(Window w)
{
    XClient* pxc = _x_wins.value(w);
    if (pxc)
    {
        pxc->slot_map_win();
    }
    else
    {
        pxc = new XClient(w,  _bdbl_clk_rule);
        if(pxc)
        {
            _x_wins.insert(w, pxc);
            _x_frames.insert(pxc->winId(), pxc);
            pxc->set_subwin_focus(0, CurrentTime);
            restack(pxc);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
bool TheApp::x_colormap_notify(XEvent *event)
{
    XClient* pxc = _x_wins.value(event->xcolormap.window);
    if (pxc)
    {
        pxc->set_color_maps(event->xcolormap.colormap);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
void TheApp::slot_alt_tab()
{
    if (_x_frames.isEmpty())
        return;
    if (_ptask_switch != NULL)
    {
        delete _ptask_switch;
        _ptask_switch = 0;
    }
    _ptask_switch = new TaskSwitch(_papp_man->get_x_active_client(), &_x_frames, _dskwidget);
    _wm_wins_state = PS_CHANGING;
    _bkey_grab = true;
    _ptask_switch->show();
    ::XSetInputFocus(DPY(), _ptask_switch->winId(), X11_None, CurrentTime);
    ::XRaiseWindow(DPY(), _ptask_switch->winId());
}

//------------------------------------------------------------------------------
bool TheApp::is_dash_widget(Window w)
{
    return _ignored_xids.find(w)!=_ignored_xids.end();
}

//------------------------------------------------------------------------------
void TheApp::restack(const XClient *pxc)
{
    size_t      pnls = 0;
    Window      wins[16]; //16 panels

    for(pnls=0; pnls<_panels.size() && pnls <16; ++pnls)
    {
        wins[pnls]=_panels[pnls]->winId();
    }
    wins[pnls] = pxc->winId();

    ::XRestackWindows(DPY(), wins, pnls);
}

//------------------------------------------------------------------------------
bool TheApp::x_hotkey(KeySym sym, uint mod)
{
    Q_UNUSED(mod);
    XClient* pxc = 0;
    bool hot = false;
    switch (sym)
    {
    case XK_Alt_R:
    case XK_Alt_L:
        break;
    case XK_F4:
        hot = true;
        if ((pxc = _papp_man->get_x_active_client()) != NULL)
        {
            pxc->slot_destroy_xcli();
        }
        break;

    case XK_F9:
        break;  // disabled mco-mco TODO
        hot = true;
        if ((pxc = _papp_man->get_x_active_client()) != NULL)
        {
            if (pxc->_xw_state == X11_NormalState)
            {
                pxc->iconify();
                //_papp_man->client_is_iconized(pxc);
                ::XSetInputFocus(DPY(), ROOT_XWIN(), X11_None, CurrentTime);
            }
            else if (pxc->_xw_state == X11_IconicState)
            {
                pxc->slot_remove_dock_widget();
                pxc->set_subwin_focus(0, CurrentTime);
                restack(pxc);
            }
        }
        break;

    case XK_Tab:
        if (_wm_wins_state != PS_CHANGING)
        {
            QTimer::singleShot(220, this, SLOT(slot_alt_tab()));
        }
        hot = true;
        break;
    default:
        break;
    }
    ::XAllowEvents(DPY(), SyncBoth, CurrentTime);
    return hot;
}


void TheApp::process_key(int key)
{
    Q_UNUSED(key);
    /*
    switch(key)
    {
        case Qt::Key_Right:
            _selected++;
            break;
        case Qt::Key_Left:
            _selected--;
            break;
        case Qt::Key_Home:
            _selected=0;
            break;
        case Qt::Key_End:
            _selected=_butons.size()-1;
            break;
        case Qt::Key_Back:
        case Qt::Key_Escape:
            _selected=-1;
            break;
        case Qt::Key_Return:
        {
            LunchButt* pb = (LunchButt*)_butons.back();
            if(pb->xset()._name=="Back...")
            {
                pb->on_click();
            }
        }
            break;
        case Qt::Key_Space:
            //_butons[_selected]->on_click();
            break;
        default:
            break;
    }
    if((size_t)_selected>=_butons.size())
        _selected=_butons.size()-1;
    else if((int)_selected<0)
        _selected=0;
    _update_btnsel();
    */

}

void TheApp::top_up_panels(const XClient* pcli)
{
    //restack(pcli);

}
