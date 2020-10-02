#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QThread>
#include "playthread.h"
#include <iostream>
#include <csignal>


class Dialog;

class PlayThread : public QThread
{
    Q_OBJECT
public:
    PlayThread(){
        _mode = "WFM";
    }

    void tstart(Dialog* pd){
        _pd = pd;
        this->start();
    }
    void stop();
    void change(const char* mode);
    double acord(double f);
protected:
    void run();

private:
    Dialog* _pd;
    std::string _mode;
    double      _freq = 92.500e6;
    bool        _runing=false;
};

#endif // PLAYTHREAD_H
