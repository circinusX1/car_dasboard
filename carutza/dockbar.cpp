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
#include "dockbar.h"
#include "dockicon.h"
#include "theapp.h"

Dockbar::Dockbar(TheApp *a, QWidget *parent) : QLabel(parent)
{
    Q_UNUSED(a);
    PA->wm_add_ignorewid(this->winId());
    dockLayout = new QHBoxLayout();
    setLayout(dockLayout);
    dockLayout->setContentsMargins(0, 0, 0, 0);
    dockLayout->setSpacing(1);
    load_config();
    setPixmap(QPixmap::fromImage(QImage(dockPix)));
    setScaledContents(true);

    timer = new QTimer();
    hideTimer = new QTimer();

    dockState = Dockbar::DOCK_DEFAULT;
    set_autohide(false);

    // to store dockicons
    iconsList = new DockIconsList;

    // for dockicon on dockbar
    dockFrame = new QFrame();
    dockFrame->resize(QApplication::desktop()->width(), 100);//height()-1);
    iconLayout = new QHBoxLayout();
    dockFrame->setLayout(iconLayout);
    iconLayout->setAlignment(Qt::AlignLeft);
    iconLayout->setContentsMargins(0, 0, 0, 0);
    iconLayout->setSpacing(1);
    dockLayout->addWidget(dockFrame);
    set_size_dims(dockFactor, false);
}

Dockbar::~Dockbar()
{
}

void Dockbar::load_config()
{
    MySett& set = MySett::config();

    dockPix    = set.read_mangled("dock_pix");
    dockFactor = set.value("dock_factor").toInt();
    autoHide   = set.value("dock_autohide", false).toBool();
}

void Dockbar::add_xcli(XClient *client)
{
    DockIcon *dockIcon = new DockIcon(client);
    iconsList->append(dockIcon);
    refresh();
    iconLayout->addWidget(dockIcon);
    connect(dockIcon, SIGNAL(sig_show_icon(DockIcon *)), SLOT(slot_remove_icon(DockIcon *))); // show iconize dockicon and update dockbar size
    connect(dockIcon, SIGNAL(sig_destroy_icon(DockIcon *)), SLOT(slot_remove_icon(DockIcon *))); // show iconize dockicon and update dockbar size
}

void Dockbar::slot_remove_icon(DockIcon *d)
{
    iconsList->removeOne(d);
    PA->wm_dock_icon_removed(d->get_x_cli());
    refresh();
}

bool Dockbar::remove_xcli(XClient *client)
{
    DockIcon *d;
    foreach(d, *iconsList)
    {
        if (d->get_x_cli() == client)
        {
            iconsList->removeOne(d);
            d->close();
            refresh();
            break;
        }
    }
    return true;
}

bool Dockbar::remove_childs()
{
    DockIcon *d;
    foreach(d, *iconsList)
    {
        d->get_x_cli()->mapFast();
        iconsList->removeOne(d);
        d->close();
        refresh();
    }
    return true;
}

void Dockbar::set_autohide(bool active)
{
    autoHide = active;

    if (autoHide)
    {
        connect(hideTimer, SIGNAL(timeout()), this, SLOT(slot_onoff_dock()));
        hideTimer->start(80);
    }
    else
    {
        hideTimer->stop();
        disconnect(hideTimer, SIGNAL(timeout()), 0, 0);
        slot_animate_show();
    }
}

// checkCursor : checks if the cursor is within dockbar area
// returns: true if yes, false if not
bool Dockbar::checkCursor()
{
    int dx = QCursor::pos().x();
    int dy = QCursor::pos().y();
    bool f1 = true;
    bool f2 = true;

    int w = TheApp::__Dsk()->width();

    if (dockState == Dockbar::DOCK_DEFAULT || dockState == Dockbar::DOCK_PROG_ON)
    {
        if (dx < x() || dx > x() + width())
            f1 = false;
        if (dy <= y() - 4)
            f2 = false;
    }
    else if (dockState == Dockbar::DOCK_HIDDEN || dockState == Dockbar::DOCK_PROG_OFF)
    {
        if (dx < w / 3 || dx > w * 2 / 3)
            f1 = false;
        if (dy < TheApp::__Dsk()->height() + PA->_tpanel_pixels->height() - 16)
            f2 = false;
    }
    //qDebug() << "CHECK CURSOR :" << (f1 && f2);
    return (f1 &&  f2);
}

