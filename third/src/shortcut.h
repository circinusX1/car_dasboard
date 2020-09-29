/***************************************************************************
                          recording.cpp  -  description
                             -------------------
    begin                : Sat Jun 21 2003
    copyright            : (C) 2003 by Pawel Pustelnik
                         : (C) 2013 by Oleh Nykyforchyn
    email                : oleh.nyk@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <qvariant.h>
#include <qdialog.h>
#include <qevent.h>

class QLabel;


class Shortcut : public QDialog
{
    Q_OBJECT

public:
    Shortcut( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~Shortcut();

    QLabel* textLabel2;

    QLabel* textLabel1;

protected:

protected slots:
    virtual void languageChange();
    void Change_shortcut();
    void keyPressEvent( QKeyEvent *k );
};

#endif // SHORTCUT_H
