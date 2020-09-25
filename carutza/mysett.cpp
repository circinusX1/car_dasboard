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

#include <stdio.h>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include "theapp.h"
#include "mysett.h"
#include "panel.h"


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
MySett::MySett(const QString &fileName, bool global):_killdelay(100)
{
    char pw[256];
    ::getcwd(pw, 255);
    _workdir = pw;

    if(this->parse((const char*)fileName.toUtf8())==nullptr)
    {
        return;
    }


    _curent = this->root();
    _setdir = _workdir+"/config/";

    _base=this->value("dashboard").toString();
    _fontpercent=this->value("font_percent").toInt();
    _killdelay=this->value("kill_delay").toInt();
    mangle(_base);
    if(global)
    {
        _desk = _base+"Desktop/";
        _images = _base+"icons/";
    }
    _drect.setX(PA->desktop()->width());
    _drect.setY(PA->desktop()->height());

    //up to 8 panels
    int posy = 0;
    for(int k=0;k<8;++k)
    {
        CfgPanel    pset;
        QString     pe("panel");
        pe += QString::number(k);

        if(false == this->beginGroup(pe))
            continue;
        pset._rect = this->value("position").toRect();

        if(pset._rect.left()<0)
            pset._rect.setLeft(0);
        int tp = pset._rect.top();
        if(tp<0){
            if(tp==-3){
                pset._rect.moveTo(0,drect().y()-pset._rect.height());
            }
            else{
                pset._rect.moveTo(0,posy);
            }
        }
        if(pset._rect.right()<0)
            pset._rect.setWidth(this->_drect.y());
        if(pset._rect.bottom()<0){
            pset._rect.setHeight(this->_drect.y()-posy);
        }

        int x,y,w,h;
        x=pset._rect.left();
        y=pset._rect.top();
        w=pset._rect.width();
        h=pset._rect.height();

        pset._name    = pe;
        pset._icons   = this->value("icons").toPoint();

        pset._align   = this->value("align").toInt();

        pset._notify  = this->value("notify").toBool();
        pset._arrange = this->value("arrange").toInt();
        pset._spacing = this->value("spacing").toInt();
        pset._bgimage = this->value("bgimage").toString();
        pset._dir     = this->value("config").toString();
        if(!pset._bgimage.isEmpty())
        {
            if(pset._bgimage[0]=='/')
            {
                //absolute path
                QString img = pset._bgimage;
                pset._bgimage = _workdir + pset._bgimage;
            }
        }
        pset._dir     += "/";
        pset._dir     += this->value("basedir").toString();
        pset._dir     += "/";
        _panels[pe]   = pset;
        posy += pset._rect.height();
        this->endGroup();
    }

/*
    QString vn;
    _theme =  _images + "theme/" + value("theme").toString()+"/";    //=32
    this->beginGroup("apps");
    QStringList keys = this->allKeys();

    for(int k=0; k < keys.size(); ++k)
    {
        qDebug() << keys.at(k);
        QString val = this->value(keys.at(k)).toString();
        if(val.contains(","))
        {
            QStringList vals = val.split(",");
            ShowHide    sh;

            sh._pname = keys.at(k);
            vn = vals.at(0);

            sh._show = vn.toInt();
            vn = vals.at(1);

            sh._hide = vn.toInt();
            vn = vals.at(2);

            sh._timing = vn.toInt();
            _showhide.push_back(sh);
        }
    }
    this->endGroup();
*/

    //?    if(value("caption_height").toInt()==0)
    //?        this->setValue("caption_height",18);
    _displays = this->value("displays").toInt();
    _display = this->value("display").toInt();

    _ok=true;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void MySett::finalize()
{
    QColor _prev_color =
            QColor(value("last_highlight").toString());
    PA->app_set_hilhight_color(_prev_color);
    PA->app_set_click_rule(value("minimize_dbl_click", false).toBool());
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void MySett::startapps()
{
    QString apps = this->value("start").toString();
    QStringList lapps= apps.split(",");
    for(int k=0; k< lapps.size(); ++k)
    {
        QString app = lapps.at(k);
        mangle(app);
        QStringList l = app.split("/");
        if(l.size())
        {
            if(PA->is_process(l.back()))
                return;
        }
        else
        {
            if(PA->is_process(app))
                return;
        }
    }
    apps += "&";
    system(apps.toUtf8());
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void MySett::prestartapps()
{
    QString apps = this->value("prestart").toString();
    QStringList lapps= apps.split(",");
    for(int k=0; k< lapps.size(); ++k)
    {
        QString app = lapps.at(k);
        mangle(app);
        app += " &";
        system(apps.toUtf8());
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
const QString&  MySett::theme_path()
{
    return _theme;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void MySett::mangle(QString& s)
{
    if(s.contains("$PWD"))
    {
        s.replace("$PWD", _workdir);
    }

    else if(s.contains("$THEME"))
    {
        s.replace("$THEME", _theme);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
int MySett::top_gap()const
{
    int tp = 0;
    std::map<QString, CfgPanel>::const_iterator it = _panels.begin();
    for(;it != _panels.end();++it)
    {
        const CfgPanel& p = (*it).second;
        if(p._rect.bottom() < this->_drect.y()/2 && p._rect.height()>0) //pane;l is on top
            tp+=p._rect.height();
    }
    return tp;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
int MySett::bottom_gap()const
{
    int tp = 0;
    std::map<QString, CfgPanel>::const_iterator it = _panels.begin();
    for(;it != _panels.end();++it)
    {
        const CfgPanel& p = (*it).second;
        if(p._rect.bottom() > this->_drect.y()/2) //pane;l is on top
            tp+=p._rect.height();
    }
    return tp;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void MySett::load_panels(std::vector<Panel*>& panels)
{
    std::map<QString, CfgPanel>::iterator it = _panels.begin();
    for(;it != _panels.end();++it)
    {
        CfgPanel& pc = (*it).second;
        Panel* p = new Panel(&pc,0);
        panels.push_back(p);
    }
}


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
bool MySett::find_widget(const QString& name, XwnSet& outval)
{
    if(name.length()==0)
    {
        return false;
    }
    QDir            dir("./");
    QFileInfoList   files = dir.entryInfoList();
    foreach(const QFileInfo &fi, files)
    {
        if(fi.fileName().at(0)=='.')
            continue;
        QString     sfile = fi.absoluteFilePath();
        QString     bn = fi.baseName();
        if(!fi.isDir() &&
                (sfile.endsWith(".widget") || sfile.endsWith(".desktop") || sfile.endsWith(".desktopp")) )
        {
            MySett    df(sfile);
            if(!df.ok())continue;
            QString   pname = df.value("Pname").toString();
            if(!pname.isEmpty() && (pname == name || name.contains(pname)))
            {
                outval.Load(df);
                qDebug()<< outval._name << outval._rect.left() << "," <<outval._rect.top();
                return true;
            }
        }
    }
    //load default rect
    outval._rect.setCoords(0,
                           top_gap(),
                           drect().x(),
                           drect().y()-bottom_gap());

    outval._rpos=0;
    return true;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
MySett& MySett::config(const QString* file)
{
    static MySett s(*file,  1);
    return s;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
QString MySett::read_mangled(const char* entry)
{
    QString s = this->value(entry).toString();
    mangle(s);
    return s;
}

bool MySett::read_rect( MySett& conf,
               const char* entry,
               QRect& xpos,
               bool ownd,
               int offset)
{

    xpos = conf.value("rect").toRect();
    if(xpos.width()<=0)
    {
        int temp = conf.value(entry).toInt();
        if(_displays==0) //no multiple displays
            temp=0;

        //CfgPanel     _notification;
        //CfgPanel     _launcher;
        //CfgPanel     _quickapps;

        int tp=CFG(top_gap());
        if(temp==0 && tp) //whole display
        {
            xpos.setCoords(0,
                           ownd? 0 : CFG(top_gap())-offset,
                           CFG(drect()).x(),
                           CFG(drect()).y()-(ownd? 0 : CFG(bottom_gap())));
        }
        else if(temp==1) //upper side
        {
            xpos.setCoords(0,
                           ownd? 0 : CFG(top_gap()),
                           CFG(drect()).x(),
                           CFG(drect()).y()/2);
        }
        else if(temp==2) //lower side
        {
            xpos.setCoords(0,
                           CFG(drect()).y()/2,
                           CFG(drect()).x(),
                           CFG(drect()).y()-(ownd? 0 : CFG(bottom_gap())));
        }
    }
    return xpos.width()>0;
}
