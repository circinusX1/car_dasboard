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

#ifndef QSLUNCHBUTT_H
#define QSLUNCHBUTT_H

#include "lunchbutt.h"
/* definitions for initial window state */
#ifdef WithdrawnState
#   undef WithdrawnState   // 0	/* for windows that are not mapped */
#   undef NormalState      // 1	/* most applications want to start this way */
#   undef IconicState      // 3	/* application wants to start as an icon */
#   define X11_WithdrawnState 0
#   define X11_NormalState    1
#   define X11_IconicState    2
#endif

#include <QtScript/QScriptEngine>


class QsLunchButt : public LunchButt
{
    Q_OBJECT
public:
     QsLunchButt(Panel* p, const QPoint& size, const XwnSet& cmd, QWidget *parent=0);
     virtual ~QsLunchButt();

public:
    virtual void    on_click();


protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void enterEvent(QEvent *);

private:
    QScriptEngine       _se;
    bool                _opened;
};

#endif // QSLUNCHBUTT_H
