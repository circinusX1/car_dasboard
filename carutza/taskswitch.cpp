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

#include "taskswitch.h"
#include "theapp.h"


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
XcliSwitchIcon::XcliSwitchIcon(XClient *client, int iscale, QWidget *parent)
    : WidgetIcon(client->_app_name, client->_app_imagez, iscale, parent)
{
    m_Client = client;

}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
XcliSwitchIcon::~XcliSwitchIcon()
{
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
TaskSwitch::TaskSwitch(XClient *active, ClientList *_x_wins, QWidget *parent)
    : QDialog(parent)
{

    int h = 0;
    int w = TheApp::__Dsk()->width();
    int numIcons = _x_wins->size();
    int s = w / (numIcons * 2);

    int iconSize = 0;
    if (s >= 128)
    {
        iconSize = 128;
    }
    else if (s >= 64 && s < 128)
    {
        iconSize = 64;
    }
    else if (s >= 48 && s < 64)
    {
        iconSize =48;
    }
    else if (s >= 40 && s < 48)
    {
        iconSize = 40;
    }
    else if (s < 40)
    {
        iconSize =32;
    }

    //qDebug() << "ICON SIZE=" << iconSize;

    layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 12, 0, 12);

    layout->addStretch();
    // add current active client first
    if (active != NULL)
    {
        XcliSwitchIcon *first = new XcliSwitchIcon(active, iconSize);
        icons << first;
        h = first->height();
        layout->addWidget(first);
    }

    XClient *c;

    foreach (c, *_x_wins)
    {
        if (active != NULL && active == c) continue;
        XcliSwitchIcon *icon = new XcliSwitchIcon(c, iconSize);
        icons << icon;
        h = icon->height();
        layout->addWidget(icon);
    }

    XcliSwitchIcon *icon = icons.at(0);
    icon->paint_border(true);
    current = 0;

    layout->addStretch();

    setLayout(layout);
    resize(w, h+24);
    move(0, TheApp::__Dsk()->height()/2 - height() / 2);

    XUngrabKeyboard(DPY(), CurrentTime);
    XSetInputFocus(DPY(), winId(), X11_None, CurrentTime);
    XRaiseWindow(DPY(), winId());
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
TaskSwitch::~TaskSwitch()
{
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
XClient *TaskSwitch::selectedIcon()
{
    return icons.at(current)->m_Client;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void TaskSwitch::reject()
{
    PA->_wm_wins_state = TheApp::PS_NORMAL;
    QDialog::reject();
}


/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void TaskSwitch::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Tab)
    {
        icons.at(current)->paint_border(false);
        current ++;
        if (current >= icons.size()) current = 0;
        icons.at(current)->paint_border(true);
        e->accept();
    }
    else
        QDialog::keyPressEvent(e);
}
