/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     antico
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

#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QPaintEvent>
#include "qwidget2.h"
#include "theapp.h"


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
QWidget2::QWidget2(CfgPanel* pconfpan, QWidget *parent)
                   :QWidget(parent),_q_pixmap(0),
                                    _pconfpan(pconfpan),
                                    _layout(0)
{
    PA->wm_add_ignorewid(this->winId());

    if(pconfpan && !pconfpan->_bgimage.isEmpty())
    {

        if(pconfpan->_bgimage[0]<'9') // is rgb
        {
            _q_pixmap = new Imagez(4,4,0);
            _q_pixmap->fromrgb(pconfpan->_bgimage.toInt());

        }
        else
        {
            _q_pixmap = new Imagez("",pconfpan->_bgimage.toUtf8());
        }
    }
}

//-----------------------------------------------------------------------------
QWidget2::~QWidget2()
{
    std::vector<QWidget*>::iterator b = _butons.begin();
    for(;b!= _butons.end();++b)
        delete *b;
    delete _layout;
    delete _q_pixmap;
}


//-----------------------------------------------------------------------------
void QWidget2::_config_ui()
{
}

//-----------------------------------------------------------------------------
void QWidget2::_load_controls(const QString& folder)
{
    Q_UNUSED(folder);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
OdButton*  QWidget2::get_widget(const QString& pname)const
{
    std::vector<QWidget*>::const_iterator b = _butons.begin();
    for(;b!= _butons.end();++b)
    {
        OdButton* pb = dynamic_cast<OdButton*>(*b);
        if(pb && pb->xset()._pname == pname)
        {
            return pb;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
void QWidget2::paintEvent(QPaintEvent *)
{
    if(_q_pixmap && !_q_pixmap->isNull())
    {
        QPainter paint(this);
        paint.drawPixmap(0, 0, *_q_pixmap);
    }
    else
    {
        QPainter painter(this);
        QRect    rect = painter.window();
        QColor   base(QColor(palette().background().color()));

        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(0, rect.top(), width(), height(), base);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/

void QWidget2::run_app(OdButton* pb)
{
    PA->runapp(pb->xset());
}



