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


void  CtrlHolder::_layout_it()
{
    if(_pcfg->_layout==0){
        _layout = new QHBoxLayout(nullptr,0,0);
        ((QHBoxLayout*)_layout)->setSpacing(_pcfg->_spacing);
        ((QHBoxLayout*)_layout)->setDirection(QBoxLayout::LeftToRight);
    }
    else if(_pcfg->_layout==1)
        _layout = new QVBoxLayout(nullptr,0,_pcfg->_spacing);
    else {
        _layout = new QGridLayout(nullptr,
                                  _pcfg->_rect.width()+_pcfg->_icons.x()/
                                  _pcfg->_icons.x()+_pcfg->_spacing,
                                  _pcfg->_rect.height()+_pcfg->_icons.y()/
                                  _pcfg->_icons.y()+_pcfg->_spacing,0,
                                  _pcfg->_spacing+_pcfg->_icons.x());
    }
    _layout->setSizeConstraint(QBoxLayout::SetMaximumSize);
    _layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_layout);
}

void CtrlHolder::_add_widget(QWidget* pw, int align)
{
    const char* cn = _layout->metaObject()->className();
    if(::strcmp(cn,"QGridLayout"))
        ((QHBoxLayout*)_layout)->addWidget(pw, 0,Qt::AlignLeft);
    else {
        ((QGridLayout*)_layout)->addWidget(pw);
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



