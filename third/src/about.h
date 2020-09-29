/***************************************************************************
                         about.h  -  description
                             -------------------
    begin                : czw lut 20 18:50:23 CET 2003
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

#ifndef ABOUT_H
#define ABOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QTabWidget;
class QLabel;
class QToolButton;
class QWidget;
class QTextEdit;

class About : public QDialog
{
    Q_OBJECT

public:
    About( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~About();

    QToolButton* toolButton6;
    QTabWidget* tabWidget3;
    QWidget* tab_1;
    QLabel* label1;
    QWidget* tab_2;
    QLabel* label2;
    QWidget* tab_3;
    QTextEdit* textEdit3;

protected:

protected slots:
    virtual void languageChange();
    void Quit();
};

#endif // ABOUT_H

