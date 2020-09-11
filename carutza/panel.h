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
--------------------------------------------------------------------------------

*/

#ifndef __LANCER_H
#define __LANCER_H

#include <QObject>
#include <QWidget>
#include <QPropertyAnimation>
#include "defs.h"
#include "qwidget2.h"
#include "dskwatch.h"
#include "imagez.h"
#include <vector>

#define TOP_Lancer_HEIGHT 64

class TheApp;
class XClient;
class LunchButt;
class QsLunchButt;
struct XwnSet;
struct CfgPanel;
class Panel : public QWidget2
{
    Q_OBJECT
public:
    explicit Panel(CfgPanel* e, QWidget *parent=0);
    virtual ~Panel(void);

    void  refresh_buts();
    void  ahide(bool hide);

    const QString& cur_dir()const{ return _curfolder;}
    void update_widget(const QString& name, const QString& value);
    void upupdate_widgetdate_widget(const QString& name, const QPixmap& value);

public slots:
    void scrool_lancer(OdButton* lb, int dx ,int dy);
    void done_scrolling(int);
    void change_folder(OdButton* pb);
    void run_app(OdButton* pb);
    void slot_pressed(OdButton* pb);
    void run_intern(OdButton* pb);
    void slot_align_edge();
    void slot_anim_done();
    void slot_floder_ch(const QString& folder);
    void slot_change_folder();
    void fit_to_parent(const QSize& proom);
protected:
    void    _config_ui();
    void    _load_controls(const QString& folder);
    void    _done_scrooling(int);
    void    _scrool_lancer(int dx ,int dy);
    void    _add_button(const XwnSet& xs, int index) ;
    size_t  _load_buts(const QString& folder, const QSize& sz, std::vector<XwnSet>& buts);
    void    _calc_overflow();
    void    _update_btnsel();
    void    _overlay_overflow();
    int     _parent_width();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    DskWatch*       _inotify;
    QString         _basefolder;
    QString         _curfolder;
    bool            _abtomove;
    QPoint          _presspt;
    QPropertyAnimation *_panimation;
    bool            _moved;
    int             _icwidth;
    int             _overflow;
    int             _layerwidth;
    int             _selected;
    bool            _hidden;
    bool            _folderchanged;
};


#endif

