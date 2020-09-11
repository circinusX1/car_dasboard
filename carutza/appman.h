/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
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

*/

#ifndef _KILLCTRL_H
#define _KILLCTRL_H

#include "button.h"
#include <set>
#include <QTimer>

class Panel;
class XClient;
class Appman : public OdButton
{
    Q_OBJECT
public:
    Appman(Panel *p, const QSize& size, const XwnSet& set, QWidget *parent = 0);
    ~Appman();
   virtual void on_click();

public:
    XClient*    get_x_active_client();
    Window      ge_curent_wid();
    bool        set_active_xwin(XClient *c, bool oneinst);
    void        remove_xcli(XClient *c);
    XClient*    is_running(const QString& name);
    XClient*    is_running(int pid);
    void        rescan_visibilities( XClient* );
    bool        make_visible(const QString& app, bool visible);
    Window      get_curent_wid();
    bool        kill_active_app();

public slots:
    void         slot_refresh();

private:
    bool        _set_active_app( XClient *c, bool oneinst);
    void        _priv_kill_app(XClient* pc);
signals:
    void        sig_moving(Appman*, int, int);
    void        sig_clicked(Appman*);
    void        sig_scrolled(int);

protected:
    virtual void _draw_text();
private:
    XClient*            _pactive;
    XClient*            _sibling;
    XClient*            _secondinst;
    XClient*            _ptokill;
    QTimer              _timer;
    QPoint              _spos;
    bool                _goup;
    int                 _delta;
    int                 _step;
    std::set<XClient*>  _clients;
};

#endif

