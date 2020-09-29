/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits    egg, antico
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
metacity:   Copyright (C) 2001 Havoc Pennington
xfwm4:      here and there source code
matchbox:   here and there source code
*/

#include <QIcon>
#include "mysett.h"
#include "wxframe.h"
#include "x11atoms.h"
#include "imagez.h"


#define CAP_W 96
#define CAP_H 96

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
FrmWinBorder::FrmWinBorder(QWidget *parent) : QLabel(parent)
{
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
FrmWinBorder::~FrmWinBorder()
{
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void FrmWinBorder::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		emit sig_mrbutton();
	} else if (event->button() == Qt::LeftButton) {
		emit sig_mlbutton();
	} else if (event->button() == Qt::MidButton) {
		emit sig_mmbutton();
	}
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void FrmWinBorder::mouseReleaseEvent(QMouseEvent *)
{
	emit sig_mrelease();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void FrmWinBorder::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton) { // to block right button mouse move
        emit sig_mouse_move(event);
	}
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
FrmSysButs::FrmSysButs(int type, QWidget *parent)
	: QLabel(parent),
      _type((eSYS_BUT_TYPE)type)
{
	int cxy = CFGX("caption_height").toInt();

	setFixedHeight(cxy);
	setFixedWidth(cxy);

    _state = _prev_state = EnormalState;
	QString pathActive, pathNormal;
	switch (_type) {
		case CloseButton:
			pathActive = "closewin.png";
			pathNormal = "closewin.png";
			break;
        case ExpandButton:
		case MaximizeButton:
			pathActive = "maximize.png";
			pathNormal = "maximize.png";
			break;
		case MinimizeButton:
			pathActive = "minimize.png";
			pathNormal = "minimize.png";
			break;
        default:
            break;
	}
	//Normal, Disabled, Active, Selected }
	if(pathNormal.length())
	{
		_norm_img = new Imagez(CFG(_images).toUtf8(),  pathNormal.toUtf8());
		_active_img = new Imagez(Imagez::new_pixmap(*_norm_img, QIcon::Active));
		_incactive_img = new Imagez(Imagez::new_pixmap(*_norm_img, QIcon::Disabled));
		update();
	}
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
FrmSysButs::~FrmSysButs()
{
	delete _active_img;
	delete _norm_img;
	delete _incactive_img;
}

void FrmSysButs::set_state(int s)
{
	_state = s;
	_prev_state = _state;
	update();
}

void FrmSysButs::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRect rect = painter.window();

	int cxy = CFGX("caption_height").toInt();
	// icon size is QSize(18, 17)
	int top = (rect.height() - (cxy-1)) / 2;
	int left = (rect.width() - cxy) / 2;

	QRect pixRect(left, top, cxy, cxy-1);

    if (_state == EnormalState) {
		painter.drawPixmap(pixRect, *_norm_img);
    } else if (_state == EactiveState) {
		painter.drawPixmap(pixRect, *_active_img);
    } else if (_state == EinactiveState) {
		painter.drawPixmap(pixRect, *_incactive_img);
	}
}

void FrmSysButs::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		emit sig_rbutton_down();
	} else if (event->button() == Qt::LeftButton) {
		emit sig_lbutton_down();
	} else if (event->button() == Qt::MidButton) {
		emit sig_mbutton_down();
	}
}

void FrmSysButs::enterEvent(QEvent *)
{
    if (_state != EactiveState) {
		_prev_state = _state;
        _state = EactiveState;
		update();
	}
}

void FrmSysButs::leaveEvent(QEvent *)
{
	if (_state != _prev_state) {
		_state = _prev_state;
		update();
	}
}

XFrameBar::XFrameBar(QWidget *parent)
	: QWidget(parent)
{
	_border_state = borderActive;
	setFixedHeight(12);
	resize(width(), 12);
	_dirty_cursor = true;
	setMouseTracking(true);
	_cur_pos_state = XFrameBar::NonePos;
	_mouse_captured = false;
}

XFrameBar::~XFrameBar()
{
}

void XFrameBar::set_state(int state)
{
	_border_state = state;
	update();
}

void XFrameBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        emit sig_rbutton_down();
    }
    else if(event->button() == Qt::LeftButton)
    {
        emit sig_lbutton_down(event);
    }
    else if(event->button() == Qt::MidButton)
    {
        emit sig_mbutton_down();
    }
	_mouse_captured = true;
}

