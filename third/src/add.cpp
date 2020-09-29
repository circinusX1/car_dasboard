/***************************************************************************
                         add.cpp  -  description
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

#include "add.h"
#include "home.h"

#include <qlabel.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qmenu.h>
#include <qevent.h>
#include <qframe.h>
#include <qheaderview.h>
#include <qtreewidget.h>
#include <qtextcodec.h>

#include <stdio.h>
#include <stdlib.h>

extern int conf_lower;
extern int conf_upper;

extern QString FontName[];
extern int     FontSize[];

/* 
 *  Constructs a Add as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Add::Add( QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl)

{
    this->setAttribute(Qt::WA_QuitOnClose, FALSE);
    QString name_home;

    QFont font(FontName[3], FontSize[3]);
    setFont(font);

    setMouseTracking(FALSE);


    menu = new QMenu( this );
    menu->addAction(tr("Remove all stations"),this,SLOT(Remove_all()));
    menu->addAction(tr("Save and Close"),this,SLOT(Save_list()));
    menu->addAction(tr("Remove selected"),this,SLOT(Remove_from_list()),Qt::Key_Delete);
    menu->setFont(font);
    // menu->setStyle(tr("Default"));

    textLabel1_2 = new QLabel( this );
    textLabel1_2->setGeometry( QRect( 10, 370, 115, 20 ) );

    line1 = new QFrame( this );
    line1->setGeometry( QRect( 0, 320, 351, 16 ) );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );

    line1_2 = new QFrame( this );
    line1_2->setGeometry( QRect( 0, 390, 351, 16 ) );
    line1_2->setFrameShape( QFrame::HLine );
    line1_2->setFrameShadow( QFrame::Sunken );
    line1_2->setFrameShape( QFrame::HLine );

    textLabel1 = new QLabel( this );
    textLabel1->setGeometry( QRect( 10, 340, 100, 20 ) );

    Name_edit = new QLineEdit( this );
    Name_edit->setGeometry( QRect( 120, 340, 210, 22 ) );

    Freq_edit = new QLineEdit( this );
    Freq_edit->setGeometry( QRect( 138, 370, 55, 22 ) );


    Rem_button = new QToolButton( this );
    Rem_button->setGeometry( QRect( 140, 300, 80, 20 ) );
    Rem_button->setPalette(QPalette(QColor(226,226,226)));

    Add_button = new QToolButton( this );
    Add_button->setGeometry( QRect( 200, 370, 65, 20 ) );
    Add_button->setPalette(QPalette(QColor(226,226,226)));

    Edit_button = new QToolButton( this );
    Edit_button->setGeometry( QRect( 270, 370, 65, 20 ) );
    Edit_button->setPalette(QPalette(QColor(226,226,226)));

    Save_button = new QToolButton( this );
    Save_button->setGeometry( QRect( 70, 410, 80, 20 ) );
    Save_button->setPalette(QPalette(QColor(226,226,226)));

    Quit_button = new QToolButton( this );
    Quit_button->setGeometry( QRect( 200, 410, 80, 20 ) );
    Quit_button->setPalette(QPalette(QColor(226,226,226)));


    Station_list = new QTreeWidget( this );
    Station_list->setColumnCount(2);
    QHeaderView *header = Station_list->header();
    header->setResizeMode(0, QHeaderView::Interactive);
    header->resizeSection(0, 170);
    header->setResizeMode(1, QHeaderView::Interactive);
    header->resizeSection(1, 120);
    header->setClickable( FALSE );
    Station_list->setGeometry( QRect( 30, 40, 290, 250 ) );
    Station_list->setAllColumnsShowFocus( TRUE );
    Station_list->setRootIsDecorated( FALSE );

    Station_cnt = 0;

    Label_how_many = new QLabel( this );
    Label_how_many->setGeometry( QRect( 40, 10, 40, 20 ) );

    textLabel1_3 = new QLabel( this );
    textLabel1_3->setGeometry( QRect( 80, 10, 170, 20 ) );

    languageChange();

    resize( QSize(350, 438).expandedTo(minimumSizeHint()) );


    connect( Add_button, SIGNAL( clicked() ), this, SLOT( Add_to_list() ) );
    connect( Rem_button, SIGNAL( clicked() ), this, SLOT( Remove_from_list() ) );
    connect( Save_button, SIGNAL( clicked() ), this, SLOT( Save_list() ) );
    connect( Quit_button, SIGNAL( clicked() ), this, SLOT( Cancel() ) );
    connect( Edit_button, SIGNAL( clicked() ), this, SLOT( Edit_to_list() ) );
    connect( Station_list, SIGNAL( itemClicked(QTreeWidgetItem*, int) ), this, SLOT( List_edit() ) );
    connect( Station_list, SIGNAL( itemDoubleClicked(QTreeWidgetItem*, int) ), this, SLOT( To_station() ) );

}



/*
 *  Destroys the object and frees any allocated resources
 */
