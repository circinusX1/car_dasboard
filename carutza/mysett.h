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

#include <QRect>
#include <QFile>
#include <QStringList>
#include <stdlib.h>
#include <vector>
#include "akajson.h"

class XwnSet;
struct CfgPanel
{
    CfgPanel(): _align(1),
               _notify(false),
               _arrange(0),
               _spacing(1),
               _layout(0) //horozontal, 1 vertical 2 grid
    {
    }
    int     _align;
    bool    _notify;
    int     _arrange;
    int     _spacing;
    int     _layout;
    QRect   _rect;
    QPoint   _icons;
    QString _name;
    QString _bgimage;
    std::vector<QString> _kids;
};

class MySett;
class QtKisstu : public Cbdler
{
    QString _fn;
public:
    QtKisstu()
    {
        _curent = this->root();
    }
    QtKisstu(const QString& file)
    {
        _fn=file;
        parse(file);
        _curent = this->root();
    }
    const QString& fileName(){return _fn;}
    struct Typoc{
        std::string _s[4]={"0","-1","-1","0"};
        Typoc(const Cbdler::Node* pn){
            if(pn!=nullptr){
                size_t els = pn->count();
                for(size_t e=0;e<els;e++)
                {
                    _s[e] = pn->value(e);
                    if(_s[e]=="MAX")_s[e]="-1";
                    if(_s[e]=="NEXT")_s[e]="-2";
                    if(_s[e]=="BOTTOM")
                        _s[e]="-3";
                }
            }
        }
        QString toString()const{return QString(_s[0].c_str());}
        bool toBool()const{return _s[0]=="true" || _s[0]=="1";}
        int toInt()const{
            return std::stod(_s[0]);
        }
        QRect toRect()const{
            QRect r;
            r.setX(std::stod(_s[0]));
            r.setY(std::stod(_s[1]));
            r.setRight(std::stod(_s[2]));
            r.setBottom(std::stod(_s[3]));
            return r;
        }
        QSize toSize()const{
            return QSize(std::stod(_s[0]),
                         std::stod(_s[1]));
        }
        QPoint toPoint(){
            if(_s[0]=="MAX")_s[0]="-1";
            if(_s[1]=="MAX")_s[0]="-1";
            return QPoint(std::stod(_s[0]),
                          std::stod(_s[1]));
        }
    };

    const QString key(){
        return QString(_curent->name().c_str());
    }

    Typoc value(const char* key, int dev=0){
        const Node* pn = _curent->pnode(key);
        if(pn==nullptr)
        {
            printf("null \n");
        }
        return Typoc(pn);
    }

    const Typoc value(const QString& key, int dev0=0){
        const Node* pn = _curent->pnode((const char*)key.toUtf8());
        return Typoc(pn);
    }

    bool beginGroup(const char* ks)
    {
        const Node* pn = _curent->pnode(ks);
        if(pn){
            _curent = _curent->pnode(ks);
            if(_curent!=nullptr)
                return true;
        }
        return false;
    }

    bool  goto_child(int index)
    {
        if(_curent->count()>index)
        {
            _curent = _curent->pnode(index);
            return true;
        }
        return false;
    }

    void   goto_parent()
    {
        _curent = _curent->pparent();
    }

    int count(){
        return _curent ? _curent->count() : 0;
    }

    void endGroup()
    {
        _curent = root();
    }
    const Cbdler::Node*  _curent;
};



class Panel;
class Container;
class MySett : public QtKisstu
{
public:

    struct ShowHide{
        QString _pname;
        int    _show;
        int    _hide;
        int    _timing;
    };

    bool read_rect(MySett& conf,
                   const char* entry,
                   QRect& xpos,
                   bool ownd,
                   int offset);
    static MySett& config(const QString* ps=0);
    MySett(const QString &fileName, bool global=false);
    QString stringValue(const QString& key)
    {
        QString value = this->value(key).toString();

        if(value.contains("$PWD"))
        {
            value.replace("$PWD", _workdir);
        }
        return value;
    }
    bool ok()const{return _ok;}
    void  load_panels(std::vector<Panel*>& panels);
    const QString& base()const{return _base;}
    const QString& home()const{return _workdir;}
    const QString& desktop()const{return  _desk;}
    const QString& images()const{return _images;}
    const QPoint& drect()const{return _drect;}
    void    startapps();
    void    prestartapps();
    void    mangle(QString& s);
    bool    find_widget(const QString& name, XwnSet& outval);
    void    finalize();
    QString read_mangled(const char* entry);
    const QString& theme_path();
    bool  is_portret()const{return _drect.x() > _drect.y();}
    int   lanch_space()const{return is_portret() ? _drect.y() : _drect.y();}
    int   top_gap()const;
    int   bottom_gap()const;
private:
    int     _parse_value(const QString value, QRect& _get_screen);

public:
    QString _workdir;
    QString _base;
    QString _desk;
    QString _images;
    QString _theme;
    QString _wallpaper;
    int     _displays;
    int     _display;
    QPoint  _drect;
    int     _fontpercent;
    int     _killdelay;
    std::vector<ShowHide> _showhide;
    std::map<QString, CfgPanel>    _panels;
    bool    _ok=false;
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
    QPoint    _imgdim;
    QString  _hidepanels;
    QString  _font; // Font="10,75,1,arial"   size, weight, bold, face

    bool     load_config(MySett& s)
    {
        bool ingroup = false;

        if(s.value("disabled").toBool()==true)
        {
            return false;
        }


        _pname = s.value("Pname").toString();
        if(_pname.isEmpty())
        {
            s.beginGroup("DesktopEntry");
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
             QtKisstu s2(newfile);
             _cmd = s2.value("Exec").toString();
        }
        _name = s.value("Name").toString();
        _pname = s.value("Pname").toString();
        _owndesktop= s.value("OwnDesktop").toBool();
        _hidden= s.value("Hidden").toBool();
        _imgdim = s.value("Isize").toPoint();
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
        s.read_rect(s, "Xrect", _rect, _owndesktop, _hoffset);
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
