/***************************************************************************
                         info.h  -  description
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <qvariant.h>
#include <qdialog.h>

class QWidget;
class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTabWidget;
class QToolButton;
class QKeyEvent;
class QGroupBox;
class QTreeWidget;

class Settings : public QDialog
{
    Q_OBJECT

public:
    Settings( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~Settings();

    QWidget* tab;
    QToolButton* toolButtonSave;
    QToolButton* toolButtonQuit;
    QTabWidget* tabWidget2;
    QGroupBox* groupBox1;
    QLabel* textLabel1;
    QLabel* textLabel3;
    QLabel* textLabel5;
    QLabel* textLabel6;
    QLabel* textLabel4;
    QLabel* textLabel2;
    QLabel* textLabel7;
    QGroupBox* groupBox2;
    QLabel* textLabel10;
    QLabel* textLabel11;
    QSpinBox* spinBox1;
    QSpinBox* spinBox2;
    QLabel* textLabel13;
    QLabel* textLabel9;
    QLabel* textLabel12;
    QComboBox* comboBox13;
    QGroupBox* groupBox3;
    QLabel* textLabel14;
    QLabel* textLabel15;
    QComboBox* comboBox7;
    QComboBox* comboBox12;
    QWidget* tab_2;
    QGroupBox* groupBox5;
    QLabel* textLabel16;
    QLabel* textLabel17;
    QComboBox* comboBox9;
    QComboBox* comboBox8;
    QGroupBox* groupBox7;
    QTreeWidget* listView1;
    QGroupBox* groupBox6;
    QLabel* textLabel19;
    QLabel* textLabel18;
    QComboBox* comboBox10;
    QComboBox* comboBox11;
    QLineEdit* lineEdit6;
    QLabel* textLabel21;
    QWidget* tab_3;
    QTreeWidget* listView2;
    QPushButton* textLabel20;

protected:

protected slots:
    virtual void languageChange();
    void Save();
    void Quit();
    void New_shortcut();
    void New_skin();
    void keyPressEvent( QKeyEvent* );
};


#endif // SETTINGS_H
