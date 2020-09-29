/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Sat Apr 12 00:32:39 CEST 2003
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

// Qt includes
#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include <qcombobox.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qpixmap.h>
//#include <qmainwindow.h>

//own includes
#include "qtradio.h"
#include "TrayIcon.h"
#include "radio_functions.h"
#include "home.h"

#include <sys/stat.h>

KDERadio *Radio;
TrayIcon *ti;

QString FontName[5];
int     FontSize[5];
/*
   0 = KDERadio
   1 = Settings
   2 = About
   3 = Stations
   4 = Recording
 */

/* in qtradio.cpp */
extern QString data_config[16];
/*
    0 = radio device
    1 = min freq
    2 = max freq
    3 = precision
    4 = threshold
    5 = prefer stereo / mono
    6 = mixer device
    7 = mixer channel
    8 = dsp device
    9 = audio format
    10 = samplerate
    11 = sample size
    12 = expanded
    13 = dockable
    14 = stereo capable (not saved)
    15 = rds capable (not saved)
 */

int main(int argc, char *argv[])
{
    char data_file[150];
    QString name_home, first_line;
    QString tmp;
    QFile *file;
    QDir *dir;
    FILE *wyj;

    QApplication a(argc, argv);

    // check if main dir '.kderadio' exists, if not = create it
    name_home = home_dir();
    name_home.append("/.qtradio");
    dir = new QDir(name_home);
    if ( ! dir->exists() )
    {
        strcpy(&data_file[0], name_home.toLocal8Bit().data());
        mkdir(&data_file[0], S_IWUSR | S_IRUSR | S_IXUSR);
    }

    name_home = home_dir();
    name_home.append("/.qtradio/Skins");

    delete dir;

    dir = new QDir(name_home);
    if ( ! dir->exists() )
    {
        strcpy(&data_file[0], name_home.toLocal8Bit().data());
        mkdir(&data_file[0], S_IWUSR | S_IRUSR | S_IXUSR);
    }

    delete dir;

    name_home = home_dir();
    name_home.append("/.qtradio/skin");

    file = new QFile(name_home);
    if ( ! file->exists() )
    {
        strcpy(&data_file[0], name_home.toLocal8Bit().data());
        creat(&data_file[0], S_IWUSR | S_IRUSR);
        strcpy(&data_file[0], name_home.toLocal8Bit().data());
        wyj = fopen(&data_file[0],"w");
        fprintf(wyj,"%s/Skins/Red\n", share_dir());
        fclose(wyj);
    }

    delete file;

    // if file containing stations names doesn't exist  = create it
    name_home = home_dir();
    name_home.append("/.qtradio/stations");
    file = new QFile(name_home);
    if ( ! file->exists() )
    {
        strcpy(&data_file[0], name_home.toLocal8Bit().data());
        creat(&data_file[0], S_IWUSR | S_IRUSR);
    }

    delete file;

    // if file containing station name on start doesn't exist  = create it
    name_home = home_dir();
    name_home.append("/.qtradio/start");
    file = new QFile(name_home);
    if ( ! file->exists() )
    {
        strcpy(&data_file[0], name_home.toLocal8Bit().data());
        creat(&data_file[0], S_IWUSR | S_IRUSR);
        name_home = home_dir();
        wyj = fopen(&data_file[0],"w");
        fprintf(wyj,"%s\n","0");
        fprintf(wyj,"%s\n","50");
        fclose(wyj);
    }

    delete file;

    // Default font names and sizes for widgets
    FontName[0] = "Helvetica"; FontSize[0] = 12;
    FontName[1] = "Helvetica"; FontSize[1] = 11;
    FontName[2] = "Helvetica"; FontSize[2] = 11;
    FontName[3] = "Helvetica"; FontSize[3] = 11;
    FontName[4] = "Helvetica"; FontSize[4] = 11;

    // if file containing font names and sizes doesn't exist  = create it
    name_home = home_dir();
    name_home.append("/.qtradio/fonts");
    file = new QFile(name_home);
    if ( ! file->exists() )
    {
        strcpy(&data_file[0], name_home.toLocal8Bit().data());
        creat(&data_file[0], S_IWUSR | S_IRUSR);
        name_home = home_dir();
        wyj = fopen(&data_file[0],"w");
        fprintf(wyj,"Radio=%d:%s\n", FontSize[0], FontName[0].toAscii().data());
        fprintf(wyj,"Settings=%d:%s\n", FontSize[1], FontName[1].toAscii().data());
        fprintf(wyj,"About=%d:%s\n", FontSize[2], FontName[2].toAscii().data());
        fprintf(wyj,"Stations=%d:%s\n", FontSize[3], FontName[3].toAscii().data());
        fprintf(wyj,"Recording=%d:%s\n", FontSize[4], FontName[4].toAscii().data());
        fclose(wyj);
    }

    delete file;

    // Comes into life with
    // next restart due to
    // asynchronous file ops

    file = new QFile(name_home);
    file->open(QIODevice::ReadOnly);
    if ( file->exists() && file->isReadable() )
    {
        QTextStream t(file);
        QString line;
        while ( ! t.atEnd() )
        {
            line = t.readLine();
            int i = -1;
            int k, s;
            bool good;

            if ( line.left(6) == "Radio=" )
                { i = 0; line.remove(0,6); }
            else
            if ( line.left(9) == "Settings=" )
                { i = 1; line.remove(0,9); }
            else
            if ( line.left(6) == "About=" )
                { i = 2; line.remove(0,6); }
            else
            if ( line.left(9) == "Stations=" )
                { i = 3; line.remove(0,9); }
            else
            if ( line.left(10) == "Recording=" )
                { i = 4; line.remove(0,10); }

            if (i < 0) continue;

            k = line.indexOf(':');
            if (k < 0)
            {
                s = line.toInt(&good);
                if (good && (s >= 6) && (s <= 12))
                    FontSize[i] = s;
                else
                    fprintf(stderr, "Incorrect font size: %s\n",
                        line.toAscii().data());
            }
            else
            {
                s = line.left(k).toInt(&good);
                if (good && (s >= 6) && (s <= 12))
                    FontSize[i] = s;
                else
                    fprintf(stderr, "Incorrect font size: %s\n",
                        line.left(k).toAscii().data());
                line.remove(0,k+1);
                if (line.length() > 0)
                    FontName[i] = line;
                else
                    fprintf(stderr, "Empty font name!\n");
            }
        }
    }

    delete file;

    // QMainWindow x;
    a.setFont(QFont(FontName[0], FontSize[0]));
    QTranslator tor( 0 );
    // set the location where your .qm files are in load() below as the last parameter instead of "."
    // for development, use "/" to use the english original as
    // .qm files are stored in the base project directory.
    tor.load( QString(share_dir()) + QString("/Languages/qtradio.") + QLocale::system().name(), "." );
    a.installTranslator( &tor );
    /* uncomment the following line, if you want a Windows 95 look*/
    // a.setStyle(WindowsStyle);

    Radio = new KDERadio();
    // x.setCentralWidget(Radio);

    QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));


    // show main window and turn radio on
    name_home = QString(share_dir()) + "/icons/qtradio.png";
    Radio->setWindowIcon(QPixmap(name_home));

    Radio->load_settings();
    Radio->load_skin();
    Radio->show();

    if ( data_config[13] == Radio->Yes() ) /* dockable */
    {
        ti = new TrayIcon();
        ti->show();
    }
    int ret=a.exec();
    // fprintf(stderr, "QApplication executed!\n");
    return ret;
}


