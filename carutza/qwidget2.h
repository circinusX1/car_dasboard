/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     antico
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
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com

*/

#ifndef __QWidget2_H
#define __QWidget2_H

#include <QWidget>
#include <QLayout>
#include "imagez.h"

class QPixmap;
class QColor;
class CfgPanel;
class OdButton;
class LunchButt;
class QWidget2 : public QWidget
{
    Q_OBJECT

public:
    QWidget2(CfgPanel* pconfpan, QWidget *parent = 0);
    virtual ~QWidget2();
    OdButton*  get_widget(const QString& pname)const;
    CfgPanel* conf()const{return  _pconfpan;}
protected:
    virtual void _config_ui();
    virtual void _load_controls(const QString& folder);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    //void run_app2(OdButton* pb);
    void run_app(OdButton* pb);

protected:
    Imagez*                 _q_pixmap;
    CfgPanel*               _pconfpan;
    QHBoxLayout*            _layout;
    std::vector<QWidget*>   _butons;
};

#endif

