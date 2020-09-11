/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @ mylinuz.com July-Sept 2013
Project:    CAROWA
Credits     matchbox, egg, antico, xfwm4, metacity
--------------------------------------------------------------------------------

 This file is part of DASHSASH dashboard

    DASHDASH is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DASHDASH is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DASHDASH.  If not, see <http://www.gnu.org/licenses/>.
*/
/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

#include <QDir>
#include <QObject>
#include "XPannel.h"
#include "menu.h"
#include "sysmenu.h"
#include "clock.h"
#include "theapp.h"
#include "client.h"
#include "mysett.h"
#include "QuickLaunch.h"
#include "imagez.h"
#include "appman.h"
#include "lunchbutt.h"


XPannel::XPannel(TheApp *a, QWidget *parent)
    : QWidget2(parent),_moved(false),_inotify(this),_overflow(0),_layerwidth(0),
      _selected(-1), _hidden(false),_folderchanged(false)
{
    Q_UNUSED(a);
    PA->wm_add_ignorewid(this->winId());
	_active_widget = 0;
	_active_wigget_id = 0;
    _panimation = new QPropertyAnimation(this, "geometry");
    connect(_panimation,SIGNAL(finished()),this,SLOT(slot_anim_done()));
   _config_ui();

   Imagez z;
   z.load_image(CFG(_images),"more.png");
   _showmore=new QIcon(z);
}

XPannel::~XPannel(void)
{
    delete _layout;
    delete _panimation;
    delete _showmore;
    _timer.stop();
}

void XPannel::_config_ui(void)
{

    move(CFG(_launcher._position.left()), CFG(_launcher._position.top()));
    _layout = new QVBoxLayout();
    _layout->setSpacing(1);
    _layout->setContentsMargins(0, 0, 0, 0);
    _basefolder = CFG(_base) + CFG(_launcher._dir);
    if(CFG(_launcher._height > 0))
    {
        _load_shortcuts(_basefolder);
        setLayout(_layout);
        show();
    }
}

size_t  XPannel::_load_buts(const QString& folder, const QSize& sz, std::vector<XwnSet>& buts)
{
    QDir            dir(folder);
    QFileInfoList   files = dir.entryInfoList();

    foreach(const QFileInfo &fi, files)
    {
        if(fi.fileName().at(0)=='.')
            continue;
        QString     sfile = fi.absoluteFilePath();

        if(sfile.endsWith(".desktop"))
        {
            QSettings df(sfile, QSettings::NativeFormat, 0);
            XwnSet butset;
            if(butset.Load(df))
            {
                butset._icwh = sz;
                buts.push_back(butset);
            }
        }
        else if(fi.isDir() && !sfile.contains("top_panel") )
        {
            QSettings df(sfile+"/desktop.conf", QSettings::NativeFormat, 0);
            XwnSet butset;
            butset.Load(df);
            butset._pname=">";
            butset._cmd=sfile; //which folder to change
            butset._icwh = sz;
            buts.push_back(butset);
        }
    }
    int wi = CFG(is_portret()) ? sz.width() : sz.height();
    if(folder != _basefolder)
        return (buts.size()+1) * (wi+1); /// add back buton space
    return buts.size() * (wi + 1);
}

