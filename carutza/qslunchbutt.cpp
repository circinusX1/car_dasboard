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

#include "panel.h"
#include "qslunchbutt.h"

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
QsLunchButt::QsLunchButt(Panel* p, const QPoint& size, const XwnSet& cmd,
                         QWidget *parent):LunchButt(p,size,cmd,parent),_opened(false)
{
    QString fn(p->cur_dir() + cmd._pname);
    QFile   qf(fn);
    if(qf.open(QIODevice::ReadOnly))
    {
        //setup context of the script
        this->setObjectName(cmd._pname);
        QScriptValue sb = _se.newQObject(this);
        _se.globalObject().setProperty("button", sb);

        _opened=true;
        QTextStream stream(&qf);
        QString contents = stream.readAll();
        qf.close();
        _se.evaluate(contents);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
QsLunchButt::~QsLunchButt()
{

}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void    QsLunchButt::on_click()
{
    emit sig_clicked(this);
    OdButton::on_click();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void QsLunchButt::paintEvent(QPaintEvent * pe)
{
    LunchButt::paintEvent(pe);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void QsLunchButt::enterEvent(QEvent *p)
{
    Q_UNUSED(p);
    //LunchButt::enterEvent(p);
}

