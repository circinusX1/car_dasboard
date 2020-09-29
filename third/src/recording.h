/***************************************************************************
                          recording.h  -  description
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

#ifndef RECORDING_H
#define RECORDING_H

#include <qvariant.h>
#include <qdialog.h>
#include <qevent.h>

class QComboBox;
class QLabel;
class QLineEdit;
class QToolButton;
class QFrame;
class QProgressBar;

class Recording : public QDialog
{
    Q_OBJECT

public:
    Recording( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~Recording();

    QLabel* textLabel1;
    QComboBox* comboBox2;
    QLabel* textLabel2;
    QComboBox* comboBox3;
    QLabel* textLabel3;
    QLabel* textLabel4;
    QComboBox* comboBox1;
    QLabel* textLabel8;
    QFrame* line1_2;
    QLabel* textLabel7;
    QFrame* line1;
    QLabel* textLabel9;
    QLabel* textLabel6;
    QComboBox* comboBox5;
    QLabel* textLabel5;
    QLabel* textLabel1_2;
    QLineEdit* lineEdit1;
    QComboBox* comboBox6;
    QToolButton* toolButton2;
    QFrame* line1_3;
    QProgressBar* progressBar1;
    QLabel* textLabel2_2;
    QLabel* textLabel10;
    QLabel* textLabel11;
    QToolButton* toolButton1;
    QToolButton* toolButton3;
    QComboBox* comboBox4;
    QLineEdit* lineEdit2;

protected:

protected slots:
    virtual void languageChange();
    void Start_recording();
    void Stop_recording();
    void Quit_recording();
    void Status();
    void keyPressEvent( QKeyEvent* );
};

#endif // RECORDING_H