void XPannel::_load_shortcuts(const QString& folder)
{
    QSize           sz;
    QSettings       dfx(folder+"desktop.conf", QSettings::NativeFormat, 0);

    std::vector<XwnSet> buts;
    sz = dfx.value("IconSize").toSize();
    if(sz.width()<=0)
        sz = CFGX("IconSize").toSize();
    if(sz.width()<=0)
        sz= QSize(64,64);

    _icwidth = CFG(is_portret()) ? sz.width() : sz.height();
    std::vector<LunchButt*>::iterator b = _butons.begin();
    for(;b!= _butons.end();++b)
        delete *b;
    _butons.clear();

    _layerwidth= _load_buts(folder, sz, buts);
    if(_layerwidth <= CFG(lanch_space()))
    {
        _layerwidth = CFG(lanch_space());
    }

    if(CFG(is_portret()))
    {
        this->setFixedWidth(_layerwidth);
        resize(_layerwidth, CFG(_lanch_pixels));
        move(0,CFG(_drect.height())-CFG(_lanch_pixels));
    }
    else
    {
        this->setFixedHeight(_layerwidth);
        resize(CFG(_lanch_pixels), _layerwidth);
        move(0,0);//CFG(_drect.height())-CFG(_lanch_pixels));
    }


    int idx=0;
    std::vector<XwnSet>::iterator it = buts.begin();
    for(;it!= buts.end();++it)
    {
        _add_button((*it), idx);
        ++idx;
    }

    if(folder != _basefolder)
    {
        XwnSet set;
        set._cmd = _basefolder;
        set._name = "Back...";
        LunchButt* pb = new LunchButt(this, sz, set);
        pb->set_image("", "back.png");
        connect(pb,SIGNAL(sig_clicked(LunchButt*)),this,
                SLOT(change_folder(LunchButt*)));
        connect(pb,SIGNAL(sig_scrolled(int)),this,
                SLOT(done_scrolling(int)));
        connect(pb,SIGNAL(sig_scrolled(int)),this,
                SLOT(done_scrolling(int)));
        _layout->addWidget(pb,0,Qt::AlignRight);
        _butons.push_back(pb);
    }
    usleep(10000);
        _inotify.watch(folder);
   _calc_overflow();
   _selected=-1;
}

void XPannel::_add_button(const XwnSet& xs, int index)
{
    Q_UNUSED(index);
    LunchButt* pb = new LunchButt(this, xs._icwh, xs);
    pb->set_image("",xs._icon);

    connect(pb,SIGNAL(sig_moving(LunchButt*,int,int)),this,SLOT(scrool_lancer(LunchButt*,int,int)));
    connect(pb,SIGNAL(sig_scrolled(int)),this,SLOT(done_scrolling(int)));

    if(xs._pname==">") // sub folder
        connect(pb,SIGNAL(sig_clicked(LunchButt*)),this,SLOT(change_folder(LunchButt*)));
    else
        connect(pb,SIGNAL(sig_clicked(LunchButt*)),this,SLOT(run_app(LunchButt*)));

    _layout->addWidget(pb, 0,Qt::AlignLeft);
    _butons.push_back(pb);
}

void XPannel::change_folder(LunchButt* pb)
{
    if(pb)
    {
        _folderchanged=true;
        usleep(1000);
        QString newfolder = pb->xset()._cmd;
        if(!newfolder.isEmpty())
        {
            _curfolder = newfolder;
            QTimer::singleShot(100, this, SLOT(slot_change_folder()));
        }
        usleep(10000);
    }
}

// from QTimer one shot
void XPannel::slot_change_folder()
{
    if(!_curfolder.isEmpty())
        _load_shortcuts(_curfolder);
}

// from Inotify
void XPannel::slot_floder_ch(const QString& folder)
{
    if(_curfolder == folder)
    {
        QTimer::singleShot(100, this, SLOT(slot_change_folder()));
    }
}

void XPannel::run_app(LunchButt* pb)
{
    _folderchanged=false;
    PA->runapp(pb->xset());
}

void XPannel::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRect rect = painter.window();

	QColor base = palette().button().color();
    int r = base.red()/3;
    int g = base.green()/3;
    int b = base.blue()/3;

	//painter.fillRect(0, rect.bottom() - 2, width(), 3, QColor(r, g, b).darker(230));
	QLinearGradient linearGrad(QPointF(0, rect.top()), QPointF(0, rect.top() + height()));
	linearGrad.setColorAt(0, QColor(r, g, b));
	linearGrad.setColorAt(1, QColor(r, g, b).darker(200));
	painter.fillRect(0, rect.top(), width(), height(), linearGrad);

    //if(_overflow & 0x1)
    //    _showmore->paint(&painter, 0,0, 16, 32);
    //if(_overflow & 0x2)
    //    _showmore->paint(&painter, rect.left()+CFG(_drect.width()-16), rect.top(), 16, 32);
}


