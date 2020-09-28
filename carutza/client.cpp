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
egg:        @author José Expósito
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com
metacity:   Copyright (C) 2001 Havoc Pennington
xfwm4:      here and there source code
matchbox:   here and there source code
*/


#include "client.h"
#include "mysett.h"
#include "wxframe.h"
#include "x11atoms.h"
#include "dockbar.h"
#include "desktop.h"
#include "mysett.h"
#include "qslunchbutt.h"
#include "theapp.h"
#include "appman.h"

extern bool __initialized;

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
XClient::XClient(Window w,  bool dblClick, QWidget *parent)
    : QWidget(parent),_x_wid(w),
                    _pid(0),
                    _p_around_grid(0),
                    _p_sysmenubar(0),
                    _p_bottombar(0),
                    _xwclass(0),
                    _xw_state(0),
                    _hidden(0),
                    _has_prot_delete(0),
                    _has_proto_tk_focus(0),
                    _wannafocus(0),
                    _urgent(0),
                    _wm_hitflags(0),
                    _gravity(0),
                    _maximized(0),
                    _kb_captured(0),
                    _min_on_dblclk(dblClick),
                    _captionh(0)
{
    setAttribute(Qt::WA_DeleteOnClose);
    init();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
XClient::~XClient()
{
    PA->appman()->remove_xcli(this);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::init()
{

    XGrabServer(DPY());
    _pid = get_proc_uid();

    XSetWindowBorderWidth(DPY(), _x_wid, 0);
    XSetWindowBorderWidth(DPY(), winId(), 0);
    XReparentWindow(DPY(), _x_wid, winId(), 0, SysButsBar::get_optim_h(0));


    // wevs
    XSetWindowAttributes at;

    at.event_mask = DECOR_EVENT_MASK;
    XChangeWindowAttributes(DPY(), winId(), CWEventMask, &at);
    at.event_mask = CLIENT_EVENT_MASK;
    XChangeWindowAttributes(DPY(), _x_wid, CWEventMask, &at);
    XAddToSaveSet(DPY(), _x_wid);

    XGrabButton(DPY(), Button1, AnyModifier, _x_wid, FALSE,
                ButtonPressMask|ButtonReleaseMask,
                GrabModeSync, GrabModeAsync, X11_None, X11_None);

    // wattrs
    XWindowAttributes attr;
    XGetWindowAttributes(DPY(), _x_wid, &attr);
    n_px = attr.x;
    n_py = attr.y;
    base_w = attr.width;
    base_h = attr.height;
    fixed_sizes();
    n_pw = base_w;
    n_ph = base_h + CFGX("caption_height").toInt() + 8;

    get_wm_hints();
    get_app_name();
    get_wm_name();
    get_protocols();

    int bx, by;
    MySett::config().find_widget(_proc_name, _set);

    bx      =   _set._rect.left();
    by      =   _set._rect.top();
    base_w  =   _set._rect.width();
    base_h  =   _set._rect.height();
    _xwclass = x11atoms::get_wxclass(_x_wid);
    cond_if(_set._frame==false, _xwclass=x11atoms::Splash;)
    if (_xwclass == x11atoms::Splash)
    {
        bx = by = 0;
        n_ph = n_ph - n_ph - 8;
    }
    else
    {
        bx = 0;
        by = n_ph;
    }

    n_px = _set._rect.left();
    n_py = _set._rect.top();
    n_pw = _set._rect.width();
    n_ph = _set._rect.height();

    XMoveResizeWindow(DPY(), _x_wid, bx, by, base_w, base_h);
    usleep(0xffff);
    move(n_px, n_py);
    usleep(0xffff);
    resize(n_pw, n_ph);
    make_frame();
    PA->appman()->set_active_xwin(this, true);
    if(!_hidden)
    {
        show();
        XMapWindow(DPY(), _x_wid);
        XSync(DPY(), false);
    }
    XUngrabServer(DPY());
    _maximized = false;
    _xw_state = X11_NormalState;
    set_font();
    QTimer::singleShot(128, this, SLOT(slot_show_top()));
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::make_frame()
{
    qDebug() << "CREATE DECORATIONS FOR ROLE " << _xwclass;

    if (_xwclass == x11atoms::Splash)
    {
        return;
    }

    _p_around_grid = new QGridLayout(this);
    _p_around_grid->setContentsMargins(0, 0, 0, 0);
    _p_around_grid->setSpacing(0);

    FrmWinBorder *center = new FrmWinBorder(this);
    _p_around_grid->addWidget(center, 1, 1);

    _p_sysmenubar = new SysButsBar(_app_imagez, _xcli_name, _xwclass);
    _p_around_grid->addWidget(_p_sysmenubar, 0, 0, 1, -1);


    if (_xwclass != x11atoms::Dialog)
    {
        connect(qobject_cast <FrmSysButs *>(_p_sysmenubar->pb_minimize()),
                SIGNAL(sig_lbutton_down()), SLOT(slot_minimize()));
        connect(qobject_cast <FrmSysButs *>(_p_sysmenubar->pb_maximize()),
                SIGNAL(sig_lbutton_down()), SLOT(slot_maximize()));
        connect(qobject_cast <FrmSysButs *>(_p_sysmenubar->pb_close()),
                SIGNAL(sig_lbutton_down()), SLOT(slot_destroy_xcli()));
    }

    _p_bottombar = new XFrameBar();
    _p_around_grid->addWidget(_p_bottombar, 2, 0, 1, -1);

    connect(_p_sysmenubar, SIGNAL(sig_lbutton_down(QMouseEvent *)), SLOT(slot_caption_ldown(QMouseEvent *)));
    connect(_p_sysmenubar, SIGNAL(sig_mouse_move(QMouseEvent *)), SLOT(slot_caption_move(QMouseEvent *)));
    if (_min_on_dblclk)
    {
        connect(_p_sysmenubar, SIGNAL(sig_mouse_dblclk()), SLOT(slot_minimize()));
    }

    connect(_p_bottombar, SIGNAL(sig_lbutton_down(QMouseEvent *)), SLOT(slot_bottom_ldown(QMouseEvent *)));
    connect(_p_bottombar, SIGNAL(sig_mouse_move(QMouseEvent *)), SLOT(slot_bottom_move(QMouseEvent *)));
    connect(_p_bottombar, SIGNAL(sig_mouse_release()), SLOT(slot_bottmo_release()));
    setLayout(_p_around_grid);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_dblclk_mini(bool _active_img)
{
    _min_on_dblclk = _active_img;
    disconnect(_p_sysmenubar, SIGNAL(sig_mouse_dblclk()), 0, 0);
    if (_min_on_dblclk)
    {
        connect(_p_sysmenubar, SIGNAL(sig_mouse_dblclk()), SLOT(slot_minimize()));
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::fixed_sizes()
{
    if (base_w >= PA->__Dsk()->width())
        base_w = PA->__Dsk()->width();

    if (PA->__Dsk()->width() - base_w < 10)
        base_w = PA->__Dsk()->width();

    if (base_h >= PA->__Dsk()->height()-32 - _captionh - 8)
        base_h = PA->__Dsk()->height()-32 - _captionh - 8;

    if (n_px <= PA->__Dsk()->x())
        n_px = PA->__Dsk()->x();
    if (n_py <= PA->__Dsk()->y() + _captionh)
        n_py = PA->__Dsk()->y() + _captionh;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::set_frame_status(bool s)
{
    if(_p_sysmenubar && _p_bottombar)
    {
        if (s)
        {
            _p_sysmenubar->set_state(SysButsBar::borderActive);
            _p_bottombar->set_state(SysButsBar::borderActive);
        }
        else
        {
            _p_sysmenubar->set_state(SysButsBar::borderInactive);
            _p_bottombar->set_state(SysButsBar::borderInactive);
        }
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_minimize()
{
    slot_unmap();
    set_xcli_state(X11_IconicState);
    //PA->__Dock()->add_xcli(this);  // add to dockbar
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_minimize_now()
{
    slot_unmap();
    _xw_state = X11_IconicState;
    //PA->__Dock()->add_xcli(this);  // add to dockbar
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_maximize()
{
    if (! _maximized)
    {
        n_px = x();
        n_py = y();
        n_pw = width();
        n_ph = height();
        m_pw = PA->__Dsk()->width();
        m_ph = PA->__Dsk()->height()-32;
        move(PA->__Dsk()->x(), PA->__Dsk()->y());
        resize(m_pw, m_ph);
        _maximized = true;
    }
    else
    {
        move(n_px, n_py);
        resize(n_pw, n_ph);
        _maximized = false;
    }
    raise();
    XResizeWindow(DPY(), _x_wid, width(), height()-CFGX("caption_height").toInt()-8);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/void XClient::slot_remove_dock_widget()
{
    slot_map_win();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_map_win()
{
    XUngrabKeyboard(DPY(), CurrentTime);
    if(!_hidden)
    {
        show();
        XMapWindow(DPY(), _x_wid);
    }
    set_xcli_state(X11_NormalState);
    XGrabKeyboard(DPY(), _x_wid, TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_map_now()
{
    XUngrabKeyboard(DPY(), CurrentTime);
    if(!_hidden)
    {
        show();
        XMapWindow(DPY(), _x_wid);
    }
    _xw_state = X11_NormalState;
    XGrabKeyboard(DPY(), _x_wid, TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_unmap()
{
    hide();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_destroy_xcli()
{
    PA->appman()->remove_xcli(this);
    if (_has_prot_delete)
    {
        qDebug() << "ON DESTROY WINDOW (PROT DELETE)";
        x11atoms::send_wm_event(_x_wid, x11atoms::atom(x11atoms::WM_DELETE_WINDOW), CurrentTime);
    }
    else
    {
        qDebug() << "ON DESTROY WINDOW";
        XDestroyWindow(DPY(), _x_wid);
        XSync(DPY(), FALSE);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_caption_ldown(QMouseEvent *event)
{
    _mousexy = event->pos()+_p_sysmenubar->pos();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_caption_move(QMouseEvent *event)
{
    QPoint p(event->globalPos()-_mousexy);
    if (p.x() <= PA->__Dsk()->x())
        p.setX(PA->__Dsk()->x());
    if (p.y() <= PA->__Dsk()->y())
        p.setY(PA->__Dsk()->y());
    move(p.x(), p.y());
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_bottom_ldown(QMouseEvent *event)
{
    if (_p_bottombar->_cur_pos_state == XFrameBar::RightPos)
        _mousexy = event->globalPos();
    else if (_p_bottombar->_cur_pos_state == XFrameBar::NonePos)
        _mousexy = event->pos()+_p_bottombar->pos();  // offset
    _resizeds = true;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_bottmo_release()
{
    _resizeds = false;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_bottom_move(QMouseEvent *event)
{
    QPoint p(event->globalPos()-_mousexy);
    if (_p_bottombar->_cur_pos_state == XFrameBar::NonePos)
    {
        cond_if (p.x() <= PA->__Dsk()->x(), p.setX(PA->__Dsk()->x()));
        cond_if (p.y() <= PA->__Dsk()->y(),  p.setY(PA->__Dsk()->y()));
        move(p.x(), p.y());
    }
    else if (_p_bottombar->_cur_pos_state == XFrameBar::RightPos)
    {
        int resh = height()+p.y();
        int resw = width()+p.x();
        cond_if (resw < 32, return);
        cond_if (resh < 33, return);
        XResizeWindow(DPY(), _x_wid, resw, resh-CFGX("caption_height").toInt() - 8);
        resize(resw, resh);
        _mousexy = event->globalPos();
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
pid_t XClient::get_proc_uid()
{
    unsigned long   len;
    int             form;
    unsigned long   remain;
    unsigned char*  list;
    Atom            type;
    pid_t           pid = 0;

    Atom prop = XInternAtom(QX11Info::DPY(), "_NET_WM_PID", True);
    if(prop)
    {
        if(XGetWindowProperty(DPY(),_x_wid,prop,
                              0,1024,False,AnyPropertyType,
                              &type,&form,&len,&remain,
                              &list)==Success)
        {
            if(list)
            {
                pid = list[1] * 256;
                pid += list[0];
            }
        }
    }
    QString fn = "/proc/";
    fn += QString::number(pid) + "/cmdline";
    QFile inputFile(fn);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        QString line = in.readLine();
        int dotp = line.indexOf('.');
        if(dotp)
        {
            _proc_name=line.left(dotp);
            line.resize(dotp);
        }
        else
        {
            _proc_name=line;
        }
    }
    return pid;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::move_out(const QPoint& steps)
{
    /*
    XWindowAttributes xwa;
    XGetWindowAttributes(QX11Info::DPY(), _x_wid, &xwa);
    xwa.y=steps.y();
    XMoveWindow(DPY(),_x_wid,xwa.x,xwa.y);
    */

}


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::get_wm_normal_hints()
{
    XSizeHints *xsizehints;
    if ((xsizehints = XAllocSizeHints()) == NULL)
        sleep(1);

    long hints;
    if (XGetWMNormalHints(DPY(), _x_wid, xsizehints, &hints) == 0)
        xsizehints->flags = 0;

    _wm_hitflags = xsizehints->flags;
    qDebug() << "WINDOW FLAGS = " << _wm_hitflags;
    _gravity = xsizehints->win_gravity;

}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::get_wm_hints()
{
    XWMHints *xwmhints;
    if ((xwmhints = XGetWMHints(DPY(), _x_wid)) != NULL)
    {
        if (xwmhints->flags & StateHint && xwmhints->initial_state == X11_IconicState)
        {
            set_xcli_state(X11_IconicState);
        }
        if (xwmhints->flags & IconPixmapHint)
        {
            get_x11_icon(xwmhints->icon_pixmap, (xwmhints->flags & IconMaskHint)?(xwmhints->icon_mask):(X11_None));
        }
        _wannafocus = xwmhints->input;
        XFree(xwmhints);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::get_wm_name()
{
    char *name;
    Atom type;
    int format;
    unsigned long nitems=0;
    unsigned long extra=0;
    unsigned char *data=NULL;

    if (XFetchName(DPY(), _x_wid, &name) && name != NULL)
    {
        _xcli_name = QString::fromUtf8((char *)name);
        XFree(name);
    }
    else
    {
        XGetWindowProperty(DPY(), _x_wid, x11atoms::atom(x11atoms::NET_WM_NAME),
                           0, 500, FALSE, AnyPropertyType, &type, &format, &nitems, &extra, &data);

        if (data != NULL)
        {
            _xcli_name = QString::fromUtf8((char *)data);
            XFree(data);
        }
        else    // use class hints
            _xcli_name = _app_name;
    }
    data = NULL;
    XGetWindowProperty(DPY(), _x_wid, x11atoms::atom(x11atoms::NET_WM_ICON_NAME),
                       0, 500, FALSE, AnyPropertyType, &type, &format, &nitems, &extra, &data);

    if (data != NULL)
    {
        _icon_name = QString::fromUtf8((char *)data);
        XFree(data);
    }
    else if (XGetIconName(DPY(), _x_wid, &name) && name != NULL)
    {
        _icon_name = name;
        XFree(name);
    }
    else
        _icon_name = _xcli_name;

    if (_app_imagez.isNull())
    {
        _app_imagez.load_images((const char*)this->_proc_name.toUtf8(),
                            (const char*)this->_app_name.toUtf8(),
                            (const char*)this->_icon_name.toUtf8(), "appicon.png",0);
    }

    if (_p_sysmenubar != NULL)
        _p_sysmenubar->set_text(_xcli_name);
    qDebug() << "WINDOW NAME: " << _xcli_name;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::get_app_name(void)
{
    XClassHint c;
    c.res_name = NULL;
    c.res_class = NULL;
    XGetClassHint(DPY(), _x_wid, &c);
    if (c.res_class != NULL)
        _app_name = QString::fromUtf8((char *)c.res_class);
    else if (c.res_name != NULL)
        _app_name = QString::fromUtf8((char *)c.res_class);
    else
        _app_name = _xcli_name;
    XFree(c.res_name);
    XFree(c.res_class);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::get_protocols(void)
{
    Atom *protocols;
    int nprot,i;

    _has_prot_delete = false;
    _has_proto_tk_focus = false;

    if (XGetWMProtocols(DPY(), _x_wid, &protocols, &nprot))
    {
        for (i=0; i < nprot; i++)
        {
            if (protocols[i] == x11atoms::atom(x11atoms::WM_DELETE_WINDOW))
            {
                _has_prot_delete = true;
            }
            else if (protocols[i] == x11atoms::atom(x11atoms::WM_TAKE_FOCUS))
            {
                _has_proto_tk_focus = true;
            }
        }
        XFree(protocols);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::set_xcli_state(int state)
{
    ulong data[2];
    data[0] = (ulong)state;
    data[1] = (ulong)X11_None;
    _xw_state = state;
    XChangeProperty(DPY(), _x_wid, x11atoms::atom(x11atoms::WM_STATE),
                    x11atoms::atom(x11atoms::WM_STATE), 32, PropModeReplace, (uchar *)data, 2);
}


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::get_x11_icon(Pixmap icon, Pixmap mask)
{
    int ix, iy;
    uint iw, ih, bwidth, depth;
    Window w;
    GC gc;

    if (!XGetGeometry(DPY(), icon, &w, &ix, &iy, &iw, &ih, &bwidth, &depth))
    {
        // no icon defined - set default icon
        _app_imagez = Imagez(CFG(_theme),"appicon.png");
        return;
    }
    QPixmap pix(iw, ih);
    pix.detach();
    gc = XCreateGC(DPY(), icon, 0, 0);
    XCopyArea(DPY(), icon, pix.handle(), gc, 0, 0, iw, ih, 0, 0);
    XFreeGC(DPY(), gc);

    if (mask != X11_None)
    {
        gc = XCreateGC(DPY(), mask, 0, 0);
        QBitmap bmap(iw, ih);
        bmap.detach();
        XCopyArea(DPY(), mask, bmap.handle(), gc, 0, 0, iw, ih, 0, 0);
        pix.setMask(bmap);
        XFreeGC(DPY(), gc);
    }

    _app_imagez = pix.scaledToHeight(PA->__Dsk()->height(), Qt::SmoothTransformation);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::get_color_maps(void)
{
    XWindowAttributes attr;
    XGetWindowAttributes(DPY(), _x_wid, &attr);
    _colormap = attr.colormap;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::set_color_maps(Colormap cmap)
{
    if (cmap== X11_None)
        _colormap = DefaultColormap(DPY(), DefaultScreen(DPY()));
    XInstallColormap(DPY(), _colormap);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::resize_request(int cx, int cy, int cw, int ch)
{
    cond_if (_resizeds,return);

    base_w = cw;
    base_h = ch;
    n_px = cx;
    n_py = cy+CFGX("caption_height").toInt()+8;
    fixed_sizes();
    n_pw = base_w;
    n_ph = base_h + CFGX("caption_height").toInt() + 8;
    resize(n_pw, n_ph);
    usleep(0xFFFF);
    XResizeWindow(DPY(), _x_wid, base_w, base_h);
    usleep(0xFFFF); // does not apply without this
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::late_grab(int t)
{
    XUngrabKeyboard(DPY(), CurrentTime);
    _kb_captured = false;
    QTimer::singleShot(t, this, SLOT(slot_grab()));
}


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_grab()
{
    int ret = 0;
    if (!_kb_captured)
        ret = XGrabKeyboard(DPY(), _x_wid, TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
    if (ret !=0)
        _kb_captured = false;
    else
    {
        _kb_captured = true;
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::set_subwin_focus(WId subwindow, long timestamp)
{
    cond_if (subwindow == 0, XSetInputFocus(DPY(), _x_wid, RevertToPointerRoot, CurrentTime));
    if (_has_proto_tk_focus)   // WM_TAKE_FOCUS protocol
    {
        x11atoms::send_wm_event(_x_wid, x11atoms::atom(x11atoms::WM_TAKE_FOCUS), timestamp);
        qDebug() << "PROT. TAKE FOCUS";
    }
    XUngrabKeyboard(DPY(), CurrentTime);
    _kb_captured = false;
    QTimer::singleShot(220, this, SLOT(slot_grab()));
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::set_font()
{
    /*
    XGCValues values;
    values.background = 1;
    GC gc = XCreateGC (DPY(), _x_wid,
                       GCBackground|GCForeground, &values);

    XSetForeground ( DPY(), gc, 0xCC0000 );
    XSetBackground ( DPY(), gc,  0x202020 );
*/

}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::bring_to_top()
{
    XRaiseWindow(DPY(), _x_wid);
    XMapWindow(DPY(), _x_wid);
    show();
    qDebug() << "BRING TOP:" << this->_set._pname << ", " << this->_app_name << "\n";
    set_xcli_state(X11_NormalState);
    XFlush(DPY());
    usleep(1000);
}


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void XClient::slot_show_top()
{
    PA->appman()->set_active_xwin(this,false);
    bring_to_top();
    this->move(_set._rect.left()+2,_set._rect.top()/2 + 32);
    this->resize(_set._rect.width(),_set._rect.height());
}

