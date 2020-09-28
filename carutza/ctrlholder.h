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
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "imagez.h"

class QPixmap;
class QColor;
class CfgPanel;
class OdButton;
class LunchButt;


class CtrlHolder : public QWidget
{
    Q_OBJECT

public:
    CtrlHolder(CfgPanel* pconfpan, QWidget *parent = 0);
    virtual ~CtrlHolder();
    OdButton*  get_widget(const QString& pname)const;
    CfgPanel* conf()const{return  _pcfg;}

protected:
    virtual void _config_ui();
    virtual void _load_controls(const QString& folder);
    void        _layout_it(int,int, const QRect& maxw);
    void        _add_widget(QWidget* pw, const QPoint& icx,
                            int col, int row=0, int al=Qt::AlignLeft);
    void        _re_stretch();
protected:
    void paintEvent(QPaintEvent *event);

public slots:
    //void run_app2(OdButton* pb);
    void run_app(OdButton* pb);

protected:
    Imagez*   _q_pixmap;
    CfgPanel* _pcfg;
    QGridLayout*  _layout = nullptr;
    std::vector<QWidget*>   _butons;
    int             _layerwidth=0;
    int             _lefts;
    int             _rights;
    int             _leftovers=0;
};

#endif

