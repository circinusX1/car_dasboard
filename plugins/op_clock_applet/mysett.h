#ifndef DASHCONFIG_H
#define DASHCONFIG_H

#include <QSettings>
#include <QRect>
#include <stdlib.h>
#include <vector>

class XwnSet;
struct CfgPanel
{
    CfgPanel():_height(0),_width(-1),_align(1){}
    QSize   _position;
    QSize   _icons;
    int     _height;
    int     _width; // container needs this
    int     _align;
    QString _dir;
    bool    _notify;
    QString _name;
    int     _arrange;
    int     _spacing;
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


    static bool read_rect(QSettings&s, const QString& entry ,QRect& xpos, bool ownd=false);
    static MySett& config(const QString* ps=0);
    MySett(const QString &fileName, Format format, int global=false);
    QString stringValue(const QString& key)
    {
        QString value = this->value(key).toString();

        if(value.contains("$HOME"))
        {
            value.replace("$HOME", _home);
        }
        return value;
    }
    void  load_panels(std::vector<Panel*>& panels);
    void  load_containers(std::vector<Container*>& containers);
    const QString& base()const{return _base;}
    const QString& home()const{return _home;}
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
    QString _home;
    QString _base;
    QString _desk;
    QString _images;
    QString _theme;
    QString _wallpaper;
    int     _displays;
    QRect   _drect;
    int     _fontpercent;
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
        WIG_CONTAINER,
    }WIDGET_TYPE;
    XwnSet():_frame(true),_noclose(false),_owndesktop(false),_bckcolor(0),_align(-1){
    }
    bool     _frame;
    bool     _noclose;
    bool     _owndesktop;
    bool     _visible;
    bool     _hidden;
    WIDGET_TYPE _wt;
    QString  _name;
    QString  _pname;
    QRect    _rect;
    QString  _cmd;
    QString  _icon;
    int      _bckcolor;
    int      _fontcolor;
    QString  _caticon;
    QSize    _icwh;
    int      _rpos;
    int      _align;
    QString  _hidepanels;
    bool     _nopush;
    bool     _bshowrun;
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
        MySett::read_rect(s, "Xrect", _rect, _owndesktop);
        _name = s.value("Name").toString();
        _pname = s.value("Pname").toString();
        _owndesktop= s.value("OwnDesktop").toBool();
        _hidden= s.value("Hidden").toBool();
        _icwh= s.value("Isize").toSize();
        _rpos = s.value("Xrect").toInt();
        _cmd = s.value("Exec").toString();
        _icon = s.value("Icon").toString();
        _bckcolor= s.value("bckcolor").toInt();
        _fontcolor= s.value("fontcolor").toInt();
        _caticon = s.value("CatIcon").toString();
        _frame = s.value("Xframe").toBool();
        _noclose = s.value("Noclose").toBool();
        _align = s.value("align").toInt();
        _hidepanels = s.value("HidePanels").toString();
        _nopush = s.value("nopush").toBool();
        _bshowrun = s.value("SHowState").toBool();
        _font = s.value("Font").toString();
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
