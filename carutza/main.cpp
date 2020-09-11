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

#include "defs.h"
#include "theapp.h"
#include "mysett.h"

extern TheApp *_PA;
bool __initialized = false;

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
  //  systyem("export DISPLAY=:0.1");
    TheApp a(argc, argv);

    // test if we have settings file
    QString env = getenv("HOME");env+="/";
    QString file=env; file.append(".carutza/config/carutza.conf");

    if(0!=::access(file.toUtf8(),0))
    {
        QString s("The folder: ");
        s.append(env);
        s.append("~/.carutza/config/carutza.conf does not exist. \n Program exits");
        QMessageBox msgBox;
        msgBox.setText(s);
        msgBox.exec();
        return -1;
    }

    MySett& s = MySett::config(&file);
    a.init(s);
	(void) QApplication::desktop();
    TheApp::_default_x11 = a.setEventFilter(TheApp::x11_event_filter);
    XSelectInput(DPY(), ROOT_XWIN(), KeyPressMask | KeyReleaseMask |
                 ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask |
                 PointerMotionMask | EnterWindowMask |
                 LeaveWindowMask | FocusChangeMask |
                 ExposureMask | StructureNotifyMask |
                 SubstructureRedirectMask | SubstructureNotifyMask);
	XClearWindow(DPY(), ROOT_XWIN());
	XSync(QX11Info::DPY(), FALSE);
    CFG(startapps());
    return a.exec();
}


