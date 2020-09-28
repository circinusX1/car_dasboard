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
#include "ctrlholder.h"
#include "theapp.h"


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
CtrlHolder::CtrlHolder(CfgPanel* pconfpan, QWidget *parent)
                   :QWidget(parent),_q_pixmap(0),
                                    _pcfg(pconfpan),
                                    _layout(0)
{
    PA->wm_add_ignorewid(this->winId());

    if(pconfpan && !pconfpan->_bgimage.isEmpty())
    {
        QFileInfo fileInfo(pconfpan->_bgimage);
        QString filename(fileInfo.fileName());
        if(filename.at(0)<'9') // is rgb
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
CtrlHolder::~CtrlHolder()
{
    std::vector<QWidget*>::iterator b = _butons.begin();
    for(;b!= _butons.end();++b)
        delete *b;
    delete _layout;
    delete _q_pixmap;
}


//-----------------------------------------------------------------------------
void CtrlHolder::_config_ui()
{
}

void  CtrlHolder::_layout_it(int  buts, int spacing, const QRect& maxw)
{
    if(_layout)
        delete _layout;
    _layout = new QGridLayout(nullptr,1,buts+1/*mid stretch*/,0,spacing);

    _layout->setSizeConstraint(QBoxLayout::SetMaximumSize);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    _layout->setGeometry(maxw);
    _leftovers = maxw.width();
/*
    int cs = _layerwidth / buts;
    for(int c=0;c<buts;c++)
        _layout->setColStretch(c,cs);
*/
    this->setLayout(_layout);
    _lefts = 0;
    _rights = buts;
}

// stretch the remainin mid one to separate left and rights
void   CtrlHolder::_re_stretch()
{
    _layout->setColStretch(_lefts,_leftovers);
}

void CtrlHolder::_add_widget(QWidget* pw,
                             const QPoint& icx,
                             int col, int row, int align)
{
    int xcx = icx.x();
    if(align==0x0001){

        _layout->addWidget(pw,row,_lefts,Qt::AlignmentFlag(align));
        _layout->setColStretch(_lefts,xcx);
        ++_lefts;
        _leftovers-=xcx;
    }
    else{
        _layout->addWidget(pw,row,_rights,Qt::AlignmentFlag(align));
        _layout->setColStretch(_rights,xcx);
        --_rights;
        _leftovers-=xcx;
    }
}

//-----------------------------------------------------------------------------
void CtrlHolder::_load_controls(const QString& folder)
{
    Q_UNUSED(folder);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
OdButton*  CtrlHolder::get_widget(const QString& pname)const
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
void CtrlHolder::paintEvent(QPaintEvent *)
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

void CtrlHolder::run_app(OdButton* pb)
{
    PA->runapp(pb->xset());
}



