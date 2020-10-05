
#include <QDebug>
#include <unistd.h>
#include "dialog.h"
#include "ui_dialog.h"
#include "sdrmain.h"


Dialog* PDLG;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    _thr = new PlayThread();
    PDLG = this;
}

Dialog::~Dialog()
{
    _thr->stop();
    delete ui;
    delete _thr;
}

void Dialog::run()
{
    ui->comboBox->setCurrentIndex(0);
    int f;

   // for(f=80;f<120;f+=20)
   //     ui->comboBox_2->addItem(QString::number(f)+" Mhz");
    _startf = 80000000;
    _endf   = 114000000;
    _stepf  = 300000;

    _thr->tstart(this);
    sleep(1);
    _thr->change((const char*)ui->comboBox->currentText().toUtf8());

}

void Dialog::on_horizontalSlider_sliderMoved(int position)
{

}

void Dialog::on_horizontalSlider_sliderReleased()
{

}

void Dialog::on_horizontalSlider_valueChanged(int value)
{

}

void Dialog::apply_freq(int s)
{
    _thr->acord(s);
    char d[32];
    sprintf(d,"%3.6f",(float)s/1000000.0);
    ui->fr->setText(d);
}


void Dialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QStringList m = arg1.split(" ");
    _startf = m.at(1).toFloat()*1000000.0f;
    _endf = m.at(2).toFloat()*1000000.0f;
    _stepf = m.at(3).toFloat()*1000000.0f;
    apply_freq(_startf);

    _gains.clear();
}


void Dialog::timerEvent(QTimerEvent *event)
{
    char l[32]={0};
    int gain = scan_freq(_startf);

    l[0]=0;
    if(gain==0)
    {
        _gains.push_back(_startf);
        ui->openGLWidget->addGain(gain, l, _startf);
    }
    else
    {
        if(_gains.size()>1)
        {
            int mid = _gains.size()/2;
            ::sprintf(l,"%d Hz", _gains[0]);
            ui->fr->setText(l);
            _gains.clear();
            ui->openGLWidget->addGain(gain, l, _gains[0]);
        }else {
            ui->openGLWidget->addGain(gain, l, _startf);
        }
    }

    ui->openGLWidget->repaint();
    _startf += _stepf;
    if(_startf > _endf)
    {
        _timer.stop();
        _gains.clear();
        _startf = 80000000;
        _endf   = 114000000;
        _stepf  = 100000;
    }
}

void Dialog::on_pushButton_clicked()
{
    if(!_timer.isActive())
    {
        _gains.clear();
        int samples = (_endf-_startf) / _stepf;
        ui->openGLWidget->setWidth(samples);
        _startf = 80000000;
        _endf   = 114000000;
        _stepf  = 100000;
        _timer.start(256,this);
    }
}

void Dialog::on_verticalSlider_sliderMoved(int position)
{

}

void Dialog::on_enter_returnPressed()
{
    int s = ui->enter->text().toFloat()*1000000.0f;
    apply_freq(s);
}

void Dialog::on_enter_editingFinished()
{
    int s = ui->enter->text().toFloat()*1000000.0f;
    apply_freq(s);
}
