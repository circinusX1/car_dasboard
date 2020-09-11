/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @ mylinuz.com July-Sept 2013
Project:    CAROWA
--------------------------------------------------------------------------------

 This file is part of DASHSASH dashboard

    DASHDASH is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DASHDASH is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DASHDASH.  If not, see <http://www.gnu.org/licenses/>.
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
#include "container.h"


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
MySett::MySett(const QString &fileName, Format format, int global):
        QSettings(fileName, format),_home(getenv("HOME"))
{
    _base=this->value("dashboard").toString();
    _fontpercent=this->value("font_percent").toInt();
    mangle(_base);
    if(global)
    {
        _desk = _base+"Desktop/";
        _images = _base+"icons/";
    }
    _drect.setCoords(0,0,PA->desktop()->width(),PA->desktop()->height());

    //up to 8 panels
    for(int k=0;k<8;++k)
    {
        CfgPanel   pset;
        QString     pe("panel");
        pe += QString::number(k);

        this->beginGroup(pe);
            pset._position = this->value("position").toSize();
            if(pset._position.width()>=0)            //is enabled
            {
                pset._name    = pe;
                pset._icons   = this->value("icons").toSize();
                pset._height  = this->value("height").toInt();
                pset._position = this->value("position").toSize();
                pset._align   = this->value("align").toInt();
                pset._dir     = _desk+this->value("basedir").toString();
                pset._notify  = this->value("notify").toBool();
                pset._width   = this->value("width").toInt();
                pset._arrange = this->value("arrange").toInt();
                pset._spacing = this->value("spacing").toInt();
                pset._bgimage = this->value("bgimage").toString();
                _panels[pe]   = pset;
            }
        this->endGroup();
    }

    //up to 8 containers
    for(int k=0;k<8;++k)
    {
        CfgPanel   pset;
        QString     pe("container");
        pe += QString::number(k);

/*
        name=container0
        height=96
        align=1
        position=@Size(0 64)
        widget0=".overflow/left.control"
        widget1=@launcher
        widget2=".overflow/right.control"
        widget2=".overflow/home.control"
        widget0=".overflow/left.control"
        widget1=@launcher
        widget2=".overflow/right.control"
        widget2=".overflow/home.control"
*/

        this->beginGroup(pe);
            pset._position = this->value("position").toSize();
            if(pset._position.width()>=0)            //is enabled
            {
                pset._height = this->value("height").toInt();
                pset._position = this->value("position").toSize();
                pset._align = this->value("align").toInt();
                pset._name= pe;
                pset._arrange= this->value("arrange").toInt();
                pset._spacing= this->value("spacing").toInt();
                pset._width= this->value("width").toInt();
                //up to 8 children
                for(int c=0;c<8;++c)
                {
                    QString     pe("widget");
                    pe += QString::number(c);

                    pe = this->value(pe).toString();
                    if(!pe.isEmpty())
                        pset._kids.push_back(pe);
                    else
                        break;
                }
                _containers[pe] = pset;
            }
        this->endGroup();
    }
    QString vn;
    _theme =  _images + "theme/" + value("theme").toString()+"/";//=32
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

    if(value("caption_height").toInt()==0)
        this->setValue("caption_height",18);
    _displays = this->value("displays").toInt();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void MySett::finalize()
{
	QColor _prev_color = QColor(value("last_highlight", QColor(0, 0, 255).name()).toString());
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
        system(apps.toUtf8());
    }
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
    if(s.contains("$HOME"))
    {
        s.replace("$HOME", _home);
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
        if(p._position.height() < this->_drect.height()/2) //pane;l is on top
            tp+=p._height+1;
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
        if(p._position.height() > this->_drect.height()/2) //pane;l is on top
            tp+=p._height+1;
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
void MySett::load_containers(std::vector<Container*>& containers)
{
    std::map<QString, CfgPanel>::iterator it = _containers.begin();
    for(;it != _containers.end();++it)
    {
        CfgPanel& pc = (*it).second;
        Container* p = new Container(&pc,0);
        containers.push_back(p);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
bool MySett::read_rect(QSettings& s, const QString& entry, QRect& xpos, bool ownd)
{
    Q_UNUSED(ownd);
    MySett& conf = MySett::config();
    xpos = s.value(entry).toRect();
    if(xpos.width()<=0)
    {
        int temp = s.value(entry).toInt();
        if(conf._displays==0) //no multiple displays
            temp=0;

        //CfgPanel     _notification;
        //CfgPanel     _launcher;
        //CfgPanel     _quickapps;

        if(temp==0) //whole display
        {
            xpos.setCoords(0,
                           conf.top_gap(),
                           conf.drect().width(),
                           conf.drect().height()-conf.bottom_gap());
        }
        else if(temp==1) //upper side
        {
            xpos.setCoords(0,
                           conf.top_gap(),
                           conf.drect().width(),
                           conf.drect().height()/2);
        }
        else if(temp==2) //lower side
        {
            xpos.setCoords(0,
                           conf.drect().height()/2,
                           conf.drect().width(),
                           conf.drect().height()-conf.bottom_gap());
        }
    }
    return xpos.width()>0;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
bool MySett::find_widget(const QString& name, XwnSet& outval)
{
    if(name.length()==0)
    {
        return false;
    }
    QDir            dir(_desk);
    QFileInfoList   files = dir.entryInfoList();
    foreach(const QFileInfo &fi, files)
    {
        if(fi.fileName().at(0)=='.')
            continue;
        QString     sfile = fi.absoluteFilePath();
        QString     bn = fi.baseName();
        if(!fi.isDir() &&
            (sfile.endsWith(".widget") || sfile.endsWith(".desktop")) )
        {
            QSettings df(sfile, QSettings::NativeFormat, 0);
            QString   pname = df.value("Pname").toString();
            if(!pname.isEmpty() && (pname == name || name.contains(pname)))
            {
                outval.Load(df);
                qDebug()<< outval._name << outval._rect.left() << "," <<outval._rect.top();
                return true;
            }
        }
    }

    QString msg = "Cnfig file for: ";
    msg += name + " not found in " + _desk;
/*
    QMessageBox msgBox;
    PA->wm_add_ignorewid(msgBox.winId());
    msgBox.setText(msg);
    msgBox.exec();

  PA->wm_remove_ignoredwid(msgBox.winId());
  */
  return false;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
MySett& MySett::config(const QString* file)
{
    static MySett s(*file, QSettings::NativeFormat, 1);
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
