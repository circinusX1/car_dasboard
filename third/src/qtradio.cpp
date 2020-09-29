/***************************************************************************
                          qtradio.cpp  -  description
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


//Qt includes
#include <qapplication.h>
#include <qkeysequence.h>
#include <qcombobox.h>
#include <qpainter.h>
#include <qlineedit.h>
#include <qslider.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qtabwidget.h>
#include <qspinbox.h>
#include <qsocketnotifier.h>
#include <qmenubar.h>
#include <qtoolbutton.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qframe.h>

#include <qtreewidget.h>
#include <qheaderview.h>

//own includes
#include "qtradio.h"
#include "TrayIcon.h"
#include "radio_functions.c"
#include "home.h"

//system includes
#include <sys/stat.h>
#include <sys/soundcard.h>
#ifdef HAVE_LIRC
#include <lirc/lirc_client.h>
#endif

extern QString FontName[];
extern int     FontSize[];

static QTimer *Timer;
QTime *Time;
QDate *Date;

int conf_lower=87;
int conf_upper=108;
float conf_dok=0.1;
int conf_thr=2;
int conf_chan;    // main mixer channel (line, vol etc)
bool conf_dock=TRUE;
int show_mixer;
int sound_on = 1;
int show_settings = 0;
int show_recording = 0;
int seek_on = 0;

static Settings *inf = NULL;
static Recording *rec = NULL;
static Add *ad = NULL;
static About *ab = NULL;

QString kseq[20];
QString names[20];

static const char *mixer_names[] = {
    "/dev/mixer",
    "/dev/mixer0",
    "/dev/mixer1",
    "/dev/mixer2",
    NULL
};

static const char *radio_names[] = {
    "/dev/radio",
    "/dev/radio0",
    "/dev/radio1",
    "/dev/radio2",
    NULL
};

static const char *dsp_names[] = {
    "/dev/dsp",
    "/dev/audio",
    "/dev/dsp0",
    "/dev/audio0",
    "/dev/dsp1",
    "/dev/audio1",
    "/dev/dsp2",
    "/dev/audio2",
    NULL
};

QString SkinDir;
QMouseEvent *e;
QPixmap *NumPixmap[10];
QPixmap *SignalPixmap[9];
QPixmap* power_on_pix, *power_off_pix, *stereo_on_pix, *stereo_off_pix;
QImage *rtext;
QPoint Main_pos;

int NORMAL_SIZE_X,NORMAL_SIZE_Y,EXP_SIZE_X,EXP_SIZE_Y;
int ticker_font_w = 6;
int ticker_font_h = 8;

int Freq_x;
int Freq_y;
int Freq_width;
int Freq_pos;

int vol[9];

int Vol_x[9];
int Vol_y[9];
int Vol_height[9];
int Vol_pos[9];

int vol_size_x,vol_size_y,nexty;

QImage *Volume;
QImage *Frequency;
QImage *Number;
QImage *Signal_Led;
QImage *Main_button;
QImage *Window_button;
QImage *List_button;
QImage *Led;

static QTimer *TimerL;
static int lirc_fd = -1;
static int sockfd = 1;
#ifdef HAVE_LIRC
static struct lirc_config *config;
#endif

QTicker::QTicker(QWidget *parent) : QLabel(parent)
{
    przes = 0;
    dlug = 0;
    pass = 0;
    off = 1;
    up = 1;
    freq = 0.0;
    station_name = "None";
    for (int i=0;i<=199;i++)
    {
        part[i] = new QImage(ticker_font_w, ticker_font_h, QImage::Format_Indexed8);
    }

}

void QTicker::newText()
{
    QString nps;

    if (pass == 0)
    {
        nps = QString::number(freq,'f',2);
        nps.append(" MHz ");
        nps.append("* ");
        nps.append(station_name);
    }
    else
    if (pass == 1)
    {
        nps = "* ";
        nps.append( Date->currentDate().toString( "ddd dd MMM yyyy" ) );
        nps.append(" * ");
        nps.append( (Time->currentTime()).toString(Qt::TextDate) );
        nps.append(" * ");
    }
    else
    if (pass == 2)
    {
        nps = "QtRadio " VERSION " " DATE;
    }
    else
    if (pass == 3)
    {
        nps = "\xa9 Pawel Pustelnik, Oleh Nykyforchyn";
    }

    int i = 0;

    while ( nps[i] != '\0' )
    {
        *part[i] = rtext->copy(36*(ticker_font_w+1),0,ticker_font_w,ticker_font_h); // question sign

        uint s = nps[i].unicode();
//        fprintf(stderr, "%d ", s);

        // First row - latin chars
        if ( (s == 'A') || (s == 'a') )
        {
            *part[i] = rtext->copy(0*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'B') || (s == 'b') )
        {
            *part[i] = rtext->copy(1*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'C') || (s == 'c') )
        {
            *part[i] = rtext->copy(2*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'D') || (s == 'd') )
        {
            *part[i] = rtext->copy(3*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'E') || (s == 'e') )
        {
            *part[i] = rtext->copy(4*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'F') || (s == 'f') )
        {
            *part[i] = rtext->copy(5*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'G') || (s == 'g') )
        {
            *part[i] = rtext->copy(6*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'H') || (s == 'h') )
        {
            *part[i] = rtext->copy(7*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'I') || (s == 'i') )
        {
            *part[i] = rtext->copy(8*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'J') || (s == 'j') )
        {
            *part[i] = rtext->copy(9*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'K') || (s == 'k') )
        {
            *part[i] = rtext->copy(10*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'L') || (s == 'l') )
        {
            *part[i] = rtext->copy(11*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'M') || (s == 'm') )
        {
            *part[i] = rtext->copy(12*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'N') || (s == 'n') )
        {
            *part[i] = rtext->copy(13*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'O') || (s == 'o') )
        {
            *part[i] = rtext->copy(14*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'P') || (s == 'p') )
        {
            *part[i] = rtext->copy(15*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'Q') || (s == 'q') )
        {
            *part[i] = rtext->copy(16*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'R') || (s == 'r') )
        {
            *part[i] = rtext->copy(17*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'S') || (s == 's') )
        {
            *part[i] = rtext->copy(18*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'T') || (s == 't') )
        {
            *part[i] = rtext->copy(19*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'U') || (s == 'u') )
        {
            *part[i] = rtext->copy(20*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'V') || (s == 'v') )
        {
            *part[i] = rtext->copy(21*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'W') || (s == 'w') )
        {
            *part[i] = rtext->copy(22*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'X') || (s == 'x') )
        {
            *part[i] = rtext->copy(23*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'Y') || (s == 'y') )
        {
            *part[i] = rtext->copy(24*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 'Z') || (s == 'z') )
        {
            *part[i] = rtext->copy(25*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '\\' )
        {
            *part[i] = rtext->copy(26*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '/' )
        {
            *part[i] = rtext->copy(27*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '@' )
        {
            *part[i] = rtext->copy(28*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '!' )
        {
            *part[i] = rtext->copy(29*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '[' )
        {
            *part[i] = rtext->copy(30*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == ']' )
        {
            *part[i] = rtext->copy(31*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '*' )
        {
            *part[i] = rtext->copy(32*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == ':' )
        {
            *part[i] = rtext->copy(33*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == ' ' )
        {
            *part[i] = rtext->copy(34*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '.' )
        {
            *part[i] = rtext->copy(35*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '?' )
        {
            *part[i] = rtext->copy(36*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '-' )
        {
            *part[i] = rtext->copy(37*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == ',' )
        {
            *part[i] = rtext->copy(38*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        if ( s == 0x0a9 ) // copyright sign
        {
            *part[i] = rtext->copy(39*(ticker_font_w+1),0,ticker_font_w,ticker_font_h);
        }
        else
        // Second row - digits
        if ( s == '0' )
        {
            *part[i] = rtext->copy(0*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '1' )
        {
            *part[i] = rtext->copy(1*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '2' )
        {
            *part[i] = rtext->copy(2*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '3' )
        {
            *part[i] = rtext->copy(3*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '4' )
        {
            *part[i] = rtext->copy(4*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '5' )
        {
            *part[i] = rtext->copy(5*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '6' )
        {
            *part[i] = rtext->copy(6*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '7' )
        {
            *part[i] = rtext->copy(7*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '8' )
        {
            *part[i] = rtext->copy(8*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( s == '9' )
        {
            *part[i] = rtext->copy(9*(ticker_font_w+1),1*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        // Third row - cyrillic chars
        if ( rtext->height() > 2*ticker_font_h+3 ) { //

        if ( (s == 0x410) || (s == 0x430) )
        {
            *part[i] = rtext->copy(0*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x411) || (s == 0x431) )
        {
            *part[i] = rtext->copy(1*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x412) || (s == 0x432) )
        {
            *part[i] = rtext->copy(2*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x413) || (s == 0x433) )
        {
            *part[i] = rtext->copy(3*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x490) || (s == 0x491) )
        {
            *part[i] = rtext->copy(4*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x414) || (s == 0x434) )
        {
            *part[i] = rtext->copy(5*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x415) || (s == 0x435) )
        {
            *part[i] = rtext->copy(6*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x404) || (s == 0x454) )
        {
            *part[i] = rtext->copy(7*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x416) || (s == 0x436) )
        {
            *part[i] = rtext->copy(8*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x417) || (s == 0x437) )
        {
            *part[i] = rtext->copy(9*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x418) || (s == 0x438) )
        {
            *part[i] = rtext->copy(10*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x406) || (s == 0x456) )
        {
            *part[i] = rtext->copy(11*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x407) || (s == 0x457) )
        {
            *part[i] = rtext->copy(12*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x419) || (s == 0x439) )
        {
            *part[i] = rtext->copy(13*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x41a) || (s == 0x43a) )
        {
            *part[i] = rtext->copy(14*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x41b) || (s == 0x43b) )
        {
            *part[i] = rtext->copy(15*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x41c) || (s == 0x43c) )
        {
            *part[i] = rtext->copy(16*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x41d) || (s == 0x43d) )
        {
            *part[i] = rtext->copy(17*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x41e) || (s == 0x43e) )
        {
            *part[i] = rtext->copy(18*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x41f) || (s == 0x43f) )
        {
            *part[i] = rtext->copy(19*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x420) || (s == 0x440) )
        {
            *part[i] = rtext->copy(20*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x421) || (s == 0x441) )
        {
            *part[i] = rtext->copy(21*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x422) || (s == 0x442) )
        {
            *part[i] = rtext->copy(22*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x423) || (s == 0x443) )
        {
            *part[i] = rtext->copy(23*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x424) || (s == 0x444) )
        {
            *part[i] = rtext->copy(24*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x425) || (s == 0x445) )
        {
            *part[i] = rtext->copy(25*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x426) || (s == 0x446) )
        {
            *part[i] = rtext->copy(26*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x427) || (s == 0x447) )
        {
            *part[i] = rtext->copy(27*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x428) || (s == 0x448) )
        {
            *part[i] = rtext->copy(28*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x429) || (s == 0x449) )
        {
            *part[i] = rtext->copy(29*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x42e) || (s == 0x44e) )
        {
            *part[i] = rtext->copy(30*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x42f) || (s == 0x44f) )
        {
            *part[i] = rtext->copy(31*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x42c) || (s == 0x44c) )
        {
            *part[i] = rtext->copy(32*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x42d) || (s == 0x44d) )
        {
            *part[i] = rtext->copy(33*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x42a) || (s == 0x44a) )
        {
            *part[i] = rtext->copy(34*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x42b) || (s == 0x44b) )
        {
            *part[i] = rtext->copy(35*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x40e) || (s == 0x45e) ) // belarussian u short
        {
            *part[i] = rtext->copy(36*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }
        else
        if ( (s == 0x401) || (s == 0x451) )  // io
        {
            *part[i] = rtext->copy(37*(ticker_font_w+1),2*(ticker_font_h+1),ticker_font_w,ticker_font_h);
        }

        } // End of cyrillic

        i++;

      }

//        fprintf(stderr, "\n");

      dlug = i;

}

void QTicker::init(float f)
{
    przes = 0;
    pass = 0;
    up = 1;
    station_name = "None";
    if (f == 0)
    {
        off = 1;
        update();
    }
    else
    {
        off = 0;
        freq = f;
        QTreeWidgetItem *item = Radio->closestStation(f);
        if (item != NULL)
            station_name = item->text(0);
        newText();
    }
}

void QTicker::paintEvent(QPaintEvent*)
{
    if (off && up)
    {
        QPainter painter;
        painter.begin(this);
        painter.eraseRect( QRect(0,0,this->width(),this->height()) );
        painter.end();
        przes++;
        up = 0;
    }
    else
    {
        if (!up) return;

        if ( przes < 4*dlug )
        {
            QPainter painter;
            painter.begin(this);
            for (int i=0; (i<=przes) && (i<dlug); i++)
                painter.drawImage( (i+1)*(ticker_font_w+1), 0, *part[i] );
            painter.end();
            przes++;
            up = 0;
        }
        else
        {
            QPainter painter;
            painter.begin(this);
            painter.eraseRect( QRect(0,0,this->width(),this->height()) );
            painter.end();
            pass++;
            if (pass > 3) pass = 0;
            newText();
            przes = 0;
        }
    }

}


QString data_config[16];
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
    14 = stereo capable
    15 = rds capable
 */