void Dockbar::slot_onoff_dock()
{
    hideTimer->stop();
    hideTimer->start(2000);
    if (dockState == Dockbar::DOCK_DEFAULT && !checkCursor())
    {
        slot_animate();
    }
    else if (dockState == Dockbar::DOCK_HIDDEN && checkCursor())
        slot_animate_show();
}

void Dockbar::slot_animate()
{
    if (checkCursor())
    {
        // cursor is within dockbar area - cancel hiding
        return;
    }
    if (dockState == Dockbar::DOCK_DEFAULT)
    {
        connect(timer, SIGNAL(timeout()), this, SLOT(slot_hide_1()));
        dockState = Dockbar::DOCK_PROG_OFF;
        timer->start(10);
    }
}

void Dockbar::slot_hide_1()
{
    if (checkCursor())
    {
        // cursor is within dockbar area - cancel hiding
        timer->stop();
        disconnect(timer, SIGNAL(timeout()), 0, 0);
        dockState = Dockbar::DOCK_HIDDEN;
        slot_animate_show();
        return;
    }

    move(x(), y() + 1);
    if (y() == TheApp::__Dsk()->height() + PA->_tpanel_pixels->height())
    {
        timer->stop();
        disconnect(timer, SIGNAL(timeout()), 0, 0);
        dockState = Dockbar::DOCK_HIDDEN;
    }
}

void Dockbar::slot_animate_show()
{
    if (dockState == Dockbar::DOCK_HIDDEN)
    {
        connect(timer, SIGNAL(timeout()), this, SLOT(slot_hide_2()));
        dockState = Dockbar::DOCK_PROG_ON;
        timer->start(10);
    }
}

void Dockbar::slot_hide_2()
{
    move(x(), y() - 1);
    if (y() == TheApp::__Dsk()->height() + PA->_tpanel_pixels->height() - height())
    {
        timer->stop();
        disconnect(timer, SIGNAL(timeout()), 0, 0);
        dockState = Dockbar::DOCK_DEFAULT;
    }
}

int Dockbar::instant_height()
{
    if (dockState == Dockbar::DOCK_DEFAULT)
        return height();
    else
        return 0;
}

//
//  dockbar::set_size_dims
//  @param - factor (0,100) precent of _get_screen width
//
void Dockbar::set_size_dims(int factor, bool doSave)
{
    //qDebug() << "RESIZING DOCK " << factor;
    int maxW = QApplication::desktop()->width();
    int w = (maxW * factor) / 100;
    int h = MAX_DOCK_HEIGHT * factor/100;
    if (h < 24) h = 24;
    //qDebug() << "DESIRED SIZE " << w << h;
    dockFrame->resize(w, h);
    resize(w, h);
    move((maxW - w)/2, QApplication::desktop()->height()-h);
    //qDebug() << "ACTUAL SIZE " << width() << height();
    refresh();
    dockFactor = factor;
    if (doSave)
    {
        autoHide = false;
        dockState = Dockbar::DOCK_DEFAULT;
    }
}

void Dockbar::refresh(void)
{
    int dockLength;
    DockIcon *d;

    if (! iconsList->isEmpty())
    {
        int num = iconsList->size();
        qDebug() << "dockicon num:" << num;
        dockLength = dockFrame->width()/num;

        if (dockLength >= dockFrame->width()/4) // max dockicon size = d_frame size/4
            dockLength = dockFrame->width()/4;

        qDebug() << "dockicon length:" << dockLength;

        for (int i = 0; i <num; i++)
        {
            d = iconsList->at(i);
            d->setFixedSize(dockLength, height()-5);
            iconLayout->addWidget(d);
        }
    }
}

