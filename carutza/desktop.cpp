/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     egg
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
#include "desktop.h"
#include "theapp.h"
#include "imagez.h"
#include "mysett.h"

extern TheApp* APP;

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
Desktop::Desktop(QWidget *parent) : QWidget2(0, parent)
{
    PA->wm_add_ignorewid(this->winId());
    _boxlayout = new QHBoxLayout();
    _boxlayout->setContentsMargins(0, 0, 0, 0);
    setLayout(_boxlayout);

}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
Desktop::~Desktop()
{
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Desktop::init()
{
    setGeometry(CFG(top_gap()),
                0,
                CFG(_drect).width(),
                CFG(_drect).height()-CFG(bottom_gap()));
    load_wall_paper(CFG(_theme));
    this->lower();
    setWindowFlags(Qt::WindowStaysOnBottomHint);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Desktop::load_wall_paper(const QString &path)
{
    Q_UNUSED(path);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Desktop::run_menu(QAction *)
{
}
