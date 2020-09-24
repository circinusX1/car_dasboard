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
#include "mysett.h"
#include "menu.h"

//-----------------------------------------------------------------------------
DskMenu::DskMenu(const QString &text, const QString &comment, QObject *parent)
                : QAction(text, parent)
{
    Q_UNUSED(comment);
}

//-----------------------------------------------------------------------------
DskMenu::DskMenu(const QIcon &icon, const QString &text, const QString &comment,
                 QObject *parent) : QAction(icon, text, parent)
{
	Q_UNUSED(comment);
}

//-----------------------------------------------------------------------------
void DskMenu::set_cmd(const QString &cmd, bool runAsRoot)
{
	Q_UNUSED(runAsRoot);
	_run_cmd = cmd;
	int dot = _run_cmd.indexOf("%");
	while (dot > 0) {
		_run_cmd[dot] = QChar(' ');
		if (dot < cmd.length() - 1)
			_run_cmd[dot+1] = QChar(' ');
		dot = _run_cmd.indexOf("%");
	}
	_run_cmd = _run_cmd.trimmed();
}

//-----------------------------------------------------------------------------
void DskMenu::run_cmd()
{
	//qDebug() << "run command " << _run_cmd;
	QProcess::startDetached(_run_cmd);
}

//-----------------------------------------------------------------------------
bool compareMnuAct(DskMenu *a1, DskMenu *a2)
{
	return a1->text() < a2->text();
}

//-----------------------------------------------------------------------------
MenuButton::MenuButton(CtrlHolder *p, const QPoint& size,
                       const XwnSet& set, QWidget *parent)
                        : OdButton(p, size, set, parent)
{
    setText("Apps...");
	_sc_catalog["Utility"] = new QIcon("util.png");
	_sc_catalog["Office"] = new QIcon("office.png");
	_sc_catalog["Network"] = new QIcon("networg.png");
	_sc_catalog["Graphics"] = new QIcon("graphics.png");
	_sc_catalog["AudioVideo"] = new QIcon("multimedia.png");
	_sc_catalog["Game"] = new QIcon("games.png");
	_sc_catalog["System"] = new QIcon("system.png");
	_sc_catalog["Development"] = new QIcon("devel.png");
	_sc_catalog["Other"] = new QIcon("others.png");
	create();
	setFixedSize(size.x(),size.y());
	connect(this, SIGNAL(clicked()), this, SLOT(slot_show_menu()));
}

//-----------------------------------------------------------------------------
MenuButton::~MenuButton()
{
}

//-----------------------------------------------------------------------------
struct Freader
{
    QMap <QString, QString> _map;

    Freader(const QString& filename)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return ;

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QStringList data = (in.readLine()).split("=");
            if (data.size() == 2)
            {
                _map.insert(data[0].trimmed(), data[1].trimmed());
            }
        }
        file.close();
    }
};

//-----------------------------------------------------------------------------
void MenuButton::create_sys(const QString &path)
{
	bool found;
	QDir dir;
	QString current = dir.currentPath();

    if (!dir.setCurrent(path))
		return;
	QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); i++)
    {
		QFileInfo fi = list.at(i);
        if (fi.isDir())
        {
            continue;
		}
        if (fi.fileName().contains(".desktop") || fi.fileName().contains(".desktopp"))
        {
            Freader r(path+fi.fileName());
			//
            QString title = tr(r._map["Name"].toLatin1());
            if (title.isEmpty()) continue;
            QString cat = r._map["Categories"];
            if(cat.isEmpty())cat="Other";

            QStringList scat = cat.split(';', QString::SkipEmptyParts);
            QString execCmd = r._map["Exec"];

            DskMenu *act = new DskMenu(title, "", this);
			act->set_cmd(execCmd);
			connect(act, SIGNAL(triggered(bool)), act, SLOT(run_cmd()));

			found = false;
            for (int c = 0; c < scat.size(); c++)
            {
                if (_sc_catalog.contains(scat[c]))
                {
                    _sc_menu_entry.insert(scat[c], act);
					found = true;
					break;
				}
			}
            if (!found)
            {
				_sc_menu_entry.insert("Other", act);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void MenuButton::create()
{
	QMenu *menu = new QMenu(this);

	// TODO: read path value from system settings
	create_sys("/usr/share/applications/");
	QList <QString> cat = _sc_menu_entry.uniqueKeys();
	for (int k = 0; k < cat.size(); k++) {
		QMenu *submenu = menu->addMenu(cat[k]);

		QList <DskMenu *> items = _sc_menu_entry.values(cat[k]);
		qSort(items.begin(), items.end(), compareMnuAct);

		for (int i = 0; i < items.size(); i++) {
			submenu->addAction(items[i]);
		}
	}

	setMenu(menu);
}