void XFrameBar::mouseMoveEvent(QMouseEvent *event)
{
	if (_dirty_cursor && !_mouse_captured) {
		if (event->x() >= width() - 8 || event->y() >= height() - 8) {
			setCursor(QCursor(Qt::SizeFDiagCursor));
			_cur_pos_state = XFrameBar::RightPos;
		} else {
			setCursor(QCursor(Qt::ArrowCursor));
			_cur_pos_state = XFrameBar::NonePos;
		}
	}
    if(event->buttons() == Qt::LeftButton) // to block right button mouse move
    {
        emit sig_mouse_move(event);
    }
}

void XFrameBar::mouseReleaseEvent(QMouseEvent *)
{
    emit sig_mouse_release();
	_mouse_captured = false;
}

void XFrameBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

	QRect rect = painter.window();
	QColor base;
	base = palette().button().color();
	if (_border_state == borderActive)
		base = base.darker(140);

	QLinearGradient linearGrad(QPointF(0, rect.top()), QPointF(0, rect.top() + height()));
	linearGrad.setColorAt(0, base.lighter(125));
	linearGrad.setColorAt(1, base.darker(150));
	painter.fillRect(0, rect.top(), width(), height(), linearGrad);
}


SysButsBar::SysButsBar(const Imagez &imagez, const QString &title, int wclass, QWidget *parent)
	: XFrameBar(parent)
{
	_image = imagez;
	_caption = title;
	_wclass = wclass;
	_dirty_cursor = false;
	setMouseTracking(false);
	//_border_state = borderActive;

	QFont f = QFont(font());
	f.setBold(true);
    f.setWeight(75);
    f.setPointSize(24);
    setFont(f);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	if (_wclass != x11atoms::Dialog) {
		_pb_close = new FrmSysButs(FrmSysButs::CloseButton);
		layout->addWidget(_pb_close);
		_pb_close->show();
		//layout->addSpacing(1);

		_pb_minimize = new FrmSysButs(FrmSysButs::MinimizeButton);
		layout->addWidget(_pb_minimize);
        _pb_minimize->show();
		//layout->addSpacing(1);

		_pb_maximize = new FrmSysButs(FrmSysButs::MaximizeButton);
		layout->addWidget(_pb_maximize);
        _pb_maximize->show();
		//layout->addSpacing(1);
	} else {
		_pb_close = NULL;
		_pb_minimize = NULL;
		_pb_maximize = NULL;
	}
	layout->addStretch();
	setLayout(layout);
	int ch = CFGX("caption_height").toInt();
    setFixedHeight(ch);
    resize(width(), ch);
}

SysButsBar::~SysButsBar()
{
}

int SysButsBar::get_optim_h(int _wclass)
{
	Q_UNUSED(_wclass);
    return CFGX("caption_height").toInt();
}

void SysButsBar::set_state(int state)
{
	_border_state = state;
	if (_wclass != x11atoms::Dialog) {
		if (state == borderInactive) {
            _pb_close->set_state(FrmSysButs::EinactiveState);
            _pb_minimize->set_state(FrmSysButs::EinactiveState);
            _pb_maximize->set_state(FrmSysButs::EinactiveState);
		} else if (state == borderActive) {
            _pb_close->set_state(FrmSysButs::EnormalState);
            _pb_minimize->set_state(FrmSysButs::EnormalState);
            _pb_maximize->set_state(FrmSysButs::EnormalState);
		}
	}
	update();
}

void SysButsBar::set_text(const QString &title)
{
	_caption = title;
	update();
}

void SysButsBar::paintEvent(QPaintEvent *event)
{
	XFrameBar::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

	//QRect rect = painter.window();
	QColor base;
	base = palette().button().color();
	if (_border_state == borderActive)
		base = base.darker(140);
	painter.setPen(base.darker(250));// text color

	int h = height() + 4;
	if (_image.isNull()) {
		h = 0;
	}
	int w = (width() - fontMetrics().width(_caption) - h) / 2;

	painter.drawPixmap(QRect(w, 2, height()-4, height()-4), _image,
		QRect(0, 0, _image.width(), _image.height()));//icon
    painter.drawText(QRect(w + h, 0, width(), height()),
		Qt::AlignVCenter, _caption); //text
}

void SysButsBar::mouseDoubleClickEvent(QMouseEvent *)
{
	emit sig_mouse_dblclk();
}