Add::~Add()
{
    // no need to delete child widgets, Qt does it all for us
    // fprintf(stderr, "Add deleted\n");
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Add::languageChange()
{

    setWindowTitle( tr( "Editing stations" ) );
    textLabel1_2->setText( tr( "Frequency [MHz] :" ) );

    textLabel1->setText( tr( "Station Name :" ) );

    Rem_button->setText( tr( "Remove" ) );

    Add_button->setText( tr( "Add" ) );

    Edit_button->setText( tr( "Change" ) );

    Save_button->setText( tr( "Save" ) );

    Quit_button->setText( tr( "Cancel" ) );

    Rem_button->setToolTip( tr( "Remove selected station" ) );
    Rem_button->setWhatsThis( tr( "Remove selected station" ) );

    Add_button->setToolTip( tr( "Add new station" ) );
    Add_button->setWhatsThis( tr( "Add new station" ) );

    Edit_button->setToolTip( tr( "Edit selected station" ) );
    Edit_button->setWhatsThis( tr( "Edit selected station" ) );

    Save_button->setToolTip( tr( "Save list and close window" ) );
    Save_button->setWhatsThis( tr( "Save list and close window" ) );

    Quit_button->setToolTip( tr( "Close window without saving" ) );
    Quit_button->setWhatsThis( tr( "Close window without saving" ) );

    QStringList header;
    header << tr( "Station Name" ) <<  tr( "Frequency [MHz]" );
    Station_list->setHeaderLabels(header);
    Label_how_many->setText("0");

    textLabel1_3->setText( tr( "station(s) in the user's list:" ) );

/*
    textLabel1_2->setBackgroundOrigin(ParentOrigin);
    Label_how_many->setBackgroundOrigin(ParentOrigin);
    textLabel1_3->setBackgroundOrigin(ParentOrigin);
    textLabel1->setBackgroundOrigin(ParentOrigin);
    line1->setBackgroundOrigin(ParentOrigin);
    line1_2->setBackgroundOrigin(ParentOrigin);
 */
}


void Add::Remove_from_list()
{
    if ( ! Station_list->selectedItems().isEmpty() )
    {
        QTreeWidgetItem *item = Station_list->selectedItems().at(0);
        int i = Station_list->indexOfTopLevelItem(item);
        if (i != -1)
        {
            Station_list->takeTopLevelItem(i);
            Station_cnt--;
            Label_how_many->setText( QString::number( Station_cnt ) );
        }
    }
}

// load list of stations
void Add::Load_list()
{

    Station_list->clear();
    Station_cnt = 0;

    QString name_home = home_dir();
    name_home.append("/.qtradio/stations");
    QFile file(name_home);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream t(&file);
        while ( !t.atEnd() )
        {
            QString s = t.readLine();
            QString name;
            QString value;
            int m = s.indexOf('=');
            if (m<0)
            {
                value = s;
                name = tr("Unknown");
            }
            else
            {
                value = s.left(m);
                name = s.mid(m+1);
            }

            // fprintf(stderr, "name = %s, value = %s\n", name.ascii(), value.ascii());
            bool good;
            float f = value.toFloat(&good);
            if (good && (f>=conf_lower) && (f<=conf_upper))
            {
                value.setNum(f, 'f', 2);
                value.prepend("     ");
                value = value.right(6);
                QTreeWidgetItem *item = new QTreeWidgetItem(Station_list);
                item->setText(0, name);
                item->setText(1, value);
                Station_cnt++;
            }
        }
    }

    Station_list->sortItems(1,Qt::AscendingOrder);

    Name_edit->setText("");
    Freq_edit->setText("");

    Label_how_many->setText( QString::number( Station_cnt ) );
}

