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

extern "C"
{
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/soundcard.h>
}

#include "recording.h"

#include <qvariant.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qfileinfo.h>
#include <qsocketnotifier.h>
#include <qframe.h>
#include <qprogressbar.h>

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
    14 = stereo capable
    15 = rds capable
 */

extern int show_recording;

extern QString FontName[];
extern int     FontSize[];

static QSocketNotifier *sn;

int samplesize;
int channels;

static int device_fd = -1;
int seconds = 0;
int sec = 0;

int sample_frequency = 48000;

static int sockfd = 1;
static int wav = -1;
static int fd = -1;
int l;
int count, c;
int yet = 0;

int abuf_size;
void *audiobuf ;

int close_device(int m)
{
    return close(m);
}

int open_device(char *device_name, int open_flags)
{
    struct stat device_info;
    int audio_fd;

    if (stat(device_name,&device_info) == -1)
    {
        fprintf(stderr, "ERROR : device <%s> doesn't exist\n",device_name);
        return -1;
    }

    if ( (device_info.st_mode & S_IFCHR) != S_IFCHR)
    {
        fprintf(stderr, "ERROR : device <%s> isn't devicefile\n",device_name);
        return -1;
    }

    audio_fd = open(device_name, open_flags);

    if (audio_fd == -1)
    {
        int reason_for_failure = errno;
        fprintf(stderr, "ERROR : device <%s> couldn't be opened with flags %d \n",device_name,open_flags);
        fprintf(stderr, "ERROR : This is the reason <%s> \n",strerror(reason_for_failure));
    }

    return audio_fd;
}



int set_play_format(int device_fd, int play_format)
{
    int new_play_format = play_format;

    if (ioctl(device_fd, SNDCTL_DSP_SETFMT, &new_play_format) == -1)
    {
        return -1;
    }

    return new_play_format;
}

int set_channels(int device_fd, int channels)
{
    int new_channels = channels;

    if (ioctl(device_fd,SNDCTL_DSP_CHANNELS, &new_channels) == -1)
    {
        if (channels <= 2)
        {
            new_channels = channels -1;

            if (ioctl(device_fd,SNDCTL_DSP_CHANNELS, &new_channels) == -1)
                return -1;

            return new_channels+1;
        }
        else
            return -1;
    }

    return new_channels;
}

int set_sample_frequency(int device_fd, int sample_frequency)
{
    int new_sample_frequency = sample_frequency;

    if(ioctl(device_fd, SNDCTL_DSP_SPEED, &new_sample_frequency) == -1)
    {
        return -1;
    }

    return new_sample_frequency;
}


int reset_device(int device_fd)
{
    if ( ioctl(device_fd, SNDCTL_DSP_RESET, 0) == -1)
        return -1;

    return 1;
}

int set_blk_size(int device_fd)
{

    int frag;

    frag = 0x7fff000d; /* 8k */
    if (-1 == ioctl(device_fd, SNDCTL_DSP_SETFRAGMENT, &frag))
        perror("ioctl SNDCTL_DSP_SETFRAGMENT");

    ioctl (device_fd, SNDCTL_DSP_GETBLKSIZE, &abuf_size);
    if ((audiobuf = malloc (abuf_size)) == NULL)
    {
        fprintf (stderr, "Unable to allocate input/output buffer\n");
        exit (-1);
    }

    if (0 == abuf_size)
        abuf_size = 4096;
    audiobuf = malloc(abuf_size);
    return(0);
}


int set_stereo(int device_fd, int dsp_stereo)
{

    if (ioctl (device_fd, SNDCTL_DSP_STEREO, &dsp_stereo)==-1)
    {
        fprintf (stderr, "Unable to set mono/stereo\n");
        exit (-1);
    }
    return(0);
}

int set_sample_size(int device_fd, int samplesize)
{

    int  tmp = samplesize;
    ioctl(device_fd, SNDCTL_DSP_SAMPLESIZE, &samplesize);
    if (tmp != samplesize)
    {
        fprintf(stderr, "Unable to set the sample size\n");
        return -1;
        exit(-1);
    }
    return(0);

}