KDERadio::KDERadio(QWidget *parent ) : QWidget(parent, Qt::FramelessWindowHint | Qt::Tool)
{ /* Qt::WFlags f */

    setAttribute(Qt::WA_QuitOnClose, TRUE);
    setAttribute(Qt::WA_DeleteOnClose, TRUE);
    setMouseTracking ( FALSE );
    power_on = FALSE;
    strength = 0;
    force = 2;
    inf = NULL;
    rec = NULL;

    vol[0] = 0;

    QFont font(FontName[0], FontSize[0]);
    QFont button_font(FontName[0], 10);

    qRegisterMetaType<QImage>("QImage");
    connect(this, SIGNAL(putImageButton(QImage,QToolButton*)),
             this, SLOT(getImageButton(QImage,QToolButton*)));
    connect(this, SIGNAL(putImageLabel(QImage,QLabel*)),
             this, SLOT(getImageLabel(QImage,QLabel*)));
    connect(this, SIGNAL(putImagePix(QImage,QPixmap*)),
             this, SLOT(getImagePix(QImage,QPixmap*)));

    Date = new QDate();
    Time = new QTime();
    Timer = new QTimer(this);

    frame = new QFrame(this);

    ad = new Add; // used by Ticker
    connect( ad, SIGNAL(change_station(float)),
         this, SLOT(set_freq(float)) );

    Ticker = new QTicker(frame);
    Ticker->setGeometry( QRect( 120, 10, 250, 24 ) );
    connect( Timer, SIGNAL(timeout()),
         this, SLOT(nice_text()) );

    Seeker = new QSeeker(this, ad);
    connect( Seeker, SIGNAL(finished()),
         this, SLOT(show_found()) );

    style = new QMenu( this );
    style_dock = new QMenu( this );
    style->setFont(font);
    style_dock->setFont(font);

    Previous_button = new QToolButton( this );
    Previous_button->setGeometry( QRect( 120, 40, 18, 18 ) );
    Previous_button->setPalette(QPalette(QColor(0,0,0)));

    Next_button = new QToolButton( this );
    Next_button->setGeometry( QRect( 160, 40, 18, 18 ) );
    Next_button->setPalette(QPalette(QColor(0,0,0)));

    Close_button = new QToolButton( this );
    Close_button->setGeometry( QRect( 423, 2, 8, 8 ) );

    Min_button = new QToolButton( this );
    Min_button->setGeometry( QRect( 413, 2, 8, 8 ) );

    Move_button  = new QToolButton( this );
    Move_button->setGeometry( QRect( 403, 2, 8, 8 ) );

    Freq_button = new QLabel( this );
    Freq_button->setMouseTracking(TRUE);

    Vol_button[0] = new QLabel( this );
    Vol_button[0]->setMouseTracking(TRUE);

    Vol_bar[1] = new QLabel( this );

    Vol_bar[2] = new QLabel( this );

    Vol_bar[3] = new QLabel( this );

    Vol_bar[4] = new QLabel( this );

    Vol_bar[5] = new QLabel( this );

    Vol_bar[6] = new QLabel( this );

    Vol_bar[7] = new QLabel( this );

    Vol_bar[8] = new QLabel( this );

    Vol_button[1] = new QLabel( this );
    Vol_button[1]->setMouseTracking(TRUE);

    Vol_button[2] = new QLabel( this );
    Vol_button[2]->setMouseTracking(TRUE);

    Vol_button[3] = new QLabel( this );
    Vol_button[3]->setMouseTracking(TRUE);

    Vol_button[4] = new QLabel( this );
    Vol_button[4]->setMouseTracking(TRUE);

    Vol_button[5] = new QLabel( this );
    Vol_button[5]->setMouseTracking(TRUE);

    Vol_button[6] = new QLabel( this );
    Vol_button[6]->setMouseTracking(TRUE);

    Vol_button[7] = new QLabel( this );
    Vol_button[7]->setMouseTracking(TRUE);

    Vol_button[8] = new QLabel( this );
    Vol_button[8]->setMouseTracking(TRUE);

    FreqDigit[0] = new QLabel(this);
    FreqDigit[1] = new QLabel(this);
    FreqDigit[2] = new QLabel(this);
    FreqDigit[3] = new QLabel(this);
    FreqDigit[4] = new QLabel(this);
    Power = new QLabel(this);	
    Stereo = new QLabel(this);
    Signal = new QLabel(this);

    Saver_button = new QToolButton( this );
    Saver_button->setGeometry( QRect( 290, 95, 18, 18 ) );
    Saver_button->setFont( button_font );
    Saver_button->setPalette(QPalette(QColor(0,0,0)));

    Mixer_button = new QToolButton( this );
    Mixer_button->setGeometry( QRect( 200, 95, 18, 18 ) );
    Mixer_button->setFont( button_font );
    Mixer_button->setPalette(QPalette(QColor(0,0,0)));

    Rec_button = new QToolButton( this );
    Rec_button->setGeometry( QRect( 170, 95, 18, 18 ) );
    Rec_button->setFont( button_font );
    Rec_button->setPalette(QPalette(QColor(0,0,0)));

    Options_button = new QToolButton( this );
    Options_button->setGeometry( QRect( 230, 95, 18, 18 ) );
    Options_button->setFont( button_font );
    Options_button->setPalette(QPalette(QColor(0,0,0)));

    Seek_button = new QToolButton( this );
    Seek_button->setGeometry( QRect( 140, 95, 18, 18 ) );
    Seek_button->setFont( button_font );
    Seek_button->setPalette(QPalette(QColor(0,0,0)));

    Forward_long = new QToolButton( this );
    Forward_long->setGeometry( QRect( 110, 95, 18, 18 ) );
    Forward_long->setFont( button_font );
    Forward_long->setPalette(QPalette(QColor(0,0,0)));

    Backward_long = new QToolButton( this );
    Backward_long->setGeometry( QRect( 30, 95, 18, 18 ) );
    Backward_long->setFont( button_font );
    Backward_long->setPalette(QPalette(QColor(0,0,0)));

    Backward_short = new QToolButton( this );
    Backward_short->setGeometry( QRect( 56, 95, 18, 18 ) );
    Backward_short->setFont( button_font );
    Backward_short->setPalette(QPalette(QColor(0,0,0)));

    Forward_short = new QToolButton( this );
    Forward_short->setGeometry( QRect( 84, 95, 18, 18 ) );
    Forward_short->setFont( button_font );
    Forward_short->setPalette(QPalette(QColor(0,0,0)));

    Sound_mute = new QToolButton( this );
    Sound_mute->setGeometry( QRect( 320, 95, 18, 18 ) );
    Sound_mute->setFont( button_font );
    Sound_mute->setPalette(QPalette(QColor(0,0,0)));

    Switch_button = new QToolButton( this );
    Switch_button->setGeometry( QRect( 380, 96, 18, 18 ) );
    Switch_button->setFont( button_font );
    Switch_button->setPalette(QPalette(QColor(0,0,0)));

    for (int i=0; i<9; i++)
    {
        SignalPixmap[i] = new QPixmap();
    }

    for (int i=0; i<10; i++)
    {
        NumPixmap[i] = new QPixmap();
    }

    power_on_pix   = new QPixmap();
    power_off_pix  = new QPixmap();
    stereo_on_pix  = new QPixmap();
    stereo_off_pix = new QPixmap();

    Previous_button->setWhatsThis( tr( "Previous station" ) );
    Previous_button->setToolTip( tr( "Previous station" ) );

    Next_button->setWhatsThis( tr( "Next station" ) );
    Next_button->setToolTip( tr( "Next station" ) );

    Saver_button->setWhatsThis( tr( "Show stations list" ) );
    Saver_button->setToolTip( tr( "Show stations list" ) );

    Mixer_button->setWhatsThis( tr( "Show/Hide mixer" ) );
    Mixer_button->setToolTip( tr( "Show/Hide mixer" ) );

    Rec_button->setWhatsThis( tr( "Recording" ) );
    Rec_button->setToolTip( tr( "Recording" ) );

    Options_button->setWhatsThis( tr( "Settings window" ) );
    Options_button->setToolTip( tr( "Settings window" ) );

    Seek_button->setWhatsThis( tr( "Find new stations and save them" ) );
    Seek_button->setToolTip( tr( "Find new stations and save them" ) );

    Forward_long->setWhatsThis( tr( "Frequency + 0.1 MHz" ) );
    Forward_long->setToolTip( tr( "Frequency + 0.1 MHz" ) );

    Backward_long->setWhatsThis( tr( "Frequency - 0.1 MHz" ) );
    Backward_long->setToolTip( tr( "Frequency - 0.1 MHz" ) );

    Backward_short->setWhatsThis( tr( "Frequency - 0.01 MHz" ) );
    Backward_short->setToolTip( tr( "Frequency - 0.01 MHz" ) );

    Forward_short->setWhatsThis( tr( "Frequency + 0.01 MHz" ) );
    Forward_short->setToolTip( tr( "Frequency + 0.01 MHz" ) );

    Sound_mute->setWhatsThis( tr( "Mute sound" ) );
    Sound_mute->setToolTip( tr( "Mute sound" ) );

    Switch_button->setWhatsThis( tr( "Power On/Off" ) );
    Switch_button->setToolTip( tr( "Power On/Off" ) );

    resize( QSize(433, 120).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( Switch_button, SIGNAL( clicked() ), this, SLOT( Switcher() ) );
    connect( Sound_mute, SIGNAL( clicked() ), this, SLOT( mute_pressed() ) );
    connect( Forward_long, SIGNAL( clicked() ), this, SLOT( on_Forward() ) );
    connect( Backward_long, SIGNAL( clicked() ), this, SLOT( on_Backward() ) );
    connect( Forward_short, SIGNAL( clicked() ), this, SLOT( short_Forward() ) );
    connect( Backward_short, SIGNAL( clicked() ), this, SLOT( short_Backward() ) );
    connect( Mixer_button, SIGNAL( clicked() ), this, SLOT( on_mixer() ) );
    connect( Seek_button, SIGNAL( clicked() ), this, SLOT( on_seeking() ) );
    connect( Options_button, SIGNAL( clicked() ), this, SLOT( on_info() ) );
    connect( Rec_button, SIGNAL( clicked() ), this, SLOT( on_record() ) );
    connect( Next_button, SIGNAL( clicked() ), this, SLOT( next_station() ) );
    connect( Previous_button, SIGNAL( clicked() ), this, SLOT( prev_station() ) );
    connect( Close_button, SIGNAL( clicked() ), this, SLOT( close_main()) );
    connect( Move_button, SIGNAL( clicked() ), this, SLOT( move_main() ) );
    connect( Min_button, SIGNAL( clicked() ), this, SLOT( min_main() ) );
    connect( Saver_button, SIGNAL( clicked() ), this, SLOT( edit_stations() ) );
    connect( Timer, SIGNAL(timeout()), this, SLOT(load_sound() ) );
    connect( Timer, SIGNAL(timeout()), this, SLOT(setting_leds_signal() ) );

}

void KDERadio::mousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::RightButton)
    {
        style->exec(QCursor::pos());
    }

    if (e->button() == Qt::LeftButton)
    {
        Freq_pos = e->x() - Freq_button->x();

        Vol_pos[0] = e->x() - Vol_button[0]->x();

        Vol_pos[1] = e->y() - Vol_button[1]->y();
        Vol_pos[2] = e->y() - Vol_button[2]->y();
        Vol_pos[3] = e->y() - Vol_button[3]->y();
        Vol_pos[4] = e->y() - Vol_button[4]->y();
        Vol_pos[5] = e->y() - Vol_button[5]->y();
        Vol_pos[6] = e->y() - Vol_button[6]->y();
        Vol_pos[7] = e->y() - Vol_button[7]->y();
        Vol_pos[8] = e->y() - Vol_button[8]->y();

        Main_pos = e->pos() ;
    }
}

