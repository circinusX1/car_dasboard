/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
--------------------------------------------------------------------------------

 This file is part of DASHSASH dashboard

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
/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

#include <QDebug>
#include "theapp.h"
#include "appman.h"
#include "panel.h"
#include "imagez.h"
#include "client.h"
#include "mysett.h"

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
Appman::Appman(Panel *p, const QPoint& size, const XwnSet& set, QWidget *parent)
    : OdButton(p, size, set, parent),_pactive(0),_sibling(0),_secondinst(0),_ptokill(0)
{
    PA->set_appman(this);
    setFixedSize(size.x(),size.y());
    set_image(CFG(_images).toUtf8(),set._icon);
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
Appman::~Appman()
{
    for(auto &a : _clients)
    {
        delete a;
    }
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void Appman::on_click()
{
    if(_pactive)
    {
        cond_if(_pactive->_set._noclose==1, return);
        _priv_kill_app(_pactive);
        OdButton::on_click();
    }
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
bool Appman::kill_active_app()
{
    if(!_pactive)
            return false;
    if(_pactive->_set._killapp==0)
        return false ;

    QString  k = "kill "+QString::number( _pactive->_pid);
    system((const char*)k.toUtf8());
    remove_xcli(_pactive); //leave it naturally
    _pactive=0;
    return true;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
bool Appman::_set_active_app(XClient *c, bool oneinst)
{
    cond_if((c && c->_app_name=="Dash"), return false);
    if(oneinst)
    {
        // return false;
        // TODO
        _sibling =  is_running(c->_proc_name);

        if(_sibling && _sibling->_xcli_name == c->_xcli_name &&
            c->_app_name == _sibling->_app_name )
        {
            //_secondinst = c;
            //c->keep_hidden();
            //QTimer::singleShot(800, this, SLOT(kill_app()));
            return false;
        }

    }
    _clients.insert(c);
    //QTimer::singleShot(200, this, SLOT(slot_refresh()));
    return true;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
XClient *Appman::get_x_active_client()
{
    return _pactive;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
Window Appman::get_curent_wid()
{
    cond_if(_pactive == 0, return 0);
    return _pactive->_x_wid;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
bool Appman::set_active_xwin( XClient *c, bool oneinst)
{
    if(c==0)
    {
        PA->show_panels(true);
        //hide();
        //set_bstate(QIcon::Disabled);
        //this->setName("");
        _pactive=0;
        QTimer::singleShot(200, this, SLOT(slot_refresh()));
        return false;
    }
    cond_if((c && c->_app_name=="Dash"), return false);
    if(_set_active_app(c, oneinst))
    {
        cond_if(c, c->set_frame_status(true));
        cond_if(_pactive, _pactive->set_frame_status(false));
        cond_if(_pactive && _pactive!=c && _pactive->_set._killapp, _ptokill =_pactive);

        _pactive = c;
        setText(c->_app_name);
        update();
        PA->show_panels(!_pactive->_set._owndesktop);
        QTimer::singleShot(200, this, SLOT(slot_refresh()));
        return true;
    }
    return false;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void     Appman::remove_xcli(XClient *c)
{
    bool erased=false;
    cond_if(_pactive==c, _pactive=0);
    std::set<XClient*>::iterator f = _clients.find(c);
    if(f != _clients.end())
    {
        _clients.erase(f);
        erased=true;
    }
    cond_if(erased,QTimer::singleShot(1000, this, SLOT(slot_refresh())));

}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
XClient* Appman::is_running(const QString& name)
{
    cond_if(name.isEmpty(), return 0);

    try{
        std::set<XClient*>::iterator it =  _clients.begin();
        for(;it != _clients.end(); ++it)
        {
            XClient* pc = (*it);
            if(pc && !pc->_proc_name.isEmpty())
            {
                if(pc->_proc_name==name ||
                    pc->_proc_name.contains(name) ||
                    name.contains(pc->_proc_name))
                return pc;
            }
        }
    }catch(...)
    {
        qDebug()<<"exception ...";
    }
    return 0;
}


/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
XClient* Appman::is_running(int pid)
{
    std::set<XClient*>::iterator it =  _clients.begin();
    for(;it != _clients.end(); ++it)
    {
        XClient* pc = (*it);
        cond_if(pc->_pid==pid, return pc);
    }
    return 0;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void  Appman::slot_refresh()
{
    if(!_pactive)
    {
        this->setText("");
        set_bstate(QIcon::Disabled);
    }
    else if( _pactive->_set._noclose)
        set_bstate(QIcon::Disabled);
    else
        set_bstate(QIcon::Normal);
    PA->refresh_buts();

    cond_if((_pactive && !_pactive->_set._owndesktop), PA->top_up_panels(_pactive));

    update();
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
bool  Appman::make_visible(const QString& app, bool show)
{
    XClient* pxc = is_running(app);
    if(pxc)
    {
        if(show)
            rescan_visibilities(pxc);
        else
            pxc->hide();
        usleep(0xFFFF);
        return true;
    }
    return false;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void  Appman::rescan_visibilities(XClient* ontop)
{
    std::set<XClient*>::iterator it =  _clients.begin();
    for(;it != _clients.end(); ++it)
    {
        XClient* pc = (*it);
        if(ontop != pc)
        {
            if(pc->_set._xrect == ontop->_set._xrect)
            {
                //pc->unmap();
                pc->hide();
                pc->slot_minimize_now();
            }
        }
    }
    ontop->bring_to_top();
    set_active_xwin(ontop, false);

}


/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void  Appman::_draw_text()
{
}


/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void  Appman::_priv_kill_app(XClient* pc)
{

    QString kill="kill ";

    if(pc->_pid!=0)
    {
        kill+= QString::number(pc->_pid);
        for(int k=0;k<2;++k)
        {
            system((const char*)kill.toUtf8());
            usleep(0xFFFF);
        }
    }
    if(PA->is_process(pc->_set))
    {
        kill = "pkill " + pc->_set._pname;
        for(int k=0;k<3;++k)
        {
            system((const char*)kill.toUtf8());
            usleep(0xFFFF);
        }
    }
    if(!PA->is_process(pc->_set))
    {
        _clients.erase(pc);
    }
    else //hide it then
    {
        rescan_visibilities(pc);
    }
}

