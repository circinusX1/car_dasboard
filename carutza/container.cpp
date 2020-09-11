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


#include <QDir>
#include <QObject>
#include "panel.h"
#include "menu.h"
#include "theapp.h"
#include "client.h"
#include "mysett.h"
#include "panel.h"
#include "imagez.h"
#include "appman.h"
#include "qslunchbutt.h"
#include "container.h"

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
Container::Container(CfgPanel* pe, QWidget *parent): QWidget2(pe, parent)
{
     _config_ui();
     //setBackgroundColor(QColor(0,0,0));
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
Container::~Container()
{
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Container::_config_ui()
{
    if(_pconfpan->_height>0)
    {
        // _background.load_image("", _pconfpan->_bgimage);

        if(_pconfpan->_width<=0)_pconfpan->_width=CFG(_drect.width());
        this->move(_pconfpan->_position.width(), _pconfpan->_position.height());
        this->setFixedSize(_pconfpan->_width, _pconfpan->_height);

        _layout = new QHBoxLayout();
        _layout->setSpacing(_pconfpan->_spacing);
        if(_pconfpan->_align==1)
            _layout->setDirection(QBoxLayout::LeftToRight);
        else if(_pconfpan->_align==2)
            _layout->setDirection(QBoxLayout::RightToLeft);

        _layout->setContentsMargins(0, 0, 0, 0);
        _load_controls(QString());
        this->setLayout(_layout);
        this->show();
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void Container::_load_controls(const QString& folder)
{
    Q_UNUSED(folder);

    // calc widths
    int                                  allwidth = 0, allpans=0;
    std::vector<XwnSet>                  ctrls;
    const std::vector<QString>&          controls = _pconfpan->_kids;
    std::vector<QString>::const_iterator it =   controls.begin();
    for(; it != controls.end(); ++it)
    {
        const QString& whada = *it;
        if(whada.at(0)!='@')
        {
            QString filen = CFG(_desk)+whada;
            QSettings df(filen, QSettings::NativeFormat, 0);
            XwnSet butset;
            if(butset.Load(df))
            {
                butset._wt = XwnSet::WIG_INERNAL;
                if(butset._icwh.width()<=0)
                {
                    butset._icwh = _pconfpan->_icons;
                }
                if(butset._align==-1)
                {
                    butset._align = _pconfpan->_align;
                }
                ctrls.push_back(butset);
                allwidth += butset._icwh.width() + _pconfpan->_spacing;
            }
        }
        else
        {
            XwnSet butset;
            butset._pname = whada.right(whada.length()-1);
            butset._wt = XwnSet::WIG_CONTAINER;
            ctrls.push_back(butset);
            ++allpans;
        }
    }

    // layout them
    if(allpans==0)allpans=1;
    int panels_width = (_pconfpan->_width - allwidth) / allpans;
    std::vector<XwnSet>::const_iterator setts = ctrls.begin();
    for(; setts != ctrls.end(); ++setts)
    {
        const XwnSet& bset = (*setts);
        if(bset._wt == XwnSet::WIG_CONTAINER)
        {
             Panel* p = PA->get_panel(bset._pname);
             if(p)
             {
                 QWidget* fixed =  new QWidget(this);
                 // not specified, just fit it
                 if(p->conf()->_width<=0)
                     p->conf()->_width = panels_width;
                 fixed->setFixedSize(p->conf()->_width, p->conf()->_height);
                 p->setParent(fixed);
                 fixed->show();
                 fixed->setBackgroundColor(QColor(0,0,0));
                 _layout->addWidget(fixed, 0, Qt::AlignLeft);
                 _butons.push_back(fixed);
             }
        }
        else
        {
            char cwd[256];
            ::getcwd(cwd,255);

            QString env = getenv("HOME");env+="/";
            QString file=env; file.append(".carutza/Desktop/icons_bandw/");


            LunchButt* pb = new LunchButt(this, bset._icwh, bset);
            pb->set_image(file,bset._icon);
            if(!bset._caticon.isEmpty())  pb->set_cat_image("",bset._caticon);
            _layout->addWidget(pb, 0,(Qt::AlignmentFlag)bset._align);
            _butons.push_back(pb);
            pb->show();
            connect(pb,SIGNAL(sig_clicked(OdButton*)),this,SLOT(run_app(OdButton*)));

        }
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
/*
void Container::run_app2(OdButton* pb)

{
    PA->runapp(pb->xset());
}
*/



