/***************************************************************************
                          qtradio.h  -  description
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

#ifndef QTRADIO_H
#define QTRADIO_H

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

// include files for QT
#include <qvariant.h>
#include <qwidget.h>
#include <qstring.h>
#include <qmutex.h>
#include <qstring.h>
#include <qevent.h>
#include <qlabel.h>
#include <qthread.h>

// application specific includes
#include "info.h"
#include "home.h"
#include "add.h"
#include "about.h"
#include "recording.h"

class QImage;
class QLineEdit;
class QToolButton;
class QFrame;
class QTreeWidgetItem;

class QTicker : public QLabel
{
//  QOBJECT
  public:
    int przes;
    int dlug;
    int pass;
    int up;
    int off;
    float freq;

    QString station_name;

    QImage *part[200];

    void newText();
    void init(float);
    QTicker(QWidget*);

  protected:
    virtual void paintEvent(QPaintEvent*);
};

class KDERadio;


class QSeeker : public QThread
{
  Q_OBJECT
  public:
    int found;

    void run();
    QSeeker(KDERadio*, Add*);

  private:
    KDERadio *r;
    Add *a;
};


class KDERadio : public QWidget
{
  Q_OBJECT
  public:
    /** constructor */
    KDERadio(QWidget* parent=0);
//    KDERadio(QWidget* parent=0, Qt::WFlags f=0);
    /** destructor */
    ~KDERadio();

    bool power_on;
    int strength;
    float freq;

    int force; /* 0 = auto, 1 = mono, 2 = stereo */

    QMutex mutex; /* for graphic operations */

    QFrame *frame;

    QMenu *style;
    QMenu *style_dock;

    QFrame* line1;
    QFrame* line1_2;
    QFrame* line2;
    QFrame* line3;

    QTicker *Ticker;
    QSeeker *Seeker;

    QLabel *FreqDigit[5];

    QLabel* Power;
    QLabel* Stereo;
    QLabel* Signal;

    QToolButton* Saver_button;
    QToolButton* Mixer_button;
    QToolButton* Rec_button;
    QToolButton* Options_button;
    QToolButton* Seek_button;
    QToolButton* Forward_long;
    QToolButton* Forward_short;
    QToolButton* Backward_long;
    QToolButton* Backward_short;
    QToolButton* Sound_mute;
    QToolButton* Switch_button;
    QToolButton* Previous_button;
    QToolButton* Next_button;
    QToolButton* Move_button;
    QToolButton* Min_button;
    QToolButton* Close_button;

    QLabel *Freq_button;

    QLabel *Vol_button[9];

    QLabel* Vol_bar[9];

    QString Yes();

    QTreeWidgetItem* closestStation(float);

    public slots:
    void nice_text();
    void Switcher();
    void load_sound();
    void load_skin();
    void load_settings();
    void load_start();
    void Power_S();
    void getImageButton(const QImage&, QToolButton*);
    void getImageLabel(const QImage&, QLabel*);
    void getImagePix(const QImage&, QPixmap*);

    signals:
    void putImageButton(const QImage&, QToolButton*);
    void putImageLabel(const QImage&, QLabel*);
    void putImagePix(const QImage&, QPixmap*);

    private slots:
    void set_freq(float);
    void freq_from_slider();
    void on_Forward();
    void on_Backward();
    void short_Backward();
    void short_Forward();
    void setting_leds_signal(); 
    void setting_leds_freq();

    void vol_from_slider();
    void volume_up();
    void volume_down();
    void mute_pressed();

    void update_panel();
    void on_record();
    void on_mixer();
    void on_about();
    void on_exit();
    void on_info();
    void on_seeking();

    void next_station();
    void prev_station();
    void edit_stations();
    void show_found();

    void close_main();
    void min_main();
    void move_main();
    void aboutQt();
    void show_skin();
    void save_start();
    void start_info(int);
    void create_menu();

    void execute_lirc_command (char *cmd);
    int  initialize_lirc();
    void deinitialize_lirc();
    void dataReceived();
    void start_lirc();


   protected:
    void closeEvent( QCloseEvent* );
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void setChannelBar(int);

    friend class QSeeker;
};



extern KDERadio *Radio;
extern float freq;
extern QString station_name;


#endif

