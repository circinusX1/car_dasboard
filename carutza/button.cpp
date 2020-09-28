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
#include "button.h"
#include "mysett.h"
#include "imagez.h"
#include "theapp.h"

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
OdButton::OdButton(CtrlHolder *p, const QPoint& sz,const XwnSet& set, QWidget *parent):
    QToolButton(parent),_set(set),
                        _btype(TEXT_BUTON),
                        _state(S_NORMAL),
                        _initsz(sz),
                        _abtomove(false)
{
    Q_UNUSED(p);
    cond_if(_initsz.x()<=0, _initsz=QPoint(64,64));
    setFixedSize(sz.x(), sz.y());
    setText(set._name);

    //Font="14,75,arial"   size, weight, bold, face
    QFont f = QFont(font());
    if(!_set._font.isEmpty())
    {
        QStringList lf = _set._font.split(',');
        for(int k=0;k<lf.size();++k)
        {
            if(lf[k]=="0")   lf[k]="16";
            switch(k)
            {
                case 0:

                    f.setPointSize(lf[k].toInt());
                    break;
                case 1:
                    f.setWeight(lf[k].toInt());
                    break;
                case 2:
                    f.setBold(lf[k].toInt()==1);
                    break;
                case 3:
                    f.setFamily(lf[k]);
                    break;
                default:
                    break;
            }
        }
    }
    else
    {
        f.setBold(true);
        f.setWeight(75);
        int fsz = _initsz.y() * (CFG(_fontpercent)/100);
        cond_if(fsz==0,  fsz=12);
        f.setPointSize(fsz);
    }
    setFont(f);
    if(set._state>0)
    {
        set_bstate((OdButton::E_BSTATE)set._state);
    }
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
OdButton::~OdButton()
{
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
bool OdButton::set_cat_image(const char* path, const char* imagefile)
{
    Imagez  ipixa;
    if(imagefile=="0")
        return false;
    if(ipixa.load_image(path,imagefile))
    {
       _caticon.addPixmap(ipixa, QIcon::Normal, QIcon::Off);
        return true;
    }
    return false;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
bool OdButton::set_image(const char* path, const char* imagefile)
{
    Imagez  ipixa;
    int     images[] = {'A','D','S', 0 };
    int     states[] = {QIcon::Active, QIcon::Disabled, QIcon::Selected};
    bool    textonly = false;

    if(*imagefile==0)
        return false;
    if(imagefile==0 || imagefile[0]=='0')
        return false;
    if(!ipixa.load_image(path,imagefile))
    {
        QPixmap pixmap(_initsz.x()-2,_initsz.y()-2);

        pixmap.fill(QColor(192,192,192));
        _icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
        textonly=true;
    }
    else
        _icon.addPixmap(ipixa, QIcon::Normal, QIcon::Off);

    for(int c = 0; images[c]; ++c)
    {
        QString fname;
        Imagez  ipix;

        const char*   pd = strchr(imagefile,'.');
        if(pd)
        {
            char loco[256];
            ::strcpy(loco, imagefile);

            *strchr(loco,'.')=0;
            fname = QString(loco) + QString((char)images[c]);
        }
        else
            fname = imagefile + QString((char)images[c]);

        if(ipix.load_image(path, fname,false))
        {
            _icon.addPixmap(ipix, (QIcon::Mode)states[c], QIcon::Off);
        }
        else
        {
            _icon.addPixmap(ipixa, (QIcon::Mode)states[c], QIcon::Off);
        }
    }
    cond_if(!textonly, _btype|=(int)OdButton::IMG_BUTON);
    return true;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void OdButton::_draw_content()
{
    if(_set._bckcolor<=0)
    {
        QPainter painter(this);
        QRect    rect = painter.window();
        QColor   base;

        painter.setRenderHint(QPainter::Antialiasing);
        if (_state & S_SELECTED)
        {
            QLinearGradient linearGrad(QPointF(0, rect.top()), QPointF(0, rect.top() + height()));

            base = QColor(palette().highlight().color());
            linearGrad.setColorAt(0, base.lighter(96));
            linearGrad.setColorAt(1, base.lighter(32));
            painter.fillRect(0, rect.top(), width(), height(), linearGrad);
        }
        else if (_state & S_ACTIVE)
        {
            QLinearGradient linearGrad(QPointF(0, rect.top()), QPointF(0, rect.top() + height()));

            base = QColor(palette().button().color());
            linearGrad.setColorAt(0, base);
            linearGrad.setColorAt(1, base.darker(128));
            painter.fillRect(0, rect.top(), width(), height(), linearGrad);
        }
        else if (_state & S_DISABLED)
        {
            QLinearGradient linearGrad(QPointF(0, rect.top()), QPointF(0, rect.top() + height()));

            linearGrad.setColorAt(0, QColor(64,64,64));
            linearGrad.setColorAt(1, QColor(16,16,16));
            painter.fillRect(0, rect.top(), width(), height(), linearGrad);
        }
        else
        {
            QLinearGradient linearGrad(QPointF(0, rect.top()), QPointF(0, rect.top() + height()));

            base = QColor(palette().button().color());
            linearGrad.setColorAt(0, base);
            linearGrad.setColorAt(1, base.darker(200));
            painter.fillRect(0, rect.top(), width(), height(), linearGrad);
        }
    }
    else
    {
        QPainter painter(this);
        QRect    rect = painter.window();
        QColor   base(QRgb(_set._bckcolor));

        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(0, rect.top(), width(), height(), base);
    }
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::_draw_text()
{
    QPainter painter(this);
    QRect    rect(painter.window());
    QRect    offset(rect.left()+1, rect.top()+2, rect.width()-1, rect.height()-2);

    painter.setRenderHint(QPainter::Antialiasing);
    if(_set._fontcolor<=0)
    {
        if (_state & S_SELECTED)
        {
            //mco-mco
            painter.setPen(QColor(255,0,0)); // palette().highlightedText().color());
        }
        else
        {
            painter.setPen(QColor(0,0,0));
        }
    }
    else
    {
        painter.setPen(QColor(QRgb(_set._fontcolor)));
    }
    if(_btype & IMG_BUTON)//has image
    {
        if(_set._talign<=0)
            painter.drawText(rect, Qt::AlignCenter|Qt::AlignBottom, this->text());
        else
            painter.drawText(rect,(Qt::AlignmentFlag)_set._talign, this->text());
    }
    else
    {
        if(_set._talign<=0)
            painter.drawText(rect, Qt::AlignCenter, this->text());
        else
            painter.drawText(rect,(Qt::AlignmentFlag)_set._talign, this->text());
    }
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::_draw_image()
{
    QPainter painter(this);
    QRect    rect(painter.window());
    QSize    sz = _icon.actualSize(this->size(),QIcon::Normal, QIcon::Off);
    int      x = (_initsz.x()-sz.width())/2;
    int      y = (_initsz.y()-sz.height())/2;
    int      icstate[]= {QIcon::Normal,
                         QIcon::Active, 0, 0,
                         QIcon::Disabled, 0, 0, 0,
                         QIcon::Selected};

    //if(_state!=0) {x-=1;y-=1;}

    if(_set._ialign<=0)
    {
        _icon.paint(&painter, x, y, sz.width(), sz.height(), Qt::AlignCenter,
                    (QIcon::Mode)icstate[_state], QIcon::Off);
    }
    else
    {
        _icon.paint(&painter, x, y, sz.width(), sz.height(), (Qt::AlignmentFlag)_set._ialign,
                    (QIcon::Mode)icstate[_state], QIcon::Off);
    }

    if(!_caticon.isNull())
    {
        QSize    sz = _caticon.actualSize(this->size(),QIcon::Normal, QIcon::Off);

         _caticon.paint(&painter,rect.width()-sz.width(),
                                 0,
                                 sz.width(),
                                 sz.height(),
                                 Qt::AlignCenter,
                                 QIcon::Normal,
                                 QIcon::Off);
    }
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::paintEvent(QPaintEvent *)
{
    //_draw_content();
    cond_if((_btype & IMG_BUTON),_draw_image());
    cond_if (!text().isNull(),_draw_text());
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::enterEvent(QEvent *)
{
    cond_if(_set._nopush,return);
    cond_if(_state & S_DISABLED,return);
    set_bstate(S_ACTIVE);
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::leaveEvent(QEvent *)
{
    cond_if(_set._nopush,return);
    cond_if(_state & S_DISABLED,return);
    set_bstate(S_NORMAL);
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::mousePressEvent(QMouseEvent *event)
{
    cond_if(_set._nopush,return);
    cond_if(_state & S_DISABLED,return);

    set_bstate(S_SELECTED);
    activate();
    QPoint pt = event->pos();
    _presspt  = pt;
    _initp    = _presspt = event->pos();
    _abtomove = true;
    _moved    = false;
    update();
    //update again !!!
    // QTimer::singleShot(100, this, SLOT(paint_again()));
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::mouseMoveEvent(QMouseEvent * event)
{
    if(_abtomove &&
       ((abs(_presspt.x()-event->pos().x()) > 16) ||
       (abs(_presspt.y()-event->pos().y()) > 16)))
    {
        _moved =true;
        set_bstate(S_NORMAL);
        on_moving(event->pos().x()-_presspt.x(),
                  event->pos().y()-_presspt.y());
    }
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::mouseReleaseEvent(QMouseEvent *event)
{
    cond_if(_set._nopush,return);
    cond_if(_state & S_DISABLED,return);

    set_bstate(S_ACTIVE);
    if(!_moved)
    {
        on_click(); //we need to activate the app
       _abtomove = false;
    }
    else
    {
        on_moved(pos().x() - event->pos().x());
    }
    //QTimer::singleShot(100, this, SLOT(paint_again()));
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::on_click()
{
    QApplication::beep ();
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::on_moving(int,int)
{
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::on_moved(int)
{
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
bool OdButton::is_activable()
{
    return !(_state & S_DISABLED);
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::activate()
{
    cond_if(_state & S_DISABLED, return);
    if(menu())
    {
        PA->wm_add_ignorewid(this->winId());
        PA->wm_add_ignorewid(menu()->winId());
        menu()->popup(this->mapToGlobal(QPoint(0, height())));
    }
    set_bstate(S_SELECTED);
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::deactivate()
{
    cond_if(menu(), menu()->hide());
    set_bstate(S_NORMAL);
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::slot_show_menu()
{
    //activate();
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::set_font_size(const QSize& sz)
{
    QFont f = QFont(font());
    f.setBold(true);
    f.setWeight(75);
    f.setPointSize(sz.height());
    setFont(f);
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::refresh()
{
    /*
    QPoint tp = this->pos();
    tp.setX(tp.x()+1);
    move(tp);
    tp.setX(tp.x()-1);
    move(tp);
    */
    update();
}

/*--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void OdButton::slot_paint_again()
{
    update();
}