void KDERadio::mouseMoveEvent( QMouseEvent *e )
{
    int i;

    KDERadio::setMouseTracking(FALSE);


    if ( (e->x() >= Freq_button->x() ) && (e->x() <= Freq_button->x() + Freq_button->width()) && (e->y() >= Freq_button->y()) && (e->y() <=Freq_button->y() + Freq_button->height()) )
    {
        Freq_button->move(e->x() - Freq_pos,Freq_y);
        if (  Freq_button->x() <=  Freq_x)
            Freq_button->move(Freq_x,Freq_y);
        if (  Freq_button->x() >=  (Freq_x+Freq_width) )
            Freq_button->move( (Freq_x+Freq_width) ,Freq_y);
        freq_from_slider();
        goto finish;
    }

    else if ( (e->x() >= Vol_button[0]->x() ) && (e->x() <= Vol_button[0]->x() + Vol_button[0]->width()) && (e->y() >= Vol_button[0]->y()) && (e->y() <=Vol_button[0]->y() + Vol_button[0]->height()) )
    {
        Vol_button[0]->move(e->x() - Vol_pos[0],Vol_y[0]);
        if (  Vol_button[0]->x() <=  Vol_x[0])
            Vol_button[0]->move(Vol_x[0],Vol_y[0]);
        if (  Vol_button[0]->x() >=  (Vol_x[0]+Vol_height[0]) )
            Vol_button[0]->move( (Vol_x[0]+Vol_height[0]) ,Vol_y[0]);
        vol_from_slider();
        goto finish;
    }

    else
    for (i=1; i<=8; i++)
    {
      if ( (e->x() >= Vol_button[i]->x() ) &&
           (e->x() <= Vol_button[i]->x() + Vol_button[i]->width()) &&
           (e->y() >= Vol_button[i]->y()) &&
           (e->y() <= Vol_button[i]->y() + Vol_button[i]->height()) )
      {
        Vol_button[i]->move(Vol_x[i], e->y()-Vol_pos[i]);
        if (  Vol_button[i]->y() <=  (Vol_y[i]-Vol_height[i]) )
            Vol_button[i]->move(Vol_x[i],Vol_y[i]-Vol_height[i]);
        if (  Vol_button[i]->y() >=  (Vol_y[i]) )
            Vol_button[i]->move(Vol_x[i], Vol_y[i] );
        int val = ( ((Vol_y[i]-Vol_button[i]->y() )*1.0)/(Vol_height[i])  ) *100;
        vol[i] = val;
        switch (i)
        {
          case 1:
            set_volume(val, 0);
            break;
          case 2:
            set_volume(val, 4);
            break;
          case 3:
            set_volume(val, 5);
            break;
          case 4:
            set_volume(val, 6);
            break;
          case 5:
            set_volume(val, 7);
            break;
          case 6:
            set_volume(val, 8);
            break;
          case 7:
            set_volume(val, 22);
            break;
          case 8:
            set_volume(val, 14);
            break;
        }
        goto finish;
      }
    }

    if ( e->y() <= 50 )
    {
        KDERadio::move(e->globalPos() - Main_pos );
    }

  finish:
    e->accept();
}

//if quit clicked, before quiting close device
void KDERadio::closeEvent( QCloseEvent* ce )
{
    ce->accept();
    on_exit();
}

//if quit clicked, before quiting close device
void KDERadio::on_exit()
{
    if (power_on)
        save_start();

    close_radio();
    close_mixer();

    if (sockfd == 1)
    {
      #ifdef HAVE_LIRC
        deinitialize_lirc();
      #endif
    }
    Timer->stop();

    if (inf) delete inf; inf = NULL;
    if (rec) delete rec; rec = NULL;
    if (ab)  delete ab;  ab  = NULL;
    if (ad)  delete ad;  ad  = NULL;

    KDERadio::close();
    // fprintf(stderr,"KDERadio closed!\n");
}

void KDERadio::close_main()
{
    if ( conf_dock == FALSE )
        KDERadio::on_exit();
    else
        KDERadio::hide();
}

void KDERadio::min_main()
{
    KDERadio::showMinimized();
}

void KDERadio::show_skin()
{
    start_info(1);
}

void KDERadio::on_info()
{
    start_info(0);
}

void KDERadio::move_main()
{
    style->exec(QCursor::pos());
}


KDERadio::~KDERadio()
{
    // fprintf(stderr, "KDERadio deleted\n");
}


void KDERadio::volume_up()
{
    if ( Vol_button[0]->x() < (Vol_height[0]+Vol_x[0]) )
        Vol_button[0]->move( Vol_button[0]->x()+( Vol_height[0]*0.1 ) ,Vol_y[0]);
    vol_from_slider();
}


void KDERadio::volume_down()
{
    if ( Vol_button[0]->x() > (Vol_x[0]) )
       Vol_button[0]->move( Vol_button[0]->x()-( Vol_height[0]*0.1 ) ,Vol_y[0]);
    vol_from_slider();
}


void KDERadio::setChannelBar(int i)
{
    int v;
    QImage img;

    if ((i<0) || (i>8)) return;

    v = vol[i];

    if (v<= 0)
        v = 0;
    else
    if (v >=100)
        v = 100;

    if (i == 0)
    {
        Vol_button[0]->move( Vol_x[0] +
                     (v*0.01)*Vol_height[0] ,Vol_y[0] );
        return;
    }

    Vol_button[i]->move(Vol_x[i], (Vol_y[i]  - (v*1.0/100) * Vol_height[i] ) );
    // fprintf(stderr, "%d x %d , (%d, %d)\n", but->width(), but->height(), but->x(), but->y());
    v /= 4;
    if (v > 23) v = 23;
    if ( v < 12)
        img = Volume->copy((vol_size_x+2)*v+1,0,vol_size_x,vol_size_y);
    else
        img = Volume->copy((vol_size_x+2)*(v-12)+1,nexty,vol_size_x,vol_size_y);
    emit putImageLabel(img, Vol_bar[i]);
}


// set new volume when slider's value is changing
void KDERadio::vol_from_slider()
{
  vol[0] = ( ((Vol_button[0]->x() - Vol_x[0])*1.0)/(Vol_height[0])  ) *100;
  set_volume(vol[0], conf_chan);
  //other label also should move
}

// set new frequency when slider's value is changing
void KDERadio::freq_from_slider()
{
    float f = ( ((Freq_button->x() - Freq_x)*1.0)/Freq_width )*(conf_upper-conf_lower) + 1.0*conf_lower;

    set_freq(f);
}

// set new frequency with a value
void KDERadio::set_freq(float f)
{
    int i = f*100+0.5;
    f = i*0.01;

    if ((f < conf_lower) || (f > conf_upper))
        return;

    freq = f;

    Ticker->freq = f;
    Ticker->newText();

    setting_leds_freq();  //show signal's value
}

// muting
void KDERadio::mute_pressed()
{
    if ( sound_on == 1)
    {
        mute(1);
        sound_on = 0;
    }
    else
    {
        mute(0);
        sound_on = 1;
    }
}

// edit station list
void KDERadio::edit_stations()
{

    if ( !power_on )
        ad->Load_list();

    ad->exec();
}

// update text and leds
void KDERadio::update_panel()
{
    setting_leds_signal();
    setting_leds_freq();

    Ticker->newText();
}

// nice text above slider
void KDERadio::nice_text()
{
    Ticker->up = 1;
    Ticker->update();
}


void KDERadio::Power_S()
{
    load_settings();
    create_menu();
    load_skin();
    Switcher();
    power_on = 1;
}

QString KDERadio::Yes()
{
    return tr("Yes");
}

void KDERadio::save_start()
{
    QString dest;
    char data[50];
    FILE *wyj;

    dest = home_dir();
    dest.append("/.qtradio/start");
    strcpy(&data[0], dest.toLocal8Bit().data());
    wyj = fopen(&data[0], "w");
    fprintf(wyj,"%.2f\n", freq);
    fprintf(wyj,"%i\n", vol[0]);
    fclose(wyj);
}

// when main switch is clicked
void KDERadio::Switcher()
{
    if ( power_on )
    {
        Power->setPixmap(*power_off_pix);
        Signal->setPixmap(*SignalPixmap[0]);
        save_start();
        freq=0;
        setting_leds_freq();
        Stereo->setPixmap(*stereo_off_pix);
        power_on = FALSE;
        seek_on = 0;
        close_radio();
        close_mixer();
        Ticker->init(0);
        Timer->stop();
    }

    else
    {
        load_skin();
        load_settings(); // load all settings
        create_menu();

        ad->Load_list(); // load station's list
        freq = 108;

        char data[50];
        if (data_config[6] == "none")
        {
            init_mixer("");
            QMessageBox::information( this, "QtRadio",
                tr("No mixer devices?") );
        }
        else
        {
            strcpy(&data[0], data_config[6].toAscii().data());
            if ( init_mixer(data) != 1)
            {
                QString nps = (tr("Can't open mixer device : "));
                nps.append(data_config[6]);
                nps.append(tr(" => Mixer is off"));
                QMessageBox::information( this, "QtRadio",nps);
                //return;
            }
            else
            {
                load_start();    // load volume and freq
                load_sound();//load previous volume value
            }
        }

        if (data_config[0] == "none")
        {
            initialize("");
            QMessageBox::information( this, "QtRadio",
                tr("No radio devices?") );
        }
        else
        {
            strcpy(&data[0], data_config[0].toAscii().data());
            if ( initialize(data) != 1 )
            {
                QString nps = (tr("Can't open radio device : "));
                nps.append(data_config[0]);
                nps.append(tr(" => Radio Tuner is off"));
                QMessageBox::information( this, "QtRadio", nps);
                //return ;
            }
        }

        mute(0);
        Power->setPixmap(*power_on_pix);
        power_on = TRUE;
        setting_leds_freq();
        setting_leds_signal();
        // timer for edit above slider, showing frequency and time etc
        Ticker->init(freq);
        Timer->start(30);
#ifdef HAVE_LIRC
        if ( !initialize_lirc() )
            fprintf(stderr, "Can't initialize LIRC\n");
        else
            start_lirc();
#endif

    }
}