void XPannel::scrool_lancer(LunchButt* lb, int dx ,int dy)
{
    Q_UNUSED(lb);
    _scrool_lancer(dx,dy);
}

void XPannel::done_scrolling(int cx)
{
    _done_scrooling(cx);
}

void XPannel::mousePressEvent(QMouseEvent *event)
{
    _presspt = event->pos();
    _abtomove = true;
}

void XPannel::mouseMoveEvent(QMouseEvent * event)
{
    if(_abtomove &&
       ((abs(_presspt.x()-event->pos().x()) > 16) ||
       (abs(_presspt.y()-event->pos().y()) > 16)))
    {
        _scrool_lancer(event->pos().x()-_presspt.x(),
                      event->pos().y()-_presspt.y());
        qDebug()<<event->pos().x()-_presspt.x();
    }
}

void XPannel::mouseReleaseEvent(QMouseEvent *event)
{
    int cx = this->pos().x() - event->pos().x();
    _done_scrooling(cx);
    _selected=-1;
    _update_btnsel();
}

void XPannel::_done_scrooling(int dr)
{
    Q_UNUSED(dr);
    //this->setDisabled(true);
    if(_icwidth==0)
        _icwidth=128;
    if(CFG(is_portret()))
    {
        _abtomove = false;
        int xs = this->pos().x();
        if(_moved && xs)
        {
            int maxleft = this->width() - CFG(_drect.width());
            int xe = 0;
            if(xs > 0)
            {
                xe = 0;
            }
            else if(xs < -maxleft)
            {
                xe=-maxleft;
            }
            else
            {
                int mox = abs(xs % _icwidth) > (_icwidth/2);
                xe  = (xs / _icwidth) - mox;
                xe *= _icwidth;
            }

            _panimation->setDuration(300);
            _panimation->setStartValue(QRect(xs,this->pos().y(),
                                             this->rect().width(),this->rect().height()));
            _panimation->setEndValue(QRect(xe,this->pos().y(),
                                           this->rect().width(),this->rect().height()));
            _panimation->setEasingCurve(QEasingCurve::OutCubic);
            _panimation->start();
        }
    }
    else
    {
        _abtomove = false;
        int ys = this->pos().y();
        if(_moved && ys)
        {
            int maxtop = this->height() - CFG(_drect.height());
            int ye = 0;
            if(ys > 0)
            {
                ye = 0;
            }
            else if(ys < -maxtop)
            {
                ye=-maxtop;
            }
            else
            {
                int mox = abs(ys % _icwidth) > (_icwidth/2);
                ye  = (ys / _icwidth) - mox;
                ye *= _icwidth;
            }

            _panimation->setDuration(300);
            _panimation->setStartValue(QRect(0,
                                             ys,
                                             this->rect().width(),
                                             this->rect().height()));

            _panimation->setEndValue(QRect(0,
                                           ye,
                                           this->rect().width(),
                                           this->rect().height()));
            _panimation->setEasingCurve(QEasingCurve::OutCubic);
            _panimation->start();
        }
    }
     _moved=false;
}

void XPannel::_scrool_lancer(int dx ,int dy)
{
    Q_UNUSED(dy);
    if(CFG(is_portret()))
    {
        QPoint pt = this->pos();
        int newx = pt.x()+dx;
        if(newx>0)
            newx=0;
        else if (newx < -(this->width() - CFG(_drect.width())))
            newx=-(this->width() - CFG(_drect.width()));
        move(newx, CFG(_drect.height())-CFG(_lanch_pixels));
       _moved=true;
    }
    else
    {
        QPoint pt = this->pos();
        int newy = pt.y()+dy;
        if(newy>0)
            newy=0;
        else if (newy < -(this->height() - CFG(_drect.height())))
            newy=-(this->height() - CFG(_drect.height()));
        move(0, newy);
       _moved=true;

    }
}


void XPannel::slot_align_edge()
{
    //int   animx = this->pos().x();
}



