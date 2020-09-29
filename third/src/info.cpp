/***************************************************************************
                         info.cpp  -  description
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

#include "home.h"
#include "info.h"
#include "shortcut.h"
#include "radio_functions.h"

#include <stdio.h>

#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qfont.h>
#include <qmessagebox.h>
#include <qevent.h>
#include <qgroupbox.h>

#include <qheaderview.h>
#include <qtreewidget.h>

/* in qtradio.cpp */
extern QString data_config[16];
extern QString names[20];
extern QString kseq[20];
extern QString SkinDir;

extern QString FontName[];
extern int     FontSize[];

extern int conf_lower;
extern int conf_upper;
extern float conf_dok;
extern int conf_thr;
extern int conf_chan;
extern bool conf_dock;

extern int show_settings;

static Shortcut *sh = NULL;

/*
 *  Constructs a Settings as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Settings::Settings( QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl )

{
    this->setAttribute(Qt::WA_QuitOnClose, FALSE);

    QFont font(FontName[1], FontSize[1]);
    setFont(font);

    toolButtonSave = new QToolButton( this );
    toolButtonSave->setGeometry( QRect( 120, 420, 80, 24 ) );
    toolButtonSave->setPalette(QPalette(QColor(226,226,226)));

    toolButtonQuit = new QToolButton( this );
    toolButtonQuit->setGeometry( QRect( 230, 420, 90, 24 ) );
    toolButtonQuit->setPalette(QPalette(QColor(226,226,226)));


    tabWidget2 = new QTabWidget( this );
    tabWidget2->setGeometry( QRect( 10, 10, 430, 400 ) );


    tab = new QWidget( tabWidget2 );

    groupBox1 = new QGroupBox( tab );
    groupBox1->setGeometry( QRect( 10, 10, 410, 90 ) );
    groupBox1->setAlignment( int( Qt::AlignHCenter ) );
    groupBox1->setFlat( FALSE );

    textLabel1 = new QLabel( groupBox1 );
    textLabel1->setGeometry( QRect( 10, 20, 100, 20 ) );

    textLabel2 = new QLabel( groupBox1 );
    textLabel2->setGeometry( QRect( 130, 20, 100, 20 ) );

    textLabel3 = new QLabel( groupBox1 );
    textLabel3->setGeometry( QRect( 10, 40, 100, 20 ) );

    textLabel4 = new QLabel( groupBox1 );
    textLabel4->setGeometry( QRect( 130, 40, 61, 20 ) );

    textLabel5 = new QLabel( groupBox1 );
    textLabel5->setGeometry( QRect( 10, 60, 100, 20 ) );

    textLabel6 = new QLabel( groupBox1 );
    textLabel6->setGeometry( QRect( 130, 60, 61, 20 ) );

    groupBox2 = new QGroupBox( tab );
    groupBox2->setGeometry( QRect( 10, 110, 410, 140 ) );
    groupBox2->setAlignment( int( Qt::AlignHCenter ) );

    textLabel9 = new QLabel( groupBox2 );
    textLabel9->setGeometry( QRect( 10, 20, 120, 20 ) );

    comboBox13 = new QComboBox( groupBox2 );
    comboBox13->setGeometry( QRect( 150, 20, 130, 20 ) );

    textLabel7 = new QLabel( groupBox2 );
    textLabel7->setGeometry( QRect( 10, 50, 120, 20 ) );

    comboBox11 = new QComboBox( groupBox2 );
    comboBox11->setGeometry( QRect( 150, 50, 130, 20 ) );

    textLabel10 = new QLabel( groupBox2 );
    textLabel10->setGeometry( QRect( 10, 80, 140, 20 ) );

    spinBox1 = new QSpinBox( groupBox2 );
    spinBox1->setGeometry( QRect( 160, 80, 55, 20 ) );
    spinBox1->setMaximum( 108 );
    spinBox1->setMinimum( 65 );
    spinBox1->setValue( 87 );

    textLabel12 = new QLabel( groupBox2 );
    textLabel12->setGeometry( QRect( 220, 80, 68, 20 ) );

    textLabel11 = new QLabel( groupBox2 );
    textLabel11->setGeometry( QRect( 10, 110, 140, 20 ) );

    spinBox2 = new QSpinBox( groupBox2 );
    spinBox2->setGeometry( QRect( 160, 110, 55, 20 ) );
    spinBox2->setMaximum( 108 );
    spinBox2->setMinimum( 65 );
    spinBox2->setValue( 108 );

    textLabel13 = new QLabel( groupBox2 );
    textLabel13->setGeometry( QRect( 220, 110, 68, 20 ) );

    groupBox3 = new QGroupBox( tab );
    groupBox3->setGeometry( QRect( 10, 260, 410, 90 ) );
    groupBox3->setAlignment( int( Qt::AlignHCenter ) );

    textLabel14 = new QLabel( groupBox3 );
    textLabel14->setGeometry( QRect( 10, 20, 130, 20 ) );

    textLabel15 = new QLabel( groupBox3 );
    textLabel15->setGeometry( QRect( 10, 50, 130, 20 ) );

    comboBox7 = new QComboBox( groupBox3 );
    comboBox7->setGeometry( QRect( 150, 50, 130, 20 ) );

    comboBox12 = new QComboBox( groupBox3 );
    comboBox12->setGeometry( QRect( 150, 20, 130, 20 ) );

    tabWidget2->insertTab( 0, tab, "" );

    tab_2 = new QWidget( tabWidget2 );

    groupBox5 = new QGroupBox( tab_2 );
    groupBox5->setGeometry( QRect( 10, 10, 410, 100 ) );
    groupBox5->setAlignment( int( Qt::AlignHCenter ) );

    textLabel16 = new QLabel( groupBox5 );
    textLabel16->setGeometry( QRect( 10, 30, 140, 20 ) );

    textLabel17 = new QLabel( groupBox5 );
    textLabel17->setGeometry( QRect( 10, 60, 140, 20 ) );

    comboBox8 = new QComboBox( groupBox5 );
    comboBox8->setGeometry( QRect( 190, 30, 100, 20 ) );

    comboBox9 = new QComboBox( groupBox5 );
    comboBox9->setGeometry( QRect( 190, 60, 100, 20 ) );

    groupBox6 = new QGroupBox( tab_2 );
    groupBox6->setGeometry( QRect( 10, 120, 410, 100 ) );
    groupBox6->setAlignment( int( Qt::AlignHCenter ) );

    textLabel19 = new QLabel( groupBox6 );
    textLabel19->setGeometry( QRect( 10, 60, 160, 20 ) );

    textLabel18 = new QLabel( groupBox6 );
    textLabel18->setGeometry( QRect( 10, 30, 160, 20 ) );

    comboBox10 = new QComboBox( groupBox6 );
    comboBox10->setGeometry( QRect( 190, 60, 60, 20 ) );

    lineEdit6 = new QLineEdit( groupBox6 );
    lineEdit6->setGeometry( QRect( 190, 30, 60, 20 ) );

    textLabel21 = new QLabel( groupBox6 );
    textLabel21->setGeometry( QRect( 250, 30, 40, 20 ) );
    tabWidget2->insertTab( 1, tab_2, "" );

    groupBox7 = new QGroupBox( tab_2 );
    groupBox7->setGeometry( QRect( 10, 230, 410, 130 ) );
    groupBox7->setAlignment( int( Qt::AlignHCenter ) );

    listView1 = new QTreeWidget( groupBox7 );
    listView1->setColumnCount(2);
    QHeaderView *header = listView1->header();
    header->setResizeMode(0, QHeaderView::Interactive);
    header->resizeSection(0, 130 );
    header->setResizeMode(1, QHeaderView::Interactive);
    header->resizeSection(1, 260 );
    header->setClickable( FALSE );
    listView1->setGeometry( QRect( 10, 20, 390, 100 ) );
    listView1->setRootIsDecorated( FALSE );
    listView1->setAllColumnsShowFocus( TRUE );


    tab_3 = new QWidget( tabWidget2 );

    listView2 = new QTreeWidget( tab_3 );
    listView2->setColumnCount(2);
    header = listView2->header();
    header->setResizeMode(0, QHeaderView::Interactive);
    header->resizeSection(0, 250 );
    header->setResizeMode(1, QHeaderView::Interactive);
    header->resizeSection(1, 140 );
    header->setClickable( FALSE );
    listView2->setGeometry( QRect( 10, 20, 410, 300 ) );
    listView2->setRootIsDecorated( FALSE );
    listView2->setAllColumnsShowFocus( TRUE );

    textLabel20 = new QPushButton( tab_3 );
    textLabel20->setGeometry( QRect( 135, 340, 150, 25 ) );
    tabWidget2->insertTab( 2, tab_3, "" );
    languageChange();
    resize( QSize(455, 449).expandedTo(minimumSizeHint()) );

    connect( textLabel20, SIGNAL( clicked() ), this, SLOT( New_shortcut() ) );
    connect( toolButtonSave, SIGNAL( clicked() ), this, SLOT( Save() ) );
    connect( toolButtonQuit, SIGNAL( clicked() ), this, SLOT( Quit() ) );
    connect( listView1, SIGNAL( itemDoubleClicked(QTreeWidgetItem*, int) ), this, SLOT( New_skin() ) );
    connect( listView2, SIGNAL( itemClicked(QTreeWidgetItem*, int) ), this, SLOT( New_shortcut() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Settings::~Settings()
{
    // no need to delete child widgets, Qt does it all for us
    if (sh) delete sh; sh = NULL;
    // fprintf(stderr, "Settings deleted\n");
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Settings::languageChange()
{
    setWindowTitle( tr( "Settings for QtRadio" ) );
    toolButtonSave->setText( tr( "Save" ) );
    toolButtonQuit->setText( tr( "Exit" ) );
    groupBox1->setTitle( tr( "Video Driver" ) );
    textLabel1->setText( tr( "Driver:" ) );
    textLabel2->setText( tr( "driver name" ) );
    textLabel3->setText( tr( "Stereo:" ) );
    textLabel4->setText( tr( "yes/no" ) );
    textLabel5->setText( tr( "RDS:" ) );
    textLabel6->setText( tr( "yes/no" ) );
    textLabel7->setText( tr( "Mode:" ) );
    groupBox2->setTitle( tr( "Radio" ) );
    textLabel10->setText( tr( "Min. Frequency:" ) );
    textLabel11->setText( tr( "Max. Frequency:" ) );
    textLabel13->setText( tr( "MHz" ) );
    textLabel9->setText( tr( "Device:" ) );
    textLabel12->setText( tr( "MHz" ) );
    groupBox3->setTitle( tr( "Mixer" ) );
    textLabel14->setText( tr( "Device:" ) );
    textLabel15->setText( tr( "Main Channel:" ) );
    tabWidget2->setTabText( 0, tr( "Devices" ) );
    groupBox5->setTitle( tr( "Main Window" ) );
    textLabel16->setText( tr( "Dock on Start:" ) );
    textLabel17->setText( tr( "Mixer on Start:" ) );
    comboBox8->clear();
    comboBox8->insertItem( 0, tr( "Yes" ) );
    comboBox8->insertItem( 1, tr( "No" ) );
    comboBox9->clear();
    comboBox9->insertItem( 0, tr( "Yes" ) );
    comboBox9->insertItem( 1, tr( "No" ) );
    groupBox7->setTitle( tr( "Skin" ) );

    QStringList headers;
    headers << tr( "Skin name" ) << tr( "Path" );
    listView1->setHeaderLabels( headers );
    headers.clear();
    headers << tr( "Signal" ) << tr( "Key" );
    listView2->setHeaderLabels( headers );

    groupBox6->setTitle( tr( "Seeking" ) );
    textLabel19->setText( tr( "Min. Signal Strength:" ) );
    textLabel18->setText( tr( "Precision:" ) );
    comboBox10->clear();
    comboBox10->insertItem( 0, tr( "1" ) );
    comboBox10->insertItem( 1, tr( "2" ) );
    comboBox10->insertItem( 2, tr( "3" ) );
    comboBox10->insertItem( 3, tr( "4" ) );
    comboBox10->insertItem( 4, tr( "5" ) );
    comboBox10->insertItem( 5, tr( "6" ) );
    comboBox10->insertItem( 6, tr( "7" ) );
    comboBox10->insertItem( 7, tr( "8" ) );
    comboBox11->clear();
    comboBox11->insertItem( 0, tr( "Automatic" ) );
    comboBox11->insertItem( 1, tr( "Prefer stereo" ) );
    comboBox11->insertItem( 2, tr( "Prefer mono" ) );
    lineEdit6->setText("0.10");
    textLabel21->setText( tr( "MHz" ) );
    tabWidget2->setTabText( 1, tr( "Options" ) );
    textLabel20->setText( tr( "New Shortcut:" ) );
    tabWidget2->setTabText( 2, tr( "Keys" ) );

    toolButtonSave->setToolTip( tr( "Save settings and close this window" ) );
    toolButtonSave->setWhatsThis( tr( "Save settings and close this window" ) );

    toolButtonQuit->setToolTip( tr( "Close this window without saving" ) );
    toolButtonQuit->setWhatsThis( tr( "Close this window without saving" ) );

    textLabel20->setToolTip( tr( "Define new shortcut " ) );
    textLabel20->setWhatsThis( tr( "Define new shortcut " ) );

}

void Settings::keyPressEvent( QKeyEvent *k )
{
    if (k->key() == Qt::Key_Escape)
    {
        show_settings = 0;
        Settings::hide();
    }
}

void Settings::Save()
{
    QString name_home,tmp;
    char temp[150];
    char path[150];
    int i;
    float f;
    bool ok;

    //saving data from tuner and mixer section
    FILE *wyj;
    name_home = home_dir();
    name_home.append("/.qtradio/settings");
    strcpy(&path[0], name_home.toLocal8Bit().data());
    //printf("%s\n",&path[0]);
    wyj = fopen(&path[0],"w");

    tmp = comboBox13->currentText();
    if (tmp == tr("none"))
    {
        data_config[0] = "none";
        fprintf(wyj,"radio=none\n");
    }
    else
    if(tmp.left(5) == "/dev/")
    {
        data_config[0] = tmp;
        strcpy(&temp[0], tmp.toLocal8Bit().data());
        fprintf(wyj,"radio=%s\n",temp);
    }

    i = spinBox1->value();
    if ((i >= 87) && (i <= 108))
    {
        data_config[1].setNum(i);
        conf_lower = i;
        fprintf(wyj,"min_freq=%i\n",i);
    }

    i = spinBox2->value();
    if ((i >= 87) && (i <= 108))
    {
        if (i<conf_lower)
            i = conf_lower;
        data_config[2].setNum(i);
        conf_upper = i;
        fprintf(wyj,"max_freq=%i\n",i);
    }

    tmp = lineEdit6->text();
    f=tmp.toFloat(&ok);
    if (ok && (f >= 0.1))
    {
        i = f/0.1;
        f = 0.1*i;
        data_config[3].setNum(f);
        conf_dok = f;
        fprintf(wyj,"prec=%.2f\n",f);
    }

    tmp = comboBox10->currentText();
    i = 0;
    if      (tmp == tr("1")) i = 1;
    else if (tmp == tr("2")) i = 2;
    else if (tmp == tr("3")) i = 3;
    else if (tmp == tr("4")) i = 4;
    else if (tmp == tr("5")) i = 5;
    else if (tmp == tr("6")) i = 6;
    else if (tmp == tr("7")) i = 7;
    else if (tmp == tr("8")) i = 8;

    if (i)
    {
        data_config[4].setNum(i);
        conf_thr = i;
        fprintf(wyj,"thr=%i\n",i);
    }

    if ( comboBox11->currentText() == (tr("Prefer stereo")) )
        tmp="Stereo";
    else
    if ( comboBox11->currentText() == (tr("Prefer mono")) )
        tmp="Mono";
    else
        tmp="Automatic";
    data_config[5] = tmp;
    strcpy(&temp[0], tmp.toLocal8Bit().data());
    fprintf(wyj,"prefer=%s\n",temp);

    tmp = comboBox12->currentText();
    if (tmp == tr("none"))
    {
        data_config[6] = "none";
        fprintf(wyj,"mixer=none\n");
    }
    else
    if (tmp.left(5) == "/dev/")
    {
        data_config[6] = tmp;
        strcpy(&temp[0], tmp.toLocal8Bit().data());
        fprintf(wyj,"mixer=%s\n",temp);
    }

    tmp = comboBox7->currentText();
    data_config[7] = tmp;
    strcpy(&temp[0], tmp.toLocal8Bit().data());
    fprintf(wyj,"chan=%s\n",temp);
    i = channel_by_name(temp);
    if (mixer_has_channel(i))
        conf_chan = i;
    else
    {
        fprintf(stderr, "Now channel %s is inactive - "
                "radio turned on while configuring?\n",
                temp);
        conf_chan = 0;
    }

    /* the following is left untouched, changed in recording.cpp */
    fprintf(wyj,"audio=%s\n",data_config[8].toAscii().data());
    fprintf(wyj,"format=%s\n",data_config[9].toAscii().data());
    fprintf(wyj,"rate=%s\n",data_config[10].toAscii().data());
    fprintf(wyj,"size=%s\n",data_config[11].toAscii().data());

    tmp = comboBox9->currentText();
    if (tmp == tr("Yes"))
    {
        data_config[12] = tr("Yes");
        fprintf(wyj,"exp=Yes\n");
    }
    else
    {
        data_config[12] = tr("No");
        fprintf(wyj,"exp=No\n");
    }

    tmp = comboBox8->currentText();
    if (tmp == tr("Yes"))
    {
        data_config[13] = tr("Yes");
        fprintf(wyj,"dock=Yes\n");
        conf_dock = TRUE;
    }
    else
    {
        data_config[13] = tr("No");
        fprintf(wyj,"dock=No\n");
        conf_dock = FALSE;
    }

    fclose(wyj);

    //save key sequences

    name_home = home_dir();
    name_home.append("/.qtradio/keys");
    strcpy(&path[0], name_home.toLocal8Bit().data());

    QTreeWidgetItem *item;
    QString npl, npr;
    item = listView2->topLevelItem(0);
    for ( i=0; i<15; i++ )
    {
        npl = item->text(0);
        npr = item->text(1);

        for (int j=0; j<15; j++)
        {
            if (npl == names[j])
            {
                kseq[j] = npr;
                break;
            }
        }

        item = listView2->itemBelow(item);
    }

    i = 0;
    wyj = fopen(&path[0],"w");

    fprintf(wyj, "About=%s\n",            kseq[0].toAscii().data());
    fprintf(wyj, "About Qt=%s\n",         kseq[1].toAscii().data());
    fprintf(wyj, "Skins=%s\n",            kseq[2].toAscii().data());
    fprintf(wyj, "Exit QtRadio=%s\n",     kseq[3].toAscii().data());
    fprintf(wyj, "Freq +0.01=%s\n",       kseq[4].toAscii().data());
    fprintf(wyj, "Freq +0.1=%s\n",        kseq[5].toAscii().data());
    fprintf(wyj, "Freq -0.01=%s\n",       kseq[6].toAscii().data());
    fprintf(wyj, "Freq -0.1=%s\n",        kseq[7].toAscii().data());
    fprintf(wyj, "Next Station=%s\n",     kseq[8].toAscii().data());
    fprintf(wyj, "Power On/Off=%s\n",     kseq[9].toAscii().data());
    fprintf(wyj, "Previous Station=%s\n", kseq[10].toAscii().data());
    fprintf(wyj, "Settings=%s\n",         kseq[11].toAscii().data());
    fprintf(wyj, "List=%s\n",             kseq[12].toAscii().data());
    fprintf(wyj, "Vol +=%s\n",            kseq[13].toAscii().data());
    fprintf(wyj, "Vol -=%s\n",            kseq[14].toAscii().data());

    fclose(wyj);

    //QMessageBox::information( this, "QtRadio",(tr("You must click twice 'Power On/Off' to get all changes")));

    show_settings = 0;
    Settings::hide();

}

