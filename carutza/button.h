/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits    antico
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
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com
--------------------------------------------------------------------------------
*/

#ifndef __GENERIC_BUTTON_H
#define __GENERIC_BUTTON_H

#include "mysett.h"
#include "defs.h"

class CtrlHolder;
class OdButton : public QToolButton
{
	Q_OBJECT
public:
    enum E_BSTATE{           //a long bitmap
        S_NORMAL=0x0,
        S_ACTIVE=0x1,
        S_DISABLED=0x4,
        S_SELECTED=0x8,
    };

    enum E_BTYPE {
                    IMG_BUTON=0x1,
                    TEXT_BUTON=0x2
                 };
    explicit OdButton(CtrlHolder *p, const QPoint& sz, const XwnSet& set,
                       QWidget *parent);
	virtual ~OdButton(void);
    const   XwnSet& xset()const{return _set;}
    bool    set_image(const char* path, const  char* imagefile);

    bool    set_cat_image(const char* path, const char* imagefile);

    void    set_bstate(OdButton::E_BSTATE s){_state=s; update();}
    void    set_font_size(const QSize& sz);
    void    refresh();

	virtual bool is_activable();
	virtual void activate();
	virtual void deactivate();
    virtual void on_click();
    virtual void on_moving(int,int);
    virtual void on_moved(int);
public slots:
	void    slot_show_menu();
    void    slot_paint_again();


signals:
    void sig_moving(OdButton*, int, int);
    void sig_clicked(OdButton*);
    void sig_scrolled(int);


protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *); //
    virtual void mouseMoveEvent(QMouseEvent *);

	void _draw_content();
    virtual void _draw_text();
	void _draw_image();

protected:
    XwnSet      _set;
    int         _btype;
    E_BSTATE    _state;
    QIcon       _icon;
    QIcon       _caticon;
    QPoint       _initsz;
    QPoint      _presspt;
    QPoint      _initp;
    bool        _abtomove;
    bool        _moved;

};

#endif