void XPannel::_calc_overflow()
{
    if(CFG(is_portret()))
    {
        int wd = CFG(_drect.width());
        _overflow=0;
        if(_layerwidth > wd)
        {
            int pos = this->pos().x();
            if(pos < 0)
                _overflow|=0x1;
            if(pos + this->rect().width() > wd)
                _overflow|=0x2;
        }
    }
    else
    {
        int wd = CFG(_drect.height());
        _overflow=0;
        if(_layerwidth > wd)
        {
            int pos = this->pos().y();
            if(pos < 0)
                _overflow|=0x1;
            if(pos + this->rect().height() > wd)
                _overflow|=0x2;
        }
    }
}

void XPannel::keyPressEvent(QKeyEvent *event)
{
    //process_key(event->key());
}


void XPannel::process_key(int key)
{
    switch(key)
    {
        case Qt::Key_Right:
            _selected++;
            break;
        case Qt::Key_Left:
            _selected--;
            break;
        case Qt::Key_Home:
            _selected=0;
            break;
        case Qt::Key_End:
            _selected=_butons.size()-1;
            break;
        case Qt::Key_Back:
        case Qt::Key_Escape:
            _selected=-1;
            break;
        case Qt::Key_Return:
        {
            LunchButt* pb = _butons.back();
            if(pb->xset()._name=="Back...")
            {
                pb->on_click();
            }
        }
            break;
        case Qt::Key_Space:
            _butons[_selected]->on_click();
            break;
        default:
            break;
    }
    if((size_t)_selected>=_butons.size())
        _selected=_butons.size()-1;
    else if((int)_selected<0)
        _selected=0;
    _update_btnsel();
}


void XPannel::_update_btnsel()
{
    std::vector<LunchButt*>::iterator it = _butons.begin();
    for(;it!= _butons.end();++it)
    {
        (*it)->deactivate();
    }
    if(_selected==-1)
        return;
    LunchButt* pb = _butons[_selected];

    int bx = pb->pos().x();
    int tx = this->pos().x();
    int onleft = tx+bx;
    _panimation->setStartValue(QRect(tx,this->pos().y(),this->rect().width(),this->rect().height()));
    _panimation->setDuration(500);
    _panimation->setEasingCurve(QEasingCurve::OutCubic);
    if(onleft<0)
    {
        int xe = -onleft;
        _panimation->setEndValue(QRect(xe,this->pos().y(),this->rect().width(),this->rect().height()));
        return;
    }

    int onright = tx+bx;
    if(onright > CFG(_drect).width())
    {
        onright = CFG(_drect).width()-(onright+pb->rect().width());
        int xe = -(tx-onright);
        _panimation->setEndValue(QRect(xe,this->pos().y(),this->rect().width(),this->rect().height()));

    }
    _panimation->start();
}

void   XPannel::refresh_buts()
{
    std::vector<LunchButt*>::iterator b = _butons.begin();
    for(;b!= _butons.end();++b)
        (*b)->refresh();
}

void     XPannel::ahide(bool hide)
{

    if(hide )
    {
        if(!_hidden)
        {
            _panimation->setEasingCurve(QEasingCurve::OutCubic);
            _panimation->setDuration(600);
            QRect sp = QRect(QRect(this->pos().x(),this->pos().y(),
                                   this->rect().width(),CFG(_lanch_pixels)));
            _panimation->setStartValue(sp);
            sp.setTop(this->pos().y()+CFG(_lanch_pixels));
            _panimation->setEndValue(sp);
            _hidden=true;
            _panimation->start();
        }
    }
    else if(_hidden)
    {
        _panimation->setEasingCurve(QEasingCurve::OutCubic);
        _panimation->setDuration(600);
        QRect sp = QRect(QRect(this->pos().x(),this->pos().y(),
                               this->rect().width(),CFG(_lanch_pixels)));
        _panimation->setStartValue(sp);
        sp.setTop(this->pos().y()-CFG(_lanch_pixels));
        _panimation->setEndValue(sp);
        _hidden=false;
        _panimation->start();
    }
}


void XPannel::slot_anim_done()
{
    if(_moved)
        _calc_overflow();
    else
        this->refresh_buts();
    _moved=false;

}
