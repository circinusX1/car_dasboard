/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     egg, antico
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

*/


#ifndef __TITLEBAR_H
#define __TITLEBAR_H

#include "defs.h"
#include "imagez.h"

//-----------------------------------------------------------------------------
class XClient;
class FrmWinBorder : public QLabel
{
	Q_OBJECT
    friend class XClient;
public:
	FrmWinBorder(QWidget *parent=0);
	~FrmWinBorder();

signals:
	void sig_mouse_move(QMouseEvent *);
	void sig_mlbutton(QMouseEvent *);
	void sig_mlbutton();
	void sig_mrbutton();
	void sig_mmbutton();
	void sig_mrelease();

protected:
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
};

//-----------------------------------------------------------------------------
class FrmSysButs : public QLabel
{
	Q_OBJECT

public:

    enum eSYS_BUT_STATE {
        EnormalState,
        EactiveState,
        EinactiveState,
	};

	enum eSYS_BUT_TYPE {
		CloseButton,
		MinimizeButton,
		MaximizeButton,
		ExpandButton
	};

	FrmSysButs(int buttonType, QWidget *parent = 0);
	~FrmSysButs();

public slots:
	void set_state(int s);

signals:
	void sig_lbutton_down();
	void sig_rbutton_down();
	void sig_mbutton_down();

protected:
	virtual void mousePressEvent(QMouseEvent *);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void paintEvent(QPaintEvent *);

private:
    Imagez*     _norm_img;
    Imagez*     _active_img;
    Imagez*     _incactive_img;
	int         _state;
	int         _prev_state;
	eSYS_BUT_TYPE  _type;
};

//-----------------------------------------------------------------------------
class XFrameBar : public QWidget
{
	Q_OBJECT
public:
    friend class XClient;
	enum eBORDER_STATE {
		borderActive,
		borderInactive
	};

	enum eBORDER_CLASS {
		normalWindow,
		dialogWindow,
		inspectorWindow,
	};

	enum e_CURSOR_POS {
		NonePos,
		LeftPos,
		MiddlePos,
		RightPos,
	};

	XFrameBar(QWidget *parent = 0);
	~XFrameBar();

	virtual void set_state(int);

signals:
    void sig_mouse_release();
    void sig_mouse_move(QMouseEvent *);
    void sig_lbutton_down(QMouseEvent *);
    void sig_rbutton_down();
    void sig_mbutton_down();
	void sig_mouse_dblclk();

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);

protected:
	int     _border_state;
	bool    _dirty_cursor;
	bool    _mouse_captured;
	int     _cur_pos_state;
};

//-----------------------------------------------------------------------------
class SysButsBar : public XFrameBar
{
	Q_OBJECT
public:
    static int      get_optim_h(int);

    SysButsBar(const Imagez &, const QString &, int _role = 0, QWidget *parent = 0);
	~SysButsBar();

	virtual void    set_state(int);
	void            set_text(const QString &);
    FrmSysButs*     pb_close(){return _pb_close;};
    FrmSysButs*     pb_minimize(){return _pb_minimize;};
    FrmSysButs*     pb_maximize(){return _pb_maximize;};
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);

protected:
    Imagez          _image;
	QString         _caption;
	int             _wclass;
	FrmSysButs*     _pb_close;
    FrmSysButs*     _pb_minimize;
	FrmSysButs*     _pb_maximize;
};

#endif
