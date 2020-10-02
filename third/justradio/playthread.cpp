

#include <QDebug>
#include "playthread.h"
#include "sdrmain.h"


void PlayThread::run()
{
    qDebug() << " RUNNING:" << _freq << "MHZ";
    ::vmain(_mode.c_str(),_freq,200000,48000);
}

void PlayThread::stop()
{
    qDebug() << "PRE-BREAK";
    ::break_palyer();
    qDebug() << "POST-BREAK";
}

double PlayThread::acord(double f)
{
    qDebug() << "ACCORD " << f;
    _freq=f;
    ::set_freq(f);
    return f;
}

void PlayThread::change(const char* mode)
{
    qDebug() << "CHANGE " << mode;
    _mode=mode;
    ::set_freq(_freq);
    set_mod(_mode.c_str());

}