void Add::Save_list()
{

    QTreeWidgetItem *item;
    QString name_prog, name_freq;

    name_prog = home_dir();
    name_prog.append("/.qtradio/stations");
    QFile wyj(name_prog);
    if ( ! wyj.open(QIODevice::WriteOnly) )
    {
        fprintf(stderr, "Can't open %s for writing!\n", name_prog.toLocal8Bit().data());
        return;
    }

    QTextStream str(&wyj);
    str.setCodec(QTextCodec::codecForLocale());

    item = Station_list->topLevelItem(0);

    while ( item != NULL )
    {
        str << item->text(1) << "=" << item->text(0) << endl;
        item = Station_list->itemBelow(item);
    }
    Add::hide();

}

extern int conf_lower;
extern int conf_upper;

void Add::Add_to_list()
{
    QTreeWidgetItem *item;

    if ( (Name_edit->text() != "") && (Freq_edit->text() != "") )
    {
        bool good;
        float f = Freq_edit->text().toFloat(&good);
        if ((!good) || (f < conf_lower) || (f> conf_upper)) return;

        QString s;
        s.setNum(f,'f',2);
        s.prepend("      ");
        s = s.right(6);

        item = new QTreeWidgetItem(Station_list);
        item->setText(0,Name_edit->text());
        item->setText(1,s);
        Station_list->sortItems(1, Qt::AscendingOrder);
        Station_cnt++;
        Label_how_many->setText( QString::number( Station_cnt ) );

        Name_edit->setText("");
        Freq_edit->setText("");
    }
}

void Add::Edit_to_list()
{
    if   ( Station_cnt && (! Station_list->selectedItems().isEmpty() ) )
    {
        bool good;
        float f = Freq_edit->text().toFloat(&good);
        if ((!good) || (f < conf_lower) || (f> conf_upper)) return;

        QString s;
        QTreeWidgetItem *item = Station_list->selectedItems().at(0);
        s.setNum(f,'f',2);
        s.prepend("      ");
        s = s.right(6);
        item->setText(0,Name_edit->text());
        item->setText(1,s);
        Station_list->sortItems(1, Qt::AscendingOrder);
    }
}

void Add::To_station()
{
    if   ( Station_cnt && (! Station_list->selectedItems().isEmpty() ) )
    {
        bool good;
        float f = Freq_edit->text().toFloat(&good);
        if ((!good) || (f < conf_lower) || (f> conf_upper)) return;

        emit change_station(f);
    }
}

void Add::List_edit()
{
    if ( ! Station_list->selectedItems().isEmpty() )
    {
        QTreeWidgetItem *item = Station_list->selectedItems().at(0);
        Name_edit->setText( item->text(0) );
        Freq_edit->setText( item->text(1) );
    }
}

void Add::Cancel()
{
    QList<QTreeWidgetItem*> list = Station_list->selectedItems();

    if ( ! list.isEmpty() )
    {
        QList<QTreeWidgetItem*>::iterator it;
        for (it = list.begin(); it != list.end(); it++)
            (*it)->setSelected(FALSE);
    }
    Name_edit->setText("");
    Freq_edit->setText("");
    Add::hide();
}

void Add::Remove_all()
{
    Station_list->clear();
    Station_cnt = 0;
    Label_how_many->setText("0");
}

/*
 *  process key events
 */
void Add::keyPressEvent( QKeyEvent *k )
{
    if (k->key() == Qt::Key_Escape)
        Add::Cancel();
}