/* ---------------------------------------------------------------------- */
/* *.wav I/O stolen from cdda2wav */
/* I've got it from xawtv tools by Gerd Knoor */

/* Copyright (C) by Heiko Eissfeldt */

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned long  FOURCC;	/* a four character code */

/* flags for 'wFormatTag' field of WAVEFORMAT */
#define WAVE_FORMAT_PCM 1

/* MMIO macros */
#define mmioFOURCC(ch0, ch1, ch2, ch3) \
  ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) | \
  ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24))

#define FOURCC_RIFF	mmioFOURCC ('R', 'I', 'F', 'F')
#define FOURCC_LIST	mmioFOURCC ('L', 'I', 'S', 'T')
#define FOURCC_WAVE	mmioFOURCC ('W', 'A', 'V', 'E')
#define FOURCC_FMT	mmioFOURCC ('f', 'm', 't', ' ')
#define FOURCC_DATA	mmioFOURCC ('d', 'a', 't', 'a')

typedef struct CHUNKHDR {
    FOURCC ckid;		/* chunk ID */
    DWORD dwSize; 	        /* chunk size */
} CHUNKHDR;

/* simplified Header for standard WAV files */
typedef struct WAVEHDR {
    CHUNKHDR chkRiff;
    FOURCC fccWave;
    CHUNKHDR chkFmt;
    WORD wFormatTag;	   /* format type */
    WORD nChannels;	   /* number of channels (i.e. mono, stereo, etc.) */
    DWORD nSamplesPerSec;  /* sample rate */
    DWORD nAvgBytesPerSec; /* for buffer estimation */
    WORD nBlockAlign;	   /* block size of data */
    WORD wBitsPerSample;
    CHUNKHDR chkData;
} WAVEHDR;

#define IS_STD_WAV_HEADER(waveHdr) ( \
  waveHdr.chkRiff.ckid == FOURCC_RIFF && \
  waveHdr.fccWave == FOURCC_WAVE && \
  waveHdr.chkFmt.ckid == FOURCC_FMT && \
  waveHdr.chkData.ckid == FOURCC_DATA && \
  waveHdr.wFormatTag == WAVE_FORMAT_PCM)

#define cpu_to_le32(x) (x)
#define cpu_to_le16(x) (x)
#define le32_to_cpu(x) (x)
#define le16_to_cpu(x) (x)

/* -------------------------------------------------------------------- */

static WAVEHDR  fileheader;
static int      wav_size;

void
wav_init_header()
{
    /* stolen from cdda2wav */
    int nBitsPerSample = samplesize;
    //channels = 2;
    int rate = sample_frequency;

    unsigned long nBlockAlign = channels * ((nBitsPerSample + 7) / 8);
    unsigned long nAvgBytesPerSec = nBlockAlign * rate;
    unsigned long temp = /* data length */ 0 +
        sizeof(WAVEHDR) - sizeof(CHUNKHDR);

    fileheader.chkRiff.ckid    = cpu_to_le32(FOURCC_RIFF);
    fileheader.fccWave         = cpu_to_le32(FOURCC_WAVE);
    fileheader.chkFmt.ckid     = cpu_to_le32(FOURCC_FMT);
    fileheader.chkFmt.dwSize   = cpu_to_le32(16);
    fileheader.wFormatTag      = cpu_to_le16(WAVE_FORMAT_PCM);
    fileheader.nChannels       = cpu_to_le16(channels);
    fileheader.nSamplesPerSec  = cpu_to_le32(rate);
    fileheader.nAvgBytesPerSec = cpu_to_le32(nAvgBytesPerSec);
    fileheader.nBlockAlign     = cpu_to_le16(nBlockAlign);
    fileheader.wBitsPerSample  = cpu_to_le16(nBitsPerSample);
    fileheader.chkData.ckid    = cpu_to_le32(FOURCC_DATA);
    fileheader.chkRiff.dwSize  = cpu_to_le32(temp);
    fileheader.chkData.dwSize  = cpu_to_le32(0 /* data length */);
}

