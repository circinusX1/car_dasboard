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

#include "theapp.h"
#include "dbusipc.h"
#include "panel.h"

/*--------------------------------------------------------------------------------------
  ------------------------0-------------------------------------------------------------*/
DbusIpc::DbusIpc():_pserver(0),_padaptor(0)
{
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
bool DbusIpc::start()
{
    _pserver = new DashServer();
    _padaptor = new dashdbIfAdaptor(_pserver);

    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService("com.zirexix.carowa");
    ret = connection.registerObject("/", _pserver);
    return ret;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
DbusIpc::~DbusIpc()
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.unregisterService("com.zirexix.carowa");
    connection.unregisterObject("/");
    delete _pserver;
    delete _padaptor;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
DashServer::DashServer(QObject *parent):QObject(parent)
{
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void DashServer::notify(const QString &panel,const QVariantMap &customdata)
{
    qDebug() << panel << "\n";

    QStringList ql = panel.split(",");

    Panel* p = PA->get_panel(ql[0]);
    if(p)
    {
        p->update_widget(ql[1], customdata["text"].toString());
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void DashServer::clicked(const QString& s)
{
    emit user_click(s);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void DbusIpc::widget_clicked(const XwnSet& xset)
{
    _pserver->clicked(xset._pname);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
/*
void DashServer::user_click(const QString &eventkind)
{

}
*/