// add 0.1 to current frequency
void KDERadio::on_Forward()
{
    set_freq(freq+0.1);
}

// sub 0.1 to current frequency
void KDERadio::on_Backward()
{
    set_freq(freq-0.1);
}

// add 0.01 to current frequency
void KDERadio::short_Forward()
{
    set_freq(freq+0.01);
}

// sub 0.01 to current frequency
void KDERadio::short_Backward()
{
    set_freq(freq-0.01);
}

// get part of QImage into QToolButton
void KDERadio::getImageButton(const QImage& image, QToolButton *but)
{
    mutex.lock();
    QPixmap pix;
    pix.convertFromImage(image);
    but->resize(pix.size());
    but->setMask(pix.createHeuristicMask());
    but->setIcon(pix);
    but->setStyleSheet("QToolButton {border : none}");
    mutex.unlock();
}

// get part of QImage into QLabel
void KDERadio::getImageLabel(const QImage& image, QLabel *label)
{
    mutex.lock();
    QPixmap pix;
    pix.convertFromImage(image);
//    but->resize(pix.size());
//    but->setMask(pix.createHeuristicMask());
    label->setPixmap(pix);
//    label->setStyleSheet("QLabel {border : none}");
    mutex.unlock();
}

// get part of QImage into QPixmap
void KDERadio::getImagePix(const QImage& image, QPixmap *pxaddr)
{
    mutex.lock();
    pxaddr->convertFromImage(image);
    mutex.unlock();
}

//show frequency in LCD view
void KDERadio::setting_leds_freq()
{
    int temp, digit;
    QString nps;
    QString temp_nps;

    temp = (freq+0.005)*100;

    digit = temp % 10;
    temp /= 10;
    FreqDigit[4]->setPixmap(*NumPixmap[digit]);

    digit = temp % 10;
    temp /= 10;
    FreqDigit[3]->setPixmap(*NumPixmap[digit]);

    digit = temp % 10;
    temp /= 10;
    FreqDigit[2]->setPixmap(*NumPixmap[digit]);

    digit = temp % 10;
    temp /= 10;
    FreqDigit[1]->setPixmap(*NumPixmap[digit]);

    digit = temp % 10;
    FreqDigit[0]->setPixmap(*NumPixmap[digit]);

    set_frequency(freq);

    QTreeWidgetItem *item = closestStation(freq);
    if (item == NULL)
        Ticker->station_name = "None";
    else
        Ticker->station_name = item->text(0);


    // turn red led on if stereo
    switch ( check_stereo(force) ) {
        case 1 :
            Stereo->setPixmap(*stereo_on_pix);
            break;
        case 0 :
        default : // Handle incorrect mode?
            Stereo->setPixmap(*stereo_off_pix);
            break;
    }

    Freq_button->move( Freq_x + ((freq-conf_lower)/(conf_upper-conf_lower))*(Freq_width), Freq_y);

}

// show strength of signal
void KDERadio::setting_leds_signal()
{
    strength = signal_strength();
    // fprintf(stderr,"Signal strength = %i\n",strength);

    if (strength >= 8)
        Signal->setPixmap(*SignalPixmap[8]);
    else
        Signal->setPixmap(*SignalPixmap[strength]);
}

void KDERadio::on_mixer()
{

  if (show_mixer == 0)
    {
      KDERadio::resize( QSize(EXP_SIZE_X, EXP_SIZE_Y).expandedTo(minimumSizeHint()) );
      show_mixer = 1;
      return;
    }
  if (show_mixer == 1)
    {
      KDERadio::resize( QSize(NORMAL_SIZE_X, NORMAL_SIZE_Y).expandedTo(minimumSizeHint()) );
      show_mixer = 0;
      return;
    }
}

// load last frequency and volume
void KDERadio::load_start()
{

    freq = conf_lower;

    QString name_home = home_dir();
    name_home.append("/.qtradio/start");
    QFile file(name_home);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream t(&file);
        bool good;
        float fr=t.readLine().toFloat(&good);
        if (good && (fr >= conf_lower) && (fr <= conf_upper))
            freq = fr;
        if (vol[0] == 0)
        {
            vol[0] = t.readLine().toInt();
            set_volume(vol[0], conf_chan);
        }
        else
            t.readLine();
    }
    set_freq(freq);
    if ( vol[0] < 0 )
        vol[0] = 0;
    else
    if ( vol[0] > 100 )
        vol[0] = 100;
}

void KDERadio::load_settings()
{
    QString name_home;
    int i;
    float f;
    QString temp;
    bool ok;
    const char *devices_table[] = SOUND_DEVICE_NAMES;
    const char **nm;

    data_config[0] = "none";
    nm = &radio_names[0];
    for ( ; *nm; nm++ ) {
        if (access(*nm, F_OK))  continue;
        if (access(*nm, R_OK | W_OK))
        {
            fprintf(stderr,
                "Insufficient rights for \'%s\', please verify!\n", *nm);
            continue;
        }
        data_config[0] = *nm;
        break;
    }
    if (data_config[0] == "none")
    {
        fprintf(stderr, "No radio devices found!\n");
    }

    data_config[1]="87";           conf_lower = 87;
    data_config[2]="108";          conf_upper = 108;
    data_config[3]="0.1";          conf_dok = 0.1;
    data_config[4]="2";            conf_thr = 2;
    data_config[5]="Stereo";

    data_config[6]="none";
    nm = &mixer_names[0];
    for ( ; *nm; nm++ ) {
        if (access(*nm, F_OK))  continue;
        if (access(*nm, R_OK | W_OK))
        {
            fprintf(stderr,
                "Insufficient rights for \'%s\', please verify!\n", *nm);
            continue;
        }
        data_config[6] = *nm;
        break;
    }
    if (data_config[6] == "none")
    {
        fprintf(stderr, "No mixer devices found!\n");
    }

    data_config[7]="line";

    data_config[8]="none";
    nm = &dsp_names[0];
    for ( ; *nm; nm++ ) {
        if (access(*nm, F_OK))  continue;
        if (access(*nm, R_OK | W_OK))
        {
            fprintf(stderr,
                "Insufficient rights for \'%s\', please verify!\n", *nm);
            continue;
        }
        data_config[8] = *nm;
        break;
    }
    if (data_config[8] == "none")
    {
        fprintf(stderr, "No audio devices found!\n");
    }

    data_config[9]="AFMT_S16_LE";
    data_config[10]="48000";
    data_config[11]="16";
    data_config[12]="No";          show_mixer = 0;
    data_config[13]="No";          conf_dock = FALSE;

    name_home = home_dir();
    name_home.append("/.qtradio/settings");
    {
        QFile file(name_home);
        if (file.exists() && file.open(QIODevice::ReadOnly))
        {
            QTextStream t( &file );
            while ( !t.atEnd() )
            {
                temp = t.readLine();
                if (temp == "") continue;
                if (temp.left(11) == "radio=/dev/")
                {
                    data_config[0] = temp.mid(6);
                    continue;
                }
                else
                if (temp.left(9) == "min_freq=")
                {
                    i=temp.mid(9).toInt(&ok);
                    if (ok && (i >= 87) && (i <= 108))
                    {
                        data_config[1].setNum(i);
                        conf_lower = i;
                    }
                    continue;
                }
                else
                if (temp.left(9) == "max_freq=")
                {
                    i=temp.mid(9).toInt(&ok);
                    if (ok && (i >= 87) && (i <= 108))
                    {
                        data_config[2].setNum(i);
                        conf_upper = i;
                    }
                    continue;
                }
                else
                if (temp.left(5) == "prec=")
                {
                    f=temp.mid(5).toFloat(&ok);
                    if (ok && (f >= 0.1))
                    {
                        i = f/0.1;
                        f = 0.1*i;
                        data_config[3].setNum(f);
                        conf_dok = f;
                    }
                    continue;
                }
                else
                if (temp.left(4) == "thr=")
                {
                    i=temp.mid(4).toInt(&ok);
                    if (ok && (i >= 1) && (i <= 8))
                    {
                        data_config[4].setNum(i);
                        conf_thr = i;
                    }
                    continue;
                }
                else
                if (temp.left(7) == "prefer=")
                {
                    if (temp.mid(7,6) == "Stereo")
                        data_config[5] = "Stereo";
                    else
                    if (temp.mid(7,4) == "Mono")
                        data_config[5] = "Mono";
                    else
                    if (temp.mid(7,9) == "Automatic")
                        data_config[5] = "Automatic";
                    continue;
                }
                else
                if (temp.left(6) == "mixer=")
                {
                    if (temp.mid(6,5) == "/dev/")
                        data_config[6] = temp.mid(6);
                    continue;
                }
                else
                if (temp.left(5) == "chan=")
                {
                    data_config[7] = temp.mid(5);
                    continue;
                }
                else
                if (temp.left(6) == "audio=")
                {
                    if (temp.mid(6,5) == "/dev/")
                        data_config[8] = temp.mid(6);
                    continue;
                }
                else
                if (temp.left(7) == "format=")
                {
                    data_config[9] = temp.mid(7);
                    continue;
                }
                else
                if (temp.left(5) == "rate=")
                {
                    i=temp.mid(5).toInt(&ok);
                    if (ok &&
                        ((i == 48000) || (i == 44100) || (i == 22050) || (i == 11025)))
                    {
                        data_config[10].setNum(i);
                        // conf_rate = i;
                    }
                    continue;
                }
                else
                if (temp.left(5) == "size=")
                {
                    i=temp.mid(5).toInt(&ok);
                    if (ok && ((i == 8) || (i == 16)))
                    {
                        data_config[11].setNum(i);
                        // conf_size = i;
                    }
                    continue;
                }
                else
                if (temp.left(4) == "exp=")
                {
                    //show_mixer sets main window in small or expanded version
                    if (temp.mid(4,3) == "Yes")
                    {
                        data_config[12] = tr("Yes");
                        show_mixer = 1;
                    }
                    else
                    if (temp.mid(4,2) == "No")
                    {
                        data_config[12] = tr("No");
                        show_mixer = 0;
                    }
                    continue;
                }
                else
                if (temp.left(5) == "dock=")
                {
                    if (temp.mid(5,3) == "Yes")
                    {
                        data_config[13] = tr("Yes");
                        conf_dock=TRUE;
                    }
                    else
                    if (temp.mid(5,2) == "No")
                    {
                        data_config[13] = tr("No");
                        conf_dock=FALSE;
                    }
                    continue;
                }
            }
            file.close();
        }
    }

    if (conf_upper < conf_lower)
    {
        conf_upper = conf_lower;
        data_config[2] = data_config[1];
    }

    conf_chan = 0;//default change vol
    for (i=0;i<SOUND_MIXER_NRDEVICES;i++)
    {
        if ( devices_table[i] == data_config[7]) conf_chan = i;
    }

    if (can_stereo() == 1) data_config[14] = tr("Yes");
    else data_config[14] = tr("No");
    if (can_rds() == 1) data_config[15] = tr("Yes");
    else data_config[15] = tr("No");

    name_home = home_dir();
    name_home.append("/.qtradio/skin");
    {
        QFile file(name_home);
        if (file.exists() && file.open(QIODevice::ReadOnly))
        {
            QTextStream t( &file );
            SkinDir = t.readLine();
            file.close();
        }
    }

    kseq[0]  = "F1";
    kseq[1]  = "F2";
    kseq[2]  = "K";
    kseq[3]  = "Ctrl+E";
    kseq[4]  = "+";
    kseq[5]  = "Up";
    kseq[6]  = "-";
    kseq[7]  = "Down";
    kseq[8]  = "Right";
    kseq[9]  = "P";
    kseq[10] = "Left";
    kseq[11] = "S";
    kseq[12] = "L";
    kseq[13] = "PgUp";
    kseq[14] = "PgDown";

    name_home = home_dir();
    name_home.append("/.qtradio/keys");
    {
        QFile file(name_home);
        if (file.exists() && file.open(QIODevice::ReadOnly))
        {
            QTextStream t( &file );
            while ( !t.atEnd() )
            {
                temp = t.readLine();
                if (temp == "") continue;
                if (temp.left(6) == "About=")
                {
                    kseq[0] = temp.mid(6);
                    continue;
                }
                else
                if (temp.left(9) == "About Qt=")
                {
                    kseq[1] = temp.mid(9);
                    continue;
                }
                else
                if (temp.left(6) == "Skins=")
                {
                    kseq[2] = temp.mid(6);
                    continue;
                }
                else
                if (temp.left(13) == "Exit QtRadio=")
                {
                    kseq[3] = temp.mid(13);
                    continue;
                }
                else
                if (temp.left(11) == "Freq +0.01=")
                {
                    kseq[4] = temp.mid(11);
                    continue;
                }
                else
                if (temp.left(10) == "Freq +0.1=")
                {
                    kseq[5] = temp.mid(10);
                    continue;
                }
                else
                if (temp.left(11) == "Freq -0.01=")
                {
                    kseq[6] = temp.mid(11);
                    continue;
                }
                else
                if (temp.left(10) == "Freq -0.1=")
                {
                    kseq[7] = temp.mid(10);
                    continue;
                }
                else
                if (temp.left(13) == "Next Station=")
                {
                    kseq[8] = temp.mid(13);
                    continue;
                }
                else
                if (temp.left(13) == "Power On/Off=")
                {
                    kseq[9] = temp.mid(13);
                    continue;
                }
                else
                if (temp.left(17) == "Previous Station=")
                {
                    kseq[10] = temp.mid(17);
                    continue;
                }
                else
                if (temp.left(9) == "Settings=")
                {
                    kseq[11] = temp.mid(9);
                    continue;
                }
                else
                if (temp.left(5) == "List=")
                {
                    kseq[12] = temp.mid(5);
                    continue;
                }
                else
                if (temp.left(6) == "Vol +=")
                {
                    kseq[13] = temp.mid(6);
                    continue;
                }
                else
                if (temp.left(6) == "Vol -=")
                {
                    kseq[14] = temp.mid(6);
                    continue;
                }
            }
            file.close();
        }
    }
}


