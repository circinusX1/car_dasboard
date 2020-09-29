/***************************************************************************
                         add.h  -  description
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

#ifndef ADD_H
#define ADD_H

#include <qvariant.h>
#include <qdialog.h>


class QMenu;
class QLabel;
class QLineEdit;
class QToolButton;
class QKeyEvent;
class QFrame;
class QTreeWidget;


class Add : public QDialog
{
    Q_OBJECT

public:
    Add( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~Add();

    QMenu *menu;
    QLabel* textLabel1_2;
    QFrame* line1;
    QFrame* line1_2;
    QLabel* textLabel1;
    QLineEdit* Name_edit;
    QLineEdit* Freq_edit;
    QToolButton* Rem_button;
    QToolButton* Add_button;
    QToolButton* Save_button;
    QToolButton* Quit_button;
    QToolButton* Edit_button;
    QTreeWidget* Station_list;
    int Station_cnt;
    QLabel* Label_how_many;
    QLabel* textLabel1_3;

public slots:
    void Load_list();
    void Save_list();
    void Add_to_list();

  signals:
    void change_station(float);

private slots:

    void Remove_from_list();
    void Cancel();
    void Edit_to_list();
    void To_station();
    void List_edit();
    void Remove_all();

protected:

protected slots:
    virtual void languageChange();
    void keyPressEvent( QKeyEvent* );

};

#endif // ADD_H