void
wav_start_write(int fd)
{
    wav_init_header();
    lseek(fd,0,SEEK_SET);
    write(fd,&fileheader,sizeof(WAVEHDR));
    wav_size = 0;
}

int
wav_write_audio(int fd, void *data, int len)
{
    int rc;

    rc = write(fd,data,len);
    if (len == rc) {
        wav_size += len;
        return 0;
    }
    else
        return -1;
}

void
wav_stop_write(int fd)
{
    unsigned long temp = wav_size + sizeof(WAVEHDR) - sizeof(CHUNKHDR);

    fileheader.chkRiff.dwSize = cpu_to_le32(temp);
    fileheader.chkData.dwSize = cpu_to_le32(wav_size);
    lseek(fd,0,SEEK_SET);
    write(fd,&fileheader,sizeof(WAVEHDR));
}

/* -------------------------------------------------------------------- */

/*
 *  Constructs a Recording as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Recording::Recording( QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl )

{
    textLabel1 = new QLabel( this );
    textLabel1->setGeometry( QRect( 20, 10, 110, 20 ) );
    textLabel1->setFrameShape( QLabel::NoFrame );
    textLabel1->setFrameShadow( QLabel::Plain );

    comboBox2 = new QComboBox( this );
    comboBox2->setGeometry( QRect( 140, 10, 160, 20 ) );

    textLabel2 = new QLabel( this );
    textLabel2->setGeometry( QRect( 20, 40, 110, 20 ) );

    comboBox3 = new QComboBox( this );
    comboBox3->setGeometry( QRect( 140, 40, 160, 20 ) );

    textLabel3 = new QLabel( this );
    textLabel3->setGeometry( QRect( 20, 70, 110, 20 ) );

    textLabel4 = new QLabel( this );
    textLabel4->setGeometry( QRect( 20, 100, 110, 20 ) );

    comboBox1 = new QComboBox( this );
    comboBox1->setGeometry( QRect( 140, 100, 160, 20 ) );

    line1_2 = new QFrame( this );
    line1_2->setGeometry( QRect( 0, 190, 331, 5 ) );
    line1_2->setFrameShape( QFrame::HLine );
    line1_2->setFrameShadow( QFrame::Sunken );
    line1_2->setFrameShape( QFrame::HLine );

    textLabel7 = new QLabel( this );
    textLabel7->setGeometry( QRect( 20, 230, 61, 20 ) );

    line1 = new QFrame( this );
    line1->setGeometry( QRect( 0, 260, 331, 5 ) );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );

    textLabel6 = new QLabel( this );
    textLabel6->setGeometry( QRect( 20, 200, 110, 20 ) );

    comboBox5 = new QComboBox( this );
    comboBox5->setGeometry( QRect( 140, 160, 160, 20 ) );

    textLabel5 = new QLabel( this );
    textLabel5->setGeometry( QRect( 20, 160, 110, 20 ) );

    textLabel1_2 = new QLabel( this );
    textLabel1_2->setGeometry( QRect( 20, 130, 110, 20 ) );

    lineEdit1 = new QLineEdit( this );
    lineEdit1->setGeometry( QRect( 100, 230, 220, 20 ) );

    comboBox6 = new QComboBox( this );
    comboBox6->setGeometry( QRect( 140, 130, 160, 20 ) );

    line1_3 = new QFrame( this );
    line1_3->setGeometry( QRect( 0, 380, 331, 5 ) );
    line1_3->setFrameShape( QFrame::HLine );
    line1_3->setFrameShadow( QFrame::Sunken );
    line1_3->setFrameShape( QFrame::HLine );

    progressBar1 = new QProgressBar( this );
    progressBar1->setGeometry( QRect( 20, 330, 280, 20 ) );
    progressBar1->setValue( 0 );

    QColor color1(255, 0, 0);
    QColor color2(190, 190, 190);
    QPalette palette;
    palette.setBrush(QPalette::WindowText, QBrush(color1));
    palette.setBrush(QPalette::Window, QBrush(color2));

    textLabel2_2 = new QLabel( this );
    textLabel2_2->setGeometry( QRect( 20, 350, 280, 20 ) );
    textLabel2_2->setPalette(palette);

    textLabel8 = new QLabel( this );
    textLabel8->setGeometry( QRect( 20, 270, 170, 20 ) );

    textLabel9 = new QLabel( this );
    textLabel9->setGeometry( QRect( 20, 300, 170, 20 ) );

    textLabel10 = new QLabel( this );
    textLabel10->setGeometry( QRect( 200, 270, 100, 20 ) );
    textLabel10->setPalette(palette);

    textLabel11 = new QLabel( this );
    textLabel11->setGeometry( QRect( 200, 300, 100, 20 ) );
    textLabel11->setPalette(palette);

    toolButton1 = new QToolButton( this );
    toolButton1->setGeometry( QRect( 20, 390, 90, 24 ) );

    toolButton2 = new QToolButton( this );
    toolButton2->setGeometry( QRect( 120, 390, 90, 24 ) );

    toolButton3 = new QToolButton( this );
    toolButton3->setGeometry( QRect( 220, 390, 90, 24 ) );

    comboBox4 = new QComboBox( this );
    comboBox4->setGeometry( QRect( 140, 70, 160, 20 ) );

    lineEdit2 = new QLineEdit( this );
    lineEdit2->setGeometry( QRect( 140, 200, 106, 20 ) );
    languageChange();
    resize( QSize(324, 422).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( toolButton1, SIGNAL( clicked() ), this, SLOT( Start_recording() ) );
    connect( toolButton2, SIGNAL( clicked() ), this, SLOT( Stop_recording() ) );
    connect( toolButton3, SIGNAL( clicked() ), this, SLOT( Quit_recording() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Recording::~Recording()
{
    // no need to delete child widgets, Qt does it all for us
    // fprintf(stderr, "Recording deleted\n");
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Recording::languageChange()
{
    QFont font(FontName[4], FontSize[4]);
    QString tmp;
    int i;

    setWindowTitle( tr( "QtRadio - Recording" ) );
    textLabel1->setText( tr( "File Type:" ) );
    textLabel1->setFont(font);
    textLabel2->setText( tr( "Sound Device:" ) );
    textLabel2->setFont(font);
    textLabel3->setText( tr( "Audio Format:" ) );
    textLabel3->setFont(font);
    textLabel4->setText( tr( "Sampling Rate:" ) );
    textLabel4->setFont(font);
    textLabel1_2->setText( tr( "Sampling Size:" ) );
    textLabel1_2->setFont(font);
    textLabel5->setText( tr( "Channels:" ) );
    textLabel5->setFont(font);

    comboBox1->clear();
    comboBox1->insertItem( 0, tr( "48000" ) );
    comboBox1->insertItem( 1, tr( "44100" ) );
    comboBox1->insertItem( 2, tr( "22050" ) );
    comboBox1->insertItem( 3, tr( "11025" ) );
    comboBox1->setFont(font);
    i = 0;
    if      (data_config[10] == tr( "44100" )) i = 1;
    else if (data_config[10] == tr( "22050" )) i = 2;
    else if (data_config[10] == tr( "11025" )) i = 3;
    else data_config[10] = tr("48000");
    comboBox1->setCurrentIndex(i);

    comboBox5->clear();
    comboBox5->insertItem( 0, tr( "Stereo" ) );
    comboBox5->insertItem( 1, tr( "Mono" ) );
    comboBox5->setFont(font);

    comboBox6->clear();
    comboBox6->insertItem( 0, tr( "16" ) );
    comboBox6->insertItem( 1, tr( "8" ) );
    comboBox6->setFont(font);
    i = 0;
    if (data_config[11] == tr( "8" )) i = 1;
    else data_config[11] = tr( "16" );
    comboBox6->setCurrentIndex(i);

    comboBox4->clear();
    comboBox4->insertItem( 0, tr( "AFMT_S16_LE" ) );
    comboBox4->insertItem( 1, tr( "AFMT_U8" ) );
    comboBox4->insertItem( 2, tr( "AFMT_S16_BE" ) );
    comboBox4->insertItem( 3, tr( "AFMT_S16_NE" ) );
    comboBox4->insertItem( 4, tr( "AFMT_S8" ) );
    comboBox4->insertItem( 5, tr( "AFMT_U16_LE" ) );
    comboBox4->insertItem( 6, tr( "AFMT_S16_BE" ) );
    comboBox4->insertItem( 7, tr( "AFMT_QUERY" ) );
    comboBox4->insertItem( 8, tr( "AFMT_MU_LAW" ) );
    comboBox4->insertItem( 9, tr( "AFMT_A_LAW" ) );
    comboBox4->insertItem( 10, tr( "AFMT_IMA_ADPCM" ) );
    comboBox4->setFont(font);
    i = 0;
    if      (data_config[9] == tr( "AFMT_U8" ))     i = 1;
    else if (data_config[9] == tr( "AFMT_S16_BE" )) i = 2;
    else if (data_config[9] == tr( "AFMT_S16_NE" )) i = 3;
    else if (data_config[9] == tr( "AFMT_S8" ))     i = 4;
    else if (data_config[9] == tr( "AFMT_U16_LE" )) i = 5;
    else if (data_config[9] == tr( "AFMT_S16_BE" )) i = 6;
    else if (data_config[9] == tr( "AFMT_QUERY" ))  i = 7;
    else if (data_config[9] == tr( "AFMT_MU_LAW" )) i = 8;
    else if (data_config[9] == tr( "AFMT_A_LAW" ))  i = 9;
    else if (data_config[9] == tr( "AFMT_IMA_ADPCM" )) i = 10;
    else data_config[9] = tr( "AFMT_S16_LE" );
    comboBox4->setCurrentIndex(i);

    comboBox3->clear();

    comboBox2->clear();
    comboBox2->insertItem( 0, tr( "wav" ) );
    comboBox2->insertItem( 1, tr( "none" ) );
    comboBox2->setFont(font);

    textLabel9->setText( tr( "Current Size [bytes] :" ) );
    textLabel9->setFont(font);
    textLabel8->setText( tr( "Elapsed Time [sec] :" ) );
    textLabel8 ->setFont(font);
    textLabel7->setText( tr( "Filename:" ) );
    textLabel7->setFont(font);
    textLabel11->setText( QString::null );
    textLabel11->setFont(font);
    textLabel10->setText( QString::null );
    textLabel10->setFont(font);
    textLabel6->setText( tr( "Time [sec] :" ) );
    textLabel6->setFont(font);
    textLabel2_2->setFont(font);
    toolButton1->setText( tr( "Start" ) );
    toolButton1->setFont(font);
    toolButton2->setText( tr( "Stop" ) );
    toolButton2->setFont(font);
    toolButton3->setText( tr( "Exit" ) );
    toolButton3->setFont(font);

    lineEdit1->setFont(font);
    lineEdit2->setFont(font);
}

inline int sysopen(const char *pathname, int flags, mode_t mode)
{
    return open(pathname, flags, mode);
}

void Recording::Start_recording()
{

    char name[150];
    strncpy(&name[0], lineEdit1->text().toLocal8Bit().data(), 149);

    char device[30];
    strncpy(&device[0], comboBox3->currentText().toLocal8Bit().data(), 29);
    data_config[8] = comboBox3->currentText();

    int dsp_stereo = 0;

    if (comboBox5->currentText() == tr("Stereo"))
    {
        channels = 2;
        dsp_stereo = 1;
    }
    else
    {
        channels = 1;
        dsp_stereo = 0;
    }

    if (comboBox6->currentText() == tr("16"))
    {
        samplesize = 16;
        data_config[11] = tr("16");
    }
    else
    {
        samplesize = 8;
        data_config[11] = tr("8");
    }

    int play_format = AFMT_S16_LE;
    if (comboBox4->currentText() == "AFMT_U8")
        play_format =  AFMT_U8;
    if (comboBox4->currentText() == "AFMT_QUERY")
        play_format =  AFMT_QUERY;
    if (comboBox4->currentText() == "AFMT_MU_LAW")
        play_format =  AFMT_MU_LAW;
    if (comboBox4->currentText() == "AFMT_IMA_ADPCM")
        play_format =  AFMT_IMA_ADPCM;
    if (comboBox4->currentText() == "AFMT_S16_LE")
        play_format =  AFMT_S16_LE;
    if (comboBox4->currentText() == "AFMT_S16_BE")
        play_format =  AFMT_S16_BE;
    if (comboBox4->currentText() == "AFMT_S16_NE")
        play_format =  AFMT_S16_NE;
    if (comboBox4->currentText() == "AFMT_U16_LE")
        play_format =  AFMT_U16_LE;
    if (comboBox4->currentText() == "AFMT_U16_BE")
        play_format =  AFMT_U16_BE;
    data_config[9] = comboBox4->currentText();

    sample_frequency = 48000;
    if (comboBox1->currentText() == "48000")
        sample_frequency = 48000;
    if (comboBox1->currentText() == "44100")
        sample_frequency = 44100;
    if (comboBox1->currentText() == "22050")
        sample_frequency = 22050;
    if (comboBox1->currentText() == "11025")
        sample_frequency = 11025;
    data_config[10] = comboBox1->currentText();


    device_fd = open_device(&device[0], O_RDONLY);

    if (device_fd == -1)
    {
        QMessageBox::information( this, "QtRadio", tr("Can't open audio device!"));
        return;
    }

    bool ok;
    seconds = lineEdit2->text().toInt(&ok,10);
    if (ok == FALSE)
    {
      seconds = 500;
      lineEdit2->setText("500");
    }

    //samplesize = comboBox6->currentText().toInt();

    if ( set_play_format(device_fd, play_format) == -1)
    {
        QMessageBox::information( this, "QtRadio",tr("Can't set play format!"));
        return;
    }
    if ( set_channels(device_fd, channels) == -1)
    {
        QMessageBox::information( this, "QtRadio",tr("Can't set channel mode!"));
        return;
    }
    if ( set_sample_frequency(device_fd, sample_frequency) == -1)
    {
        QMessageBox::information( this, "QtRadio",tr("Can't set sample rate!"));
        return;
    }
    if ( set_stereo(device_fd, dsp_stereo) == -1)
    {
        QMessageBox::information( this, "QtRadio",tr("Can't set stereo!"));
        return;
    }
    if ( set_sample_size(device_fd, samplesize) == -1)
    {
        QMessageBox::information( this, "QtRadio",tr("Can't set sample size!"));
        return;
    }

    set_blk_size(device_fd);

    QFileInfo file(name);
    if  ( file.exists() )
    {
        QMessageBox::warning( this, "QtRadio",tr("Can't record : File exists!"));
        close_device(device_fd);
        return;
    }

    if ( comboBox2->currentText() == "wav")
    {
        wav = sysopen(&name[0], O_RDWR | O_EXCL | O_CREAT, 0666);
        fprintf(stderr,"CREATED WAV FILE\n");
        sec = 0;
        wav_start_write(wav);
    }
    else //recording none format
    {
        if (!name[0])
        {
            fd = 1;
            strcpy(&name[0], "stdout");
        }
        else
        {
            if ((fd = sysopen (&name[0], O_RDWR | O_EXCL | O_CREAT, 0666)) == -1)
            {
                perror (name);
                exit (-1); // Why ?!!!
            }
        }

        if (!seconds)
            count = 0x7fffffff;
        else
        {
            count = seconds * sample_frequency;
            if (dsp_stereo)
                count *= 4;
            if (samplesize != 8)
                count *= 4;
        }
    }

    QString s;
    s = "Recording ";
    //s.append(name);
    s.append("in progress");
    textLabel2_2->setText(s);

    toolButton1->setEnabled(FALSE);

    sn = new QSocketNotifier( sockfd, QSocketNotifier::Write, this );
    connect( sn, SIGNAL(activated(int)),this, SLOT(Status()) );
    sn->setEnabled(TRUE);

}

void Recording::Stop_recording()
{
    if ( wav != -1 )
    {
        sn->setEnabled(FALSE);
        wav_stop_write(wav);
        close_device(wav);
        reset_device(device_fd);
        close_device(device_fd);
        wav = -1;
        device_fd = -1;
        textLabel2_2->setText("Recording finished");
        toolButton1->setEnabled(TRUE);
    }

    if ( fd != -1)
    {
        sn->setEnabled(FALSE);
        fprintf(stderr,"END RECORDING NONE TYPE FILE\n");
        close_device(fd);
        reset_device(device_fd);
        close_device(device_fd);
        fd = -1;
        device_fd = -1;
        textLabel2_2->setText("Recording finished");
        toolButton1->setEnabled(TRUE);
    }
}



void Recording::Quit_recording()
{
    if (wav != -1)
    {
        QMessageBox::warning( this, "QtRadio","Recording WAV in progress - Stop it first!");
        return;
    }

    if (fd != -1)
    {
        QMessageBox::warning( this, "QtRadio","Recording in progress - Stop it first!");
        return;
    }

    if (device_fd != -1)
    {
        fprintf(stderr,"Trying to close device : ");
        reset_device(device_fd);
        if ( close_device(device_fd) != 0)
            fprintf(stderr,"Error when closing device\n");
        else
            fprintf(stderr,"success\n");
    }

    show_recording = 0;
    hide();
}

void Recording::Status()
{
  if ( comboBox2->currentText() == "wav")
  {

    if (sec < seconds)
    {
        if (abuf_size != read(device_fd,audiobuf,abuf_size))
        {
            perror("read /dev/dsp*");
            exit(1);
        }
        wav_write_audio(wav, audiobuf, abuf_size);

        if (channels == 2)
            sec = wav_size / (sample_frequency*4);
        else
            sec = wav_size / (sample_frequency*2);
        progressBar1->setValue(100*((1.0*sec)/seconds));
        textLabel10->setText( QString::number(sec) );
        textLabel11->setText( QString::number(wav_size) );
    }
    else
    {
        sn->setEnabled(FALSE);
        fprintf(stderr,"END RECORDING WAV FILE\n");
        wav_stop_write(wav);
        close_device(wav);
        wav = -1;
        reset_device(device_fd);
        close_device(device_fd);
        device_fd = -1;
        textLabel2_2->setText("Recording finished");
        toolButton1->setEnabled(TRUE);
    }
  }

  else
  {
    if (count)
    {
        c = count;
        if (c > abuf_size)
        c = abuf_size;

        if ((l = read (device_fd, audiobuf, c)) > 0)
        {
            if (write (fd, audiobuf, l) != l)
            {
                fprintf(stderr,"Write error\n");
                exit (-1);
            }
            yet = yet + c;
            printf("%i %i %i %i \n",l,c,yet,abuf_size);

            if (channels == 2)
                sec = yet / (sample_frequency*4);
            else
                sec = yet / (sample_frequency*2);

            progressBar1->setValue(100*((1.0*sec)/seconds));
            textLabel10->setText( QString::number(sec) );
            textLabel11->setText( QString::number(yet) );
            count -= l;
        }

        if (l == -1)
        {
            fprintf (stderr,"Device error\n");
            exit (-1);
        }
    }
    else
    {
        sn->setEnabled(FALSE);
        fprintf(stderr,"END RECORDING NONE TYPE FILE\n");
        close_device(fd);
        fd = -1;
        reset_device(device_fd);
        close_device(device_fd);
        device_fd = -1;
        textLabel2_2->setText("Recording finished");
        toolButton1->setEnabled(TRUE);
    }

  }

}

/*
 *  process key events
 */
void Recording::keyPressEvent( QKeyEvent *k )
{
    if (k->key() == Qt::Key_Escape)
        Recording::Quit_recording();
}