void Settings::Quit()
{
    show_settings = 0;
    Settings::hide();
}

void  Settings::New_shortcut()
{
    if (listView2->selectedItems().isEmpty() ) return;

    QTreeWidgetItem *item = listView2->selectedItems().at(0);

    if (sh == NULL)
        sh = new Shortcut();
    QLabel *l = sh->textLabel1;

    l->setText("");
    sh->exec();
    if (sh->textLabel1->text() != "")
    {
        item->setText(1, sh->textLabel1->text() );
        for (int i=0; i<15; i++)
        {
            if ( item->text(0) == names[i] )
            {
                kseq[i] = sh->textLabel1->text();
                //fprintf(stderr, "shortcut[%i] now is \'%s\'\n", i, kseq[i].toAscii().data());
                break;
            }
        }
    }
}

void  Settings::New_skin()
{
    if (listView1->selectedItems().isEmpty() ) return;

    QTreeWidgetItem *item = listView1->selectedItems().at(0);

    QString nps;
    char data[150];
    FILE *wyj;

    nps = home_dir();
    nps.append("/.qtradio/skin");


    strcpy(&data[0],nps.toLocal8Bit().data());
    wyj = fopen(&data[0],"w");
    nps = item->text(1);
    SkinDir = nps;
    strcpy(&data[0],nps.toLocal8Bit().data());
    fprintf(wyj,"%s\n",&data[0]);
    fclose(wyj);
    Quit();
}
