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

#include "dskwatch.h"
#include "imagez.h"
#include "panel.h"

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
DskWatch::DskWatch(Panel* pmd)
{
    pmd->connect(&_watcher, SIGNAL(directoryChanged(QString)), pmd, SLOT(slot_floder_ch(QString)));
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void DskWatch::watch(const QString& path)
{
    if(_path.length())
        _watcher.removePath(_path);
    _path=path;
    _watcher.addPath(path);

}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
DskWatch::~DskWatch()
{
    _watcher.removePath(_path);
}