void KDERadio::load_sound()
{
  //load current values of sound mixers

    vol[0] = get_volume(conf_chan);
    vol[1] = get_volume(0);
    vol[2] = get_volume(4);
    vol[3] = get_volume(5);
    vol[4] = get_volume(6);
    vol[5] = get_volume(7);
    vol[6] = get_volume(8);
    vol[7] = get_volume(22);
    vol[8] = get_volume(14);

    for (int i = 0; i <= 8; i++)
        setChannelBar(i);
}

// Helper functions
// Return value = TRUE if error happened

static QString lineBuffer;

static bool nextLine(QTextStream* t)
{
    if (t->atEnd())
    {
        fprintf(stderr, "No more lines!\n");
        return TRUE;
    }
    lineBuffer = t->readLine();
    if (lineBuffer.length() == 0)
    {
        fprintf(stderr, "Line should not be empty!\n");
        return TRUE;
    }
    else
        return FALSE;
}

static bool skipEmptyLine(QTextStream* t)
{
    if (t->atEnd())
    {
        fprintf(stderr, "No more lines!\n");
        return TRUE;
    }
    lineBuffer = t->readLine();
    if (lineBuffer.length() != 0)
    {
        fprintf(stderr, "Line \"%s\" should be empty!\n",
            lineBuffer.toLocal8Bit().data());
        return TRUE;
    }
    else
        return FALSE;
}

static bool takePrefix(const QString s)
{
    QString ss = s + " = ";
    if (ss != lineBuffer.left(ss.length()))
    {
        fprintf(stderr, "Line \"%s\" should start with \"%s\"!\n",
            lineBuffer.toLocal8Bit().data(),
            ss.toAscii().data());
        return TRUE;
    }
    lineBuffer.remove(0, ss.length());
    return FALSE;
}

static bool takeString(QString& s)
{
    s = lineBuffer;
    lineBuffer = "";
    return (s.length() == 0);
}

static bool takeInt(int& i)
{
    bool good;
    i = lineBuffer.toInt(&good);
    if (!good)
    {
        fprintf(stderr, "String \"%s\" cannot be converted to int!\n",
            lineBuffer.toLocal8Bit().data());
        lineBuffer = "";
        return TRUE;
    }
    lineBuffer = "";
    return FALSE;
}

static bool takeIntToComma(int& i)
{
    bool good;
    int k = lineBuffer.indexOf(',');
    if (k < 0)
    {
        fprintf(stderr, "No comma in the line \"%s\"!\n",
            lineBuffer.toLocal8Bit().data());
        lineBuffer = "";
        return TRUE;
    }
    QString ss = lineBuffer.left(k);
    i = ss.toInt(&good);
    lineBuffer.remove(0, ss.length()+1);
    if (!good)
    {
        fprintf(stderr, "String \"%s\" before a comma cannot be converted to int!\n",
            ss.toLocal8Bit().data());
        return TRUE;
    }
    return FALSE;
}


