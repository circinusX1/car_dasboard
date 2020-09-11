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
#include "dashdb_cli.h"

/*
 * Implementation of interface class dashdb_cli
 */

dashdb_cli::dashdb_cli(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

dashdb_cli::~dashdb_cli()
{
}

