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


#ifndef CONTAINER_H
#define CONTAINER_H

#include "qwidget2.h"
#include "lunchbutt.h"


class Container : public QWidget2
{
public:
    Container(CfgPanel* pe, QWidget *parent);
    virtual ~Container();
protected:
    virtual void _config_ui();
    virtual void _load_controls(const QString& folder);

public slots:
    void run_app2(OdButton* pb);

};


#endif // CONTAINER_H