//THIS FUNCTION READS SKIN CONFIG FILE 
void KDERadio::load_skin()
{
    QString name, name_fb, name_home;
    QFile *file = NULL;
    QTextStream *t = NULL;
    QFont font;
    QString temp, temp1;
    QColor color;
    QImage img;
    int i, int1, int2, int3, int4;


    name = SkinDir; //skin's dir form config file
    name_fb = QString(share_dir()) + "/Skins/Red"; // fallback

    goto begin;

  fallback:

    QMessageBox::information( this, "QtRadio",(tr("Can't open skin \'")
        + name + tr("\' - see README in your sources")));
    if (name == name_fb)
    {
        KDERadio::close();
        return;
    }
    else
        name = name_fb;

  begin:

    name_home = name;
    name_home.append("/description");

    if (file) delete file;
    file = new QFile(name_home);
    file->open(QIODevice::ReadOnly);
    if ( ! file->exists() || ! file->isReadable())
        goto fallback;

    if (t) delete t;
    t = new QTextStream(file);

    if (nextLine(t) ||
        takePrefix("MAIN_WINDOW_SIZE_X") ||
        takeInt(NORMAL_SIZE_X)) goto fallback;

    if (nextLine(t) ||
        takePrefix("MAIN_WINDOW_SIZE_Y") ||
        takeInt(NORMAL_SIZE_Y)) goto fallback;

    if ( show_mixer == 0 ) KDERadio::resize( QSize(NORMAL_SIZE_X, NORMAL_SIZE_Y).expandedTo(minimumSizeHint()) );

    if (nextLine(t) ||
        takePrefix("PIXMAP") ||
        takeString(temp)) goto fallback;

    temp1 = name;
    temp1.append("/");
    temp1.append(temp);

    if (temp != "")
    {
        QPalette palette;
        palette.setBrush(KDERadio::backgroundRole(), QBrush(QPixmap(temp1)));
        KDERadio::setPalette(palette);
    }

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("EXP_WINDOW_SIZE_X") ||
        takeInt(EXP_SIZE_X)) goto fallback;

    if (nextLine(t) ||
        takePrefix("EXP_WINDOW_SIZE_Y") ||
        takeInt(EXP_SIZE_Y)) goto fallback;

    if ( show_mixer == 1 ) KDERadio::resize( QSize(EXP_SIZE_X, EXP_SIZE_Y).expandedTo(minimumSizeHint()) );

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("CLOSE_BUTTON_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("CLOSE_BUTTON_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("CLOSE_BUTTON_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("CLOSE_BUTTON_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Close_button->setGeometry(QRect(int1, int2, int3, int4));

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("MIN_BUTTON_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("MIN_BUTTON_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("MIN_BUTTON_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("MIN_BUTTON_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Min_button->setGeometry(QRect(int1, int2, int3, int4));

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("MOVE_BUTTON_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("MOVE_BUTTON_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("MOVE_BUTTON_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("MOVE_BUTTON_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Move_button->setGeometry(QRect(int1, int2, int3, int4));

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Freq_width = int3;
    // int4 is unused
    Freq_x = int1;
    Freq_y = int2;
    Freq_button->setGeometry(Freq_x, Freq_y, 11, 11 );

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Vol_height[0] = int3; // In fact it is width
    // int4 is unused
    Vol_x[0] = int1;
    Vol_y[0] = int2;
    Vol_button[0]->setGeometry(Vol_x[0], Vol_y[0], 11, 11 );

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_FIRST_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_FIRST_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_FIRST_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_FIRST_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    FreqDigit[0]->setGeometry(int1, int2, int3, int4);
    // FreqDigit[0]->setBackgroundOrigin(ParentOrigin);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_SECOND_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_SECOND_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_SECOND_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_SECOND_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    FreqDigit[1]->setGeometry(int1, int2, int3, int4);
    // FreqDigit[1]->setBackgroundOrigin(ParentOrigin);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_THIRD_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_THIRD_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_THIRD_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_THIRD_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    FreqDigit[2]->setGeometry(int1, int2, int3, int4);
    // FreqDigit[2]->setBackgroundOrigin(ParentOrigin);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_AFTER_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_AFTER_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_AFTER_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_AFTER_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    FreqDigit[3]->setGeometry(int1, int2, int3, int4);
    // FreqDigit[3]->setBackgroundOrigin(ParentOrigin);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_AFTER2_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_AFTER2_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_AFTER2_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_AFTER2_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    FreqDigit[4]->setGeometry(int1, int2, int3, int4);
    // FreqDigit[4]->setBackgroundOrigin(ParentOrigin);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_TEXT_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_TEXT_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_TEXT_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_TEXT_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Ticker->setGeometry(0,0,int3*2,int4);
    frame->setGeometry(QRect(int1-1,int2-1,int3+2,int4+2));

    if (nextLine(t) ||
        takePrefix("BACKGROUND_COLOR") ||
        takeString(temp)) goto fallback;
    color.setNamedColor(temp);
    Ticker->setPalette(QPalette(color));
    frame->setPalette(QPalette(color));

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_SIGNAL_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_SIGNAL_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_SIGNAL_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("LCD_SIGNAL_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Signal->setGeometry(int1,int2,int3,int4);

    if (nextLine(t) ||
        takePrefix("BACKGROUND_COLOR") ||
        takeString(temp)) goto fallback;
    color.setNamedColor(temp);
    Signal->setPalette(QPalette(color));

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("POWER_LED_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("POWER_LED_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("POWER_LED_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("POWER_LED_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Power->setGeometry(int1,int2,int3,int4);

    if (nextLine(t) ||
        takePrefix("BACKGROUND_COLOR") ||
        takeString(temp)) goto fallback;
    color.setNamedColor(temp);
    Power->setPalette(QPalette(color));

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("STEREO_LED_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("STEREO_LED_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("STEREO_LED_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("STEREO_LED_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Stereo->setGeometry(int1,int2,int3,int4);

    if (nextLine(t) ||
        takePrefix("BACKGROUND_COLOR") ||
        takeString(temp)) goto fallback;
    color.setNamedColor(temp);
    Stereo->setPalette(QPalette(color));

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_NEXT_STATION_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_NEXT_STATION_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_NEXT_STATION_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_NEXT_STATION_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Next_button->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_PREV_STATION_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_PREV_STATION_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_PREV_STATION_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_PREV_STATION_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Previous_button->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_LONG_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_LONG_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_LONG_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_LONG_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Forward_long->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_SHORT_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_SHORT_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_SHORT_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_SHORT_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Forward_short->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SUB_LONG_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SUB_LONG_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SUB_LONG_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SUB_LONG_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Backward_long->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SUB_SHORT_FREQ_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SUB_SHORT_FREQ_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SUB_SHORT_FREQ_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SUB_SHORT_FREQ_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Backward_short->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SEEK_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SEEK_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SEEK_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SEEK_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Seek_button->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_REC_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_REC_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_REC_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_REC_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Rec_button->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_MIXER_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_MIXER_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_MIXER_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_MIXER_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Mixer_button->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SETTINGS_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SETTINGS_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SETTINGS_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_SETTINGS_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Options_button->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_STATION_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_STATION_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_STATION_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_ADD_STATION_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Saver_button->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_MUTE_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_MUTE_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_MUTE_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_MUTE_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Sound_mute->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_POWER_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_POWER_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_POWER_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("BUTTON_POWER_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Switch_button->setGeometry(int1, int2, int3, int4);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_1_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_1_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_1_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_1_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Vol_bar[1]->setGeometry(int1, int2-int4, int3, int4);
    Vol_height[1] = int4-15;
    Vol_x[1] = int1+1;
    Vol_y[1] = int2-12;
    Vol_button[1]->setGeometry(Vol_x[1], Vol_y[1], 10, 10 );
    // Vol_bar[1]->setBackgroundOrigin(ParentOrigin);
    // Vol_button[1]->setBackgroundOrigin(ParentOrigin);


    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_2_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_2_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_2_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_2_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Vol_bar[2]->setGeometry(int1, int2-int4, int3, int4);
    Vol_height[2] = int4-15;
    Vol_x[2] = int1+1;
    Vol_y[2] = int2-12;
    Vol_button[2]->setGeometry(Vol_x[2], Vol_y[2], 10, 10 );
    // Vol_bar[1]->setBackgroundOrigin(ParentOrigin);
    // Vol_button[1]->setBackgroundOrigin(ParentOrigin);


    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_3_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_3_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_3_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_3_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Vol_bar[3]->setGeometry(int1, int2-int4, int3, int4);
    Vol_height[3] = int4-15;
    Vol_x[3] = int1+1;
    Vol_y[3] = int2-12;
    Vol_button[3]->setGeometry(Vol_x[3], Vol_y[3], 10, 10 );
    // Vol_bar[1]->setBackgroundOrigin(ParentOrigin);
    // Vol_button[1]->setBackgroundOrigin(ParentOrigin);


    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_4_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_4_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_4_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_4_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Vol_bar[4]->setGeometry(int1, int2-int4, int3, int4);
    Vol_height[4] = int4-15;
    Vol_x[4] = int1+1;
    Vol_y[4] = int2-12;
    Vol_button[4]->setGeometry(Vol_x[4], Vol_y[4], 10, 10 );
    // Vol_bar[1]->setBackgroundOrigin(ParentOrigin);
    // Vol_button[1]->setBackgroundOrigin(ParentOrigin);


    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_5_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_5_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_5_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_5_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Vol_bar[5]->setGeometry(int1, int2-int4, int3, int4);
    Vol_height[5] = int4-15;
    Vol_x[5] = int1+1;
    Vol_y[5] = int2-12;
    Vol_button[5]->setGeometry(Vol_x[5], Vol_y[5], 10, 10 );
    // Vol_bar[1]->setBackgroundOrigin(ParentOrigin);
    // Vol_button[1]->setBackgroundOrigin(ParentOrigin);


    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_6_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_6_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_6_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_6_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Vol_bar[6]->setGeometry(int1, int2-int4, int3, int4);
    Vol_height[6] = int4-15;
    Vol_x[6] = int1+1;
    Vol_y[6] = int2-12;
    Vol_button[6]->setGeometry(Vol_x[6], Vol_y[6], 10, 10 );
    // Vol_bar[1]->setBackgroundOrigin(ParentOrigin);
    // Vol_button[1]->setBackgroundOrigin(ParentOrigin);


    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_7_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_7_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_7_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_7_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Vol_bar[7]->setGeometry(int1, int2-int4, int3, int4);
    Vol_height[7] = int4-15;
    Vol_x[7] = int1+1;
    Vol_y[7] = int2-12;
    Vol_button[7]->setGeometry(Vol_x[7], Vol_y[7], 10, 10 );
    // Vol_bar[1]->setBackgroundOrigin(ParentOrigin);
    // Vol_button[1]->setBackgroundOrigin(ParentOrigin);


    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_8_POS_X") ||
        takeInt(int1)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_8_POS_Y") ||
        takeInt(int2)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_8_SIZE_X") ||
        takeInt(int3)) goto fallback;

    if (nextLine(t) ||
        takePrefix("SLIDER_VOL_8_SIZE_Y") ||
        takeInt(int4)) goto fallback;

    Vol_bar[8]->setGeometry(int1, int2-int4, int3, int4);
    Vol_height[8] = int4-15;
    Vol_x[8] = int1+1;
    Vol_y[8] = int2-12;
    Vol_button[8]->setGeometry(Vol_x[8], Vol_y[8], 10, 10 );
    // Vol_bar[1]->setBackgroundOrigin(ParentOrigin);
    // Vol_button[1]->setBackgroundOrigin(ParentOrigin);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("PIXMAP_VOLUME") ||
        takeString(temp)) goto fallback;

    temp1 = name;
    temp1.append("/");
    temp1.append(temp);

    Volume = new QImage(temp1);

    if (nextLine(t) ||
        takePrefix("PIXMAP_VOLUME_BAR_SIZE") ||
        takeIntToComma(int1) ||
        takeInt(int2)) goto fallback;
    vol_size_x = int1;
    vol_size_y = int2;

    if (nextLine(t) ||
        takePrefix("PIXMAP_VOLUME_BUTTON_SIZE") ||
        takeIntToComma(int1) ||
        takeInt(int2)) goto fallback;
    int sx = int1;
    int sy = int2;

    if (nextLine(t) ||
        takePrefix("PIXMAP_VOLUME_POS") ||
        takeIntToComma(int1) ||
        takeIntToComma(int2) ||
        takeInt(int3)) goto fallback;
    nexty = int1;
    int xpos = int2;
    int ypos = int3;

    // fprintf(stderr,"%i %i %i %i %i %i %i\n",vol_size_x,vol_size_y,nexty,xpos,ypos,sx,sy);

    img = Volume->copy(xpos,ypos,sx,sy);
    for (i=1; i<=8; i++)
        emit putImageLabel(img,Vol_button[i]);

    img = Volume->copy(0,0,vol_size_x,vol_size_y);
    for (i=1; i<=8; i++)
        emit putImageLabel(img,Vol_bar[i]);

    if (skipEmptyLine(t)) goto fallback;

    if (nextLine(t) ||
        takePrefix("PIXMAP_SLIDER_BUTTONS") ||
        takeString(temp)) goto fallback;

    temp1 = name;
    temp1.append("/");
    temp1.append(temp);
    Frequency = new QImage(temp1);

    if (nextLine(t) ||
        takePrefix("PIXMAP_SLIDER_BUTTONS_SIZES") ||
        takeIntToComma(int1) ||
        takeIntToComma(int2) ||
        takeIntToComma(int3) ||
        takeInt(int4)) goto fallback;
    int x1 = int1;
    int y1 = int2;
    int x2 = int3;
    int y2 = int4;

    img = Frequency->copy(0,0,x1,y1);
    emit putImageLabel(img, Freq_button);

    img = Frequency->copy(x1+1,0,x2,y2);
    emit putImageLabel(img, Vol_button[0]);

    if (skipEmptyLine(t)) goto fallback;

    //numbers
    if (nextLine(t) ||
        takePrefix("PIXMAP_NUMBER") ||
        takeString(temp)) goto fallback;

    temp1 = name;
    temp1.append("/");
    temp1.append(temp);
    Number = new QImage(temp1);

    if (nextLine(t) ||
        takePrefix("PIXMAP_NUMBER_SIZE") ||
        takeIntToComma(int1) ||
        takeInt(int2)) goto fallback;
    int w = int1;
    int h = int2;

    for (int i=0; i<10; i++)
    {
        img = Number->copy((w+1)*i,0,w,h);
        emit putImagePix(img, NumPixmap[i]);
    }

    if (skipEmptyLine(t)) goto fallback;

    //led buttons
    if (nextLine(t) ||
        takePrefix("PIXMAP_LED") ||
        takeString(temp)) goto fallback;

    temp1 = name;
    temp1.append("/");
    temp1.append(temp);
    Led = new QImage(temp1);

    if (nextLine(t) ||
        takePrefix("PIXMAP_LED_SIZES") ||
        takeIntToComma(int1) ||
        takeIntToComma(int2) ||
        takeIntToComma(int3) ||
        takeInt(int4)) goto fallback;
    x1 = int1;
    y1 = int2;
    x2 = int3;
    y2 = int4;

    img = Led->copy(0,0,x1,y1);
    emit putImagePix(img, stereo_on_pix);
    img = Led->copy(x1+2,0,x1,y1);
    emit putImagePix(img, stereo_off_pix);
    img = Led->copy(x1*2+4,0,x2,y2);
    emit putImagePix(img, power_on_pix);
    img = Led->copy(x1*2+6+x2,0,x2,y2);
    emit putImagePix(img, power_off_pix);

    if (skipEmptyLine(t)) goto fallback;

    //signal strength
    if (nextLine(t) ||
        takePrefix("PIXMAP_SIGNAL") ||
        takeString(temp)) goto fallback;

    temp1 = name;
    temp1.append("/");
    temp1.append(temp);
    Signal_Led = new QImage(temp1);

    if (nextLine(t) ||
        takePrefix("PIXMAP_SIGNAL_SIZE") ||
        takeIntToComma(int1) ||
        takeInt(int2)) goto fallback;
    x1 = int1;
    y1 = int2;


    for (i=0; i<=8; i++)
    {
        img = Signal_Led->copy(0,(y1+1)*(8-i),x1,y1);
        emit putImagePix(img, SignalPixmap[i]);
    }

    if (skipEmptyLine(t)) goto fallback;

    //text
    if (nextLine(t) ||
        takePrefix("PIXMAP_TEXT") ||
        takeString(temp)) goto fallback;

    temp1 = name;
    temp1.append("/");
    temp1.append(temp);
    rtext = new QImage(temp1);

    if (nextLine(t) ||
        takePrefix("PIXMAP_TEXT_FONT_SIZE") ||
        takeIntToComma(int1) ||
        takeInt(int2)) goto fallback;
    ticker_font_w = int1;
    ticker_font_h = int2;

    if (skipEmptyLine(t)) goto fallback;

    //main buttons
    if (nextLine(t) ||
        takePrefix("PIXMAP_MAIN_BUTTON") ||
        takeString(temp)) goto fallback;
    temp1 = name;
    temp1.append("/");
    temp1.append(temp);
    Main_button = new QImage(temp1);

    if (nextLine(t) ||
        takePrefix("PIXMAP_MAIN_BUTTON_SIZE") ||
        takeIntToComma(int1) ||
        takeInt(int2)) goto fallback;
    w = int1;
    h = int2;

    img = Main_button->copy(0,0,w,h);
    emit putImageButton(img,Backward_long);
    img = Main_button->copy((w),0,w,h);
    emit putImageButton(img,Backward_short);
    img = Main_button->copy((w+1)*2,0,w,h);
    emit putImageButton(img,Forward_long);
    img = Main_button->copy((w+1)*3,0,w,h);
    emit putImageButton(img,Forward_short);
    img = Main_button->copy((w+1)*4,0,w,h);
    emit putImageButton(img,Seek_button);
    img = Main_button->copy((w+1)*5,0,w,h);
    emit putImageButton(img,Sound_mute);
    img = Main_button->copy((w+1)*6,0,w,h);
    emit putImageButton(img,Rec_button);
    img = Main_button->copy((w+1)*7,0,w,h);
    emit putImageButton(img,Options_button);
    img = Main_button->copy((w+1)*8,0,w,h);
    emit putImageButton(img,Switch_button);
    img = Main_button->copy((w+1)*9,0,w,h);
    emit putImageButton(img,Mixer_button);
    img = Main_button->copy((w+1)*10,0,w,h);
    emit putImageButton(img,Previous_button);
    img = Main_button->copy((w+1)*11,0,w,h);
    emit putImageButton(img,Next_button);
    img = Main_button->copy((w+1)*12,0,w,h);
    emit putImageButton(img,Saver_button);

    if (skipEmptyLine(t)) goto fallback;

    //window buttons
    if (nextLine(t) ||
        takePrefix("PIXMAP_WINDOW_BUTTON") ||
        takeString(temp)) goto fallback;

    temp1 = name;
    temp1.append("/");
    temp1.append(temp);

    Window_button = new QImage(temp1);

    if (nextLine(t) ||
        takePrefix("PIXMAP_WINDOW_BUTTON_SIZE") ||
        takeIntToComma(int1) ||
        takeInt(int2)) goto fallback;
    w = int1;
    h = int2;

    img = Window_button->copy(0,0,w,h);
    emit putImageButton(img,Move_button);
    img = Window_button->copy((w+1),0,w,h);
    emit putImageButton(img,Min_button);
    img = Window_button->copy((w+1)*2,0,w,h);
    emit putImageButton(img,Close_button);

    file->close();

    Power->setPixmap(*power_off_pix);
    Stereo->setPixmap(*stereo_off_pix);
    Signal->setPixmap(*SignalPixmap[0]);
    FreqDigit[0]->setPixmap(*NumPixmap[0]);
    FreqDigit[1]->setPixmap(*NumPixmap[8]);
    FreqDigit[2]->setPixmap(*NumPixmap[8]);
    FreqDigit[3]->setPixmap(*NumPixmap[0]);
    FreqDigit[4]->setPixmap(*NumPixmap[0]);
}


// find station for frequency
QTreeWidgetItem* KDERadio::closestStation(float f)
{
    if (f == 0)
        return NULL;

    QTreeWidgetItem* item = ad->Station_list->topLevelItem(0);
    QTreeWidgetItem* prev = NULL;

    if (item == NULL)
        return NULL;

    if (f < conf_lower)
        f = conf_lower;
    else
    if (f > conf_upper)
        f = conf_upper;

    float below = 0;
    float above;

    while (item != NULL)
    {
        above = item->text(1).toFloat();
        if (above < f)
        {
            below = above;
            prev = item;
            item = ad->Station_list->itemBelow(item);
            continue;
        }
        else
        {
            if (((f - below) < (above - f)) && prev)
                return prev;
            else
                return item;
        }
    }
    return prev;
}

// show about window
void KDERadio::on_about()
{
    if (ab == NULL)
        ab = new About;

    ab->exec();
}

// show info window
void KDERadio::start_info(int page)
{
    if (show_settings)
    {
        if ((page>=0) && (page<=2))
        {
            inf->tabWidget2->setCurrentIndex(page);
            inf->update();
        }
        return;
    }

    if (inf == NULL)
        inf = new Settings;

    show_settings = 1;

    QLabel *lab;
    QLineEdit *ld;
    QComboBox *comb;
    QSpinBox *sp;
    QTreeWidget *tw;
    QTreeWidgetItem *item;

    int i;
    int where;
    const char *devices_table[] = SOUND_DEVICE_NAMES;
    const char **nm;

//  Tuner capabilities

    lab = inf->textLabel2;
    lab->setText("Video4Linux2");

    if (can_stereo() == 1) data_config[14] = tr("Yes");
    else data_config[14] = tr("No");

    if (can_rds() == 1) data_config[15] = tr("Yes");
    else data_config[15] = tr("No");

    lab = inf->textLabel4;
    lab->setText(data_config[14]);

    lab = inf->textLabel6;
    lab->setText(data_config[15]);

//  Radio device

    comb = inf->comboBox13;
    comb->clear();
    nm = &radio_names[0];
    for ( ; *nm; nm++ ) {
        if (access(*nm, F_OK))  continue;
        if (access(*nm, R_OK | W_OK))
        {
            fprintf(stderr,
                "Insufficient rights for \'%s\', please verify!\n", *nm);
            continue;
        }
        comb->insertItem( comb->count(), *nm );
        if ( data_config[0] == *nm )
            comb->setCurrentIndex( comb->count()-1 );
    }
    if ( comb->count() == 0 )
    {
        fprintf(stderr, "No radio devices found!\n");
        comb->insertItem( 0, tr("none") );
        comb->setCurrentIndex( 0 );
        // Exit ??
    }

    comb = inf->comboBox11;
    if ( data_config[5] == "Stereo" )
        comb->setCurrentIndex(1);
    else
    if ( data_config[5] == "Mono" )
        comb->setCurrentIndex(2);
    else
        comb->setCurrentIndex(0);

    sp = inf->spinBox1;
    sp->setValue( (data_config[1]).toInt() );

    sp = inf->spinBox2;
    sp->setValue( (data_config[2]).toInt() );

// Mixer properties

    comb = inf->comboBox12;
    comb->clear();
    nm = &mixer_names[0];
    for ( ; *nm; nm++ ) {
        if (access(*nm, F_OK))  continue;
        if (access(*nm, R_OK | W_OK))
        {
            fprintf(stderr,
                "Insufficient rights for \'%s\', please verify!\n", *nm);
            continue;
        }
        comb->insertItem( comb->count(), *nm );
        if ( data_config[6] == *nm )
            comb->setCurrentIndex( comb->count()-1 );
    }
    if ( comb->count() == 0 )
    {
        fprintf(stderr, "No mixer devices found!\n");
        comb->insertItem( 0, tr("none") );
        comb->setCurrentIndex( 0 );
        // Exit ??
    }

    comb = inf->comboBox7;
    comb->clear();
    for (i=0, where = 0; i<SOUND_MIXER_NRDEVICES; i++)
    {
        if (mixer_has_channel(i))
        {
            comb->insertItem(where,devices_table[i]);
            if ( devices_table[i] == data_config[7])
                //combo shows actual chosen label of mixer
                comb->setCurrentIndex(where);
            where++;
        }
    }

//  Main window

    comb = inf->comboBox9;
    if ( data_config[12] == (tr("Yes")) )
        comb->setCurrentIndex(0);
    else comb->setCurrentIndex(1);

    comb = inf->comboBox8;
    if ( data_config[13] == (tr("Yes")) )
        comb->setCurrentIndex(0);
    else comb->setCurrentIndex(1);

//  Seeking

    ld = inf->lineEdit6;
    ld->setText( QString::number(conf_dok,'f',2) );

    comb = inf->comboBox10;
    if (conf_thr == 1) comb->setCurrentIndex(0);
    if (conf_thr == 2) comb->setCurrentIndex(1);
    if (conf_thr == 3) comb->setCurrentIndex(2);
    if (conf_thr == 4) comb->setCurrentIndex(3);
    if (conf_thr == 5) comb->setCurrentIndex(4);
    if (conf_thr == 6) comb->setCurrentIndex(5);
    if (conf_thr == 7) comb->setCurrentIndex(6);
    if (conf_thr == 8) comb->setCurrentIndex(7);

//  Etc

    names[0] = tr("About");
    names[1] = tr("About Qt");
    names[2] = tr("Choose Skin");
    names[3] = tr("Exit");
    names[4] = tr("Frequency + 0.01 MHz");
    names[5] = tr("Frequency + 0.1 MHz");
    names[6] = tr("Frequency - 0.01 MHz");
    names[7] = tr("Frequency - 0.1 MHz");
    names[8] = tr("Next Station");
    names[9] = tr("Power");
    names[10] = tr("Previous Station");
    names[11] = tr("Settings");
    names[12] = tr("Show Stations List");
    names[13] = tr("Volume Down");
    names[14] = tr("Volume Up");

    tw = inf->listView2;
    tw->clear();
    for (i=0;i<=14;i++)
    {
        item = new QTreeWidgetItem(tw);
        item->setText(1, kseq[i]);
        item->setText(0,names[i]);
    }

    tw = inf->listView1;
    tw->clear();
    tw->setSortingEnabled(FALSE);

    QString nps;
    QDir d;

    d.setFilter( QDir::Dirs );
    d.setSorting( QDir::Name);
    d.setPath(QString(share_dir()) + "/Skins/");
    QFileInfoList list = d.entryInfoList();
    QFileInfoList::iterator it = list.begin();
    while ( it != list.end() )
    {
        nps = it->fileName();
        if ( ( nps != "/.." ) && ( nps != "/.") && !( nps =="." ) && !( nps ==".." ) )
        {
            item = new QTreeWidgetItem(tw);
            item->setText(0, nps);
            nps.prepend("/Skins/");
            nps.prepend(share_dir());
            if (nps == SkinDir)
                item->setSelected(TRUE);
            else
                item->setSelected(FALSE);
            item->setText(1,nps);
        }
        ++it;
    }

    nps = home_dir();
    nps.append("/.qtradio/Skins");
    d.setPath(nps);
    list = d.entryInfoList();
    QFileInfoList::iterator it2 = list.begin();
    while ( it2 != list.end() )
    {
        nps = it2->fileName();
        if ( ( nps != "/.." ) && ( nps != "/.") && !( nps =="." ) && !( nps ==".." ) )
        {
            item = new QTreeWidgetItem(tw);
            item->setText(0, nps);
            nps.prepend("/.qtradio/Skins/");
            nps.prepend(home_dir());
            if (nps == SkinDir)
                item->setSelected(TRUE);
            else
                item->setSelected(FALSE);
            item->setText(1,nps);
        }
        ++it2;
    }

    if ((page>=0) && (page<=2))
        inf->tabWidget2->setCurrentIndex(page);

    inf->exec();
}

// auto-seeking stations
void KDERadio::on_seeking()
{
    if (!power_on)
    {
        QMessageBox::information( this, "QtRadio",
                    tr("Turn on radio to start seeking!"));
        return;
    }
    if (seek_on)
    {
        seek_on = 0;
    }
    else
    {
        fprintf(stderr, "Seek started\n");
        Seeker->start();
    }
}


// show seeking result
void KDERadio::show_found()
{
    if (!power_on)
        return;

    if (seek_on)
        fprintf(stderr, "Seek ended, %d stations found\n", Seeker->found);
    seek_on = 0;

    QString nps = tr("I've found ");
    nps.append(QString::number(Seeker->found));
    nps.append(tr(" stations"));
    if (Seeker->found)
        nps.append(tr("\nSee and save stations list!"));
    QMessageBox::information( this, "QtRadio",(nps));
    Seeker->found = 0;
}


//  record
void KDERadio::on_record()
{
    //QMessageBox::information( this, "QtRadio",(tr("Recording is not implemented yet!")));
    if (show_recording)
        return;
    show_recording = 1;

    if (rec == NULL)
        rec = new Recording;
    QLineEdit *l;
    QComboBox *c;

    QFileInfo file;

    QString s;
    int i = 0;

    s = home_dir();
    s.append("/");

    if (Ticker->station_name != "")
        s.append(Ticker->station_name);
    else
        s.append("qtradio");

    s.append("_");

    QDateTime data_time;
    data_time = data_time.currentDateTime ();
    s.append( data_time.toString ("dd.MM.yyyy_hh.mm.ss") );

    s.append(".wav");

    //if file exists add value to its end
    file.setFile(s);
    if ( file.exists() == TRUE)
    {
        do
        {
            s.insert(s.length()-4,QString::number(i));
            i++;
            printf("%i\n",i);
            file.setFile(s);
        }    while ( file.exists() == TRUE );
    }

    l = rec->lineEdit1;
    l->setText(s);
    l = rec->lineEdit2;
    l->setText("500");

    c = rec->comboBox3;
    c->clear();
    const char **nm = &dsp_names[0];
    for ( ; *nm; nm++ ) {
        if (access(*nm, F_OK))  continue;
        if (access(*nm, R_OK | W_OK)) // Write needed to set play format
        {
            fprintf(stderr,
                "Insufficient rights for \'%s\', please verify!\n", *nm);
            continue;
        }
        c->insertItem( c->count(), *nm );
        if ( data_config[8] == *nm )
            c->setCurrentIndex( c->count()-1 );
    }
    if ( c->count() == 0 )
    {
        fprintf(stderr, "No audio devices found!\n");
        c->insertItem( 0, tr("none") );
        c->setCurrentIndex( 0 );
        // Exit ??
    }

    rec->exec();
}

void KDERadio::next_station()
{
    QTreeWidgetItem *item = closestStation(freq);
    if (item == NULL)
        return;

    item = ad->Station_list->itemBelow(item);
    if (item == NULL)
        item = ad->Station_list->topLevelItem(0);

    set_freq(item->text(1).toFloat());

    update_panel();
}

void KDERadio::prev_station()
{
    QTreeWidgetItem *item = closestStation(freq);
    if (item == NULL)
        return;

    QTreeWidget *list = ad->Station_list;
    item = list->itemAbove(item);
    if (item == NULL)
        item = list->topLevelItem(list->topLevelItemCount()-1);

    set_freq(item->text(1).toFloat());

    update_panel();
}

void KDERadio::aboutQt()
{
    QMessageBox::aboutQt( this, (tr("About Qt")) );
}

void KDERadio::create_menu()
{
    style->clear();
    style_dock->clear();

    QAction *act;

    act = new QAction(tr("Skins"), this);
    act->setShortcut(QKeySequence(kseq[2]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(show_skin()));
    style->addAction(act);
    this->addAction(act);

    act = new QAction(tr("List"), this);
    act->setShortcut(QKeySequence(kseq[12]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(edit_stations()));
    style->addAction(act);
    this->addAction(act);

    act = new QAction(tr("Settings"), this);
    act->setShortcut(QKeySequence(kseq[11]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(on_info()));
    style->addAction(act);
    this->addAction(act);

    act = new QAction(tr("About"), this);
    act->setShortcut(QKeySequence(kseq[0]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(on_about()));
    style->addAction(act);
    this->addAction(act);

//

    act = new QAction(tr("Power On/Off"), this);
    act->setShortcut(QKeySequence(kseq[9]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(Power_S()));
    style_dock->addAction(act);
    this->addAction(act);

    act = new QAction(tr("Next Station"), this);
    act->setShortcut(QKeySequence(kseq[8]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(next_station()));
    style_dock->addAction(act);
    this->addAction(act);

    act = new QAction(tr("Previous Station"), this);
    act->setShortcut(QKeySequence(kseq[10]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(prev_station()));
    style_dock->addAction(act);
    this->addAction(act);

    act = new QAction(tr("Freq +0.1"), this);
    act->setShortcut(QKeySequence(kseq[5]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(on_Forward()));
    style_dock->addAction(act);
    this->addAction(act);

    act = new QAction(tr("Freq +0.01"), this);
    act->setShortcut(QKeySequence(kseq[4]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(short_Forward()));
    style_dock->addAction(act);
    this->addAction(act);

    act = new QAction(tr("Freq -0.1"), this);
    act->setShortcut(QKeySequence(kseq[7]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(on_Forward()));
    style_dock->addAction(act);
    this->addAction(act);

    act = new QAction(tr("Freq -0.01"), this);
    act->setShortcut(QKeySequence(kseq[6]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(short_Forward()));
    style_dock->addAction(act);
    this->addAction(act);

    act = new QAction(tr("Vol +"), this);
    act->setShortcut(QKeySequence(kseq[14]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(volume_up()));
    style_dock->addAction(act);
    this->addAction(act);

    act = new QAction(tr("Vol -"), this);
    act->setShortcut(QKeySequence(kseq[13]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(volume_down()));
    style_dock->addAction(act);
    this->addAction(act);

//

    act = new QAction(tr("About Qt"), this);
    act->setShortcut(QKeySequence(kseq[1]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(aboutQt()));
    style->addAction(act);
    style_dock->addAction(act);
    this->addAction(act);

    style->addSeparator();
    style_dock->addSeparator();

    act = new QAction(tr("Exit QtRadio"), this);
    act->setShortcut(QKeySequence(kseq[3]));
    act->setShortcutContext(Qt::ApplicationShortcut);
    connect(act,SIGNAL(activated()),this,SLOT(on_exit()));
    style->addAction(act);
    style_dock->addAction(act);
    this->addAction(act);

    // style->setStyle(tr("Default"));
    // style_dock->setStyle(tr("Default"));
}



void KDERadio::execute_lirc_command (char *cmd)
{
  #ifdef HAVE_LIRC
    if (strcasecmp (cmd, "freq up") == 0)
    {
        short_Forward();
    }
    else if (strcasecmp (cmd, "freq down") == 0)
    {
        short_Backward();
    }
    else if (strcasecmp (cmd, "volume up") == 0)
    {
        volume_up();
    }
    else if (strcasecmp (cmd, "volume down") == 0)
    {
        volume_down();
    }
    else if (strcasecmp (cmd, "mute") == 0)
    {
        mute_pressed();
    }
    else if (strcasecmp (cmd, "tv") == 0)
    {
        ;
    }
    else if (strcasecmp (cmd, "quit") == 0)
    {
        on_exit();
    }
    else if (strcasecmp (cmd, "channel up") == 0)
    {
        next_station();
    }
    else if (strcasecmp (cmd, "channel down") == 0)
    {
        prev_station();
    }
    else if (strcasecmp (cmd, "seeking") == 0)
    {
        on_seeking();
    }
    else if (strcasecmp (cmd, "mixer") == 0)
    {
        on_mixer();
    }
    else if (strcasecmp (cmd, "record") == 0)
    {
        on_record();
    }
    else if (strcasecmp (cmd, "power") == 0)
    {
        Switcher();
    }
    else if (strcasecmp (cmd, "mouse mode") == 0)
    {
      /*
      is it useful???
      if ( sockfd == 1) lirc_deinit();
      else
        initialize_lirc();
       */
    }
    else
    {
        fprintf (stderr, "Unknown LIRC command: %s\n", cmd);
    }
  #endif
}

int KDERadio::initialize_lirc()
{
  #ifdef HAVE_LIRC
    printf("Initializing LIRC\n");
    if ((lirc_fd = lirc_init((char*)("QtRadio"), 0)) <=0)
    {
        perror("lirc_init");
        return 0;
    }

    if (lirc_readconfig (NULL, &config, NULL) != 0)
    {
        perror("lirc_readconfig");
        lirc_deinit ();
        lirc_fd = -1;
        return 0;
    }

    fcntl (lirc_fd, F_SETFL, O_NONBLOCK);
    fcntl (lirc_fd, F_SETFD, FD_CLOEXEC);
    fprintf(stderr, "LIRC init successful\n");
    //sockfd = 1;
    return 1;
  #endif
}

void KDERadio::deinitialize_lirc()
{
  #ifdef HAVE_LIRC
    if (lirc_fd <= 0)
        return;

    fprintf(stderr,"Shutting down LIRC\n");
    //sn->setEnabled(FALSE);
    TimerL->stop();
    //sockfd = 0;
    lirc_freeconfig(config);
    lirc_deinit();
  #endif
}

void KDERadio::dataReceived()
{
  #ifdef HAVE_LIRC
    char *code, *cmd;
    int ret ;
    fprintf(stderr,"Data received\n");
    while (lirc_nextcode (&code) == 0 )
    {
        if (code==NULL) return;
        if (((ret=lirc_code2char (config, code, &cmd)) == 0) && (cmd != NULL))
        {
            fprintf(stderr,"LIRC command: %s\n", cmd);
            execute_lirc_command(cmd);
        }
        else
        {
            /*
            // it works...I know it doesn't look good...
            printf("Lirc error occured\n");
            sn->setEnabled(FALSE);
            lirc_freeconfig (config);
            config = NULL;
            lirc_deinit ();
             */

            free (code);
        }
    }


  /* previous unstable:
  while (lirc_nextcode (&code) == 0 )
  {
    if (code==NULL) continue;

    while (ret=lirc_code2char (config, code, &cmd) == 0 && cmd != NULL)
    {
        printf("lirc command: %s\n", cmd);
        execute_lirc_command(cmd);
    }

    free (code);
    //if (ret ==-1) break;
  }

  // on LIRC error, shutdown added input
  if (ret == -1)
  {
     printf("Lirc error occured\n");
     //sn->setEnabled(FALSE);
     lirc_freeconfig (config);
     config = NULL;
     lirc_deinit ();
  }
  */
  #endif
}

void KDERadio::start_lirc()
{
  #ifdef HAVE_LIRC
    if (lirc_fd<0)
    {
        printf("Lirc not initialized\n");
        return;
    }

    TimerL = new QTimer( this);
    connect( TimerL, SIGNAL(timeout()),this, SLOT(dataReceived()) );
    TimerL->start(1);

    /* sn = new QSocketNotifier( sockfd, QSocketNotifier::Write, this );
    connect( sn, SIGNAL(activated(int)),this, SLOT(dataReceived()) );
    sn->setEnabled(TRUE);  */
  #endif

}

QSeeker::QSeeker(KDERadio* R, Add* A) : QThread(R)
{
    r = R;
    a = A;
    found = 0;
}

void QSeeker::run()
{
    seek_on = 1;

    float tmp, tmpp;
    tmp = conf_lower;
    int sig, str;
    QString nps;

    r->load_settings();//for sure if new values appeared
    r->save_start();
    found = 0;
    do
    {
        // stopped
        if (!seek_on)
            break;

        set_frequency(tmp);
        fprintf(stderr, "Scanning freq = %.2f\n", tmp);
        sleep(1);
        nps.setNum(tmp, 'f', 2);
        a->Name_edit->setText(tr("Scanning..."));
        a->Freq_edit->setText(nps);
        str = signal_strength();

        //fprintf(stderr,"SIGNAL = %i for Freq = %.2f\n", str, tmp);
        if (str >= conf_thr )
        {
            tmp -= conf_dok;
            tmp += 0.01;
            if (tmp < conf_lower)
                tmp = conf_lower;
            tmpp = tmp;
            sig = 0;
            do
            {
                // stopped
                if (!seek_on)
                    break;

                set_frequency(tmpp);
                fprintf(stderr, "Adjusting freq = %.2f\n", tmpp);
                sleep(1);
                nps.setNum(tmpp, 'f', 2);
                a->Name_edit->setText(tr("Adjusting..."));
                a->Freq_edit->setText(nps);
                str = signal_strength();
                if (str > sig)
                {
                    sig = str;
                    tmp = tmpp;
                    //fprintf(stderr,"Better Freq = %.2f, SIGNAL = %i \n", tmpp, sig);
                }
                else if (str < sig)
                {
                    break;
                }
                tmpp += 0.01;
            } while (tmpp <= conf_upper) ;

            // stopped
            if (!seek_on)
                break;

            found++;
            tmp = (tmp + tmpp - 0.01)/2;
            tmp = (long)(100*tmp)*0.01;
            fprintf(stderr,"SAVING Freq = %.2f, SIGNAL = %i\n", tmp, sig);
            nps.setNum(tmp, 'f', 2);
            a->Name_edit->setText(tr("Unknown"));
            a->Freq_edit->setText(nps);
            a->Add_to_list();
            tmp = tmpp;
        }
        tmp += conf_dok;
    } while (tmp <= conf_upper);

    a->Name_edit->setText("");
    a->Freq_edit->setText("");

    r->load_start();

    this->quit();
}
