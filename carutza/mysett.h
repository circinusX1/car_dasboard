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

#ifndef DASHCONFIG_H
#define DASHCONFIG_H

#include <QSettings>
#include <QRect>
#include <QFile>
#include <stdlib.h>
#include <vector>

class XwnSet;
struct CfgPanel
{
    CfgPanel():_height(-1),
               _width(-1),
               _align(1),
               _notify(false),
               _arrange(0),
               _spacing(1)
    {
    }
    int     _height;
    int     _width; // container needs this
    int     _align;
    bool    _notify;
    int     _arrange;
    int     _spacing;
    QSize   _position;
    QSize   _icons;
    QString _dir;
    QString _name;
    QString _bgimage;
    std::vector<QString> _kids;
};


class Panel;
class Container;
class MySett : public QSettings
{
public:

    struct ShowHide{
        QString _pname;
        int    _show;
        int    _hide;
        int    _timing;
    };


    static bool read_rect(QSettings&s, const QString& entry ,QRect& xpos,bool ownd=false,
                          int off=0);
    static MySett& config(const QString* ps=0);
    MySett(const QString &fileName, Format format, int global=false);
    QString stringValue(const QString& key)
    {
        QString value = this->value(key).toString();

        if(value.contains("$PWD"))
        {
            value.replace("$PWD", _workdir);
        }
        return value;
    }
    void  load_panels(std::vector<Panel*>& panels);
    void  load_containers(std::vector<Container*>& containers);
    const QString& base()const{return _base;}
    const QString& home()const{return _workdir;}
    const QString& desktop()const{return  _desk;}
    const QString& images()const{return _images;}
    const QRect& drect()const{return _drect;}
    void    startapps();
    void    prestartapps();
    void    mangle(QString& s);
    bool    find_widget(const QString& name, XwnSet& outval);
    void    finalize();
    QString read_mangled(const char* entry);
    const QString& theme_path();
    bool  is_portret()const{return _drect.height() > _drect.width();}
    int   lanch_space()const{return is_portret() ? _drect.width() : _drect.height();}
    int  top_gap()const;
    int  bottom_gap()const;
private:
    int     _parse_value(const QString value, QRect& _get_screen);

public:
    QString _workdir;
    QString _setdir;
    QString _base;
    QString _desk;
    QString _images;
    QString _theme;
    QString _wallpaper;
    int     _displays;
    int     _display;
    QRect   _drect;
    int     _fontpercent;
    int     _killdelay;
    std::vector<ShowHide> _showhide;
    std::map<QString, CfgPanel>    _panels;
    std::map<QString, CfgPanel>    _containers;
};



struct XwnSet
{
    typedef enum _WIDGET_TYPE{
        WIG_DESKTOP,
        WIG_APPLET,
        WIG_INERNAL,
        WIG_SIGNAL,
        WIG_CONTAINER,
        WIG_BACK,
    }WIDGET_TYPE;
    XwnSet():_frame(0),
            _noclose(0),
            _owndesktop(0),
            _hidden(0),
            _bckcolor(0),
            _fontcolor(0),
            _rpos(0),
            _align(0),
            _ialign(0),
            _talign(0),
            _nopush(0),
            _bshowrun(0),
            _hoffset(0),
            _killapp(false),
            _state(8),
            _wt(WIG_DESKTOP)
    {
    }
    bool     _frame;
    bool     _noclose;
    bool     _owndesktop;
    bool     _hidden;
    int      _bckcolor;
    int      _fontcolor;
    int      _rpos;
    int      _align;
    int      _ialign;
    int      _talign;
    bool     _nopush;
    bool     _bshowrun;
    int     _hoffset;
    bool    _killapp;
    int     _state;
    WIDGET_TYPE _wt;
    QString  _name;
    QString  _pname;
    QRect    _rect;
    QString  _cmd;
    QString  _icon;
    QString  _caticon;
    QSize    _icwh;
    QString  _hidepanels;
    QString  _font; // Font="10,75,1,arial"   size, weight, bold, face

    bool     Load(QSettings& s)
    {
        bool ingroup = false;
        _pname = s.value("Pname").toString();
        if(_pname.isEmpty())
        {
            s.beginGroup("Desktop Entry");
             ingroup = true;
        }

        _cmd = s.value("Exec").toString();

        if(!_cmd.isEmpty() && _cmd.at(0)=='@')
        {
             QString newfile = s.fileName();
             QStringList parts = newfile.split("/");
             QString lastBit = parts.at(parts.size()-1);

             newfile.replace(lastBit,"");

             newfile += _cmd.right(_cmd.length()-1);
             QSettings s2(newfile, QSettings::NativeFormat);
             _cmd = s2.value("Exec").toString();
        }
        _name = s.value("Name").toString();
        _pname = s.value("Pname").toString();
        _owndesktop= s.value("OwnDesktop").toBool();
        _hidden= s.value("Hidden").toBool();
        _icwh= s.value("Isize").toSize();
        _rpos = s.value("Xrect").toInt();
        _icon = s.value("Icon").toString();
        _caticon = s.value("CatIcon").toString();
        _frame = s.value("Xframe").toBool();
        _noclose = s.value("Noclose").toBool();
        _hidepanels = s.value("HidePanels").toString();
        _bshowrun = s.value("ShowState").toBool();
        _font = s.value("Font").toString();
        _hoffset = s.value("Hoffset").toInt();
        _killapp = s.value("Killapp").toInt();
        //state attribs
        _bckcolor= s.value("bckcolor").toInt();
        _fontcolor= s.value("fontcolor").toInt();
        _align = s.value("align").toInt();
        _ialign = s.value("ialign").toInt();
        _talign = s.value("talign").toInt();
        _nopush = s.value("nopush").toBool();
        _state = s.value("state").toInt();
        MySett::read_rect(s, "Xrect", _rect, _owndesktop, _hoffset);
        if(ingroup)
            s.endGroup();
        if(_name.isEmpty())
            _name=_pname;
        return  !_pname.isEmpty();
    }
};


#define CFG(x_) MySett::config().x_
#define CFGX(y_) MySett::config().value(y_)

#define     cond_if(a, b)  if(a) b


#endif // DASHCONFIG_H
