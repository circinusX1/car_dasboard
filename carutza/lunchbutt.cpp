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
#include "theapp.h"
#include "panel.h"

#include "button.h"
#include "mysett.h"
#include "imagez.h"
#include "lunchbutt.h"

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
LunchButt::LunchButt(CtrlHolder *p, const QPoint& size, const XwnSet& set, QWidget *parent)
    : 	OdButton(p, size, set, parent)
{

}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void    LunchButt::LunchButt::on_click()
{
    PA->_inited=true;
    emit sig_clicked(this);
    OdButton::on_click();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void    LunchButt::on_moving(int x, int y)
{
    emit sig_moving(this, x, y);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void LunchButt::on_moved(int dx)
{
    emit sig_scrolled(dx);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void LunchButt::paintEvent(QPaintEvent * pe)
{
    OdButton::paintEvent(pe);
    if(_set._bshowrun)
    {
        XClient* px = PA->is_running(_set._pname);
        if(px)
        {
            QPainter painter(this);

            Imagez& im = PA->_runicon;
            painter.drawPixmap(QRect(0,
                                     0,
                                    im.width(),
                                    im.height()),
                                    im, QRect(0, 0, im.width(),im.height()));
        }
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void LunchButt::enterEvent(QEvent *p)
{
    OdButton::enterEvent(p);
}

