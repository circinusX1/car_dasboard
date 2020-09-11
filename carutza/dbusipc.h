/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
--------------------------------------------------------------------------------
 This file is part of CARUTZA dashboard

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

#ifndef DBUSIPC_H
#define DBUSIPC_H


#include <QIcon>
#include <QtCore/QObject>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusInterface>
#include "dashdb_iface.h"

struct XwnSet;
class DashServer : public QObject
{
Q_OBJECT
public:
    explicit DashServer(QObject *parent = 0);

    void clicked(const QString& s);
public Q_SLOTS:
    void notify(const QString &panel, const QVariantMap &customdata);

Q_SIGNALS:
    void user_click(const QString &eventkind);
};



class DbusIpc
{
public:
    DbusIpc();
    virtual ~DbusIpc();
    bool start();
    void widget_clicked(const XwnSet& xset);
private:
    dashdbIfAdaptor* _padaptor;
    DashServer*     _pserver;
};

#endif // DBUSIPC_H
