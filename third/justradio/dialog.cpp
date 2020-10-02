
#include <QDebug>
#include <unistd.h>
#include "dialog.h"
#include "ui_dialog.h"
#include "sdrmain.h"



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    _thr = new PlayThread();
}

Dialog::~Dialog()
{
    _thr->stop();
    delete ui;
    delete _thr;
}

void Dialog::run()
{
    ui->horizontalSlider->setRange(80000,120000);
    ui->horizontalSlider->setTickInterval(1000);
    ui->horizontalSlider->setValue(92500);
    ui->comboBox->setCurrentIndex(0);
    int f;

   // for(f=80;f<120;f+=20)
   //     ui->comboBox_2->addItem(QString::number(f)+" Mhz");

    _thr->tstart(this);
    sleep(1);
    _thr->change((const char*)ui->comboBox->currentText().toUtf8());
    //on_pushButton_clicked();
}



void Dialog::on_pushButton_9_clicked()
{
    int s = ui->lineEdit->text().toInt();
    _applyfreq(s);
}

void Dialog::on_horizontalSlider_sliderMoved(int position)
{
    _applyfreq(ui->horizontalSlider->value());
}

void Dialog::on_horizontalSlider_sliderReleased()
{

}

void Dialog::on_horizontalSlider_valueChanged(int value)
{
    char d[32];
    sprintf(d,"%d",value);
    ui->lineEdit->setText(d);
    int s = ui->horizontalSlider->value();
    _applyfreq(s);
}

void Dialog::on_lineEdit_returnPressed()
{
    on_pushButton_9_clicked();
}

void Dialog::_applyfreq(int s)
{
    int hz = s * 1000;
    _thr->acord(hz);
    char d[32];
    sprintf(d,"%dHz  %2.5fMhz", hz, s/1000.0);
    ui->fr->setText(d);
}



void Dialog::on_radioButton_clicked(bool checked)
{
    if(checked==false)return;

    ui->horizontalSlider->setRange(80000,120000);
    ui->horizontalSlider->setTickInterval(1000);
    ui->horizontalSlider->setValue(92500);
    _applyfreq(ui->horizontalSlider->value()*1000);
    ui->horizontalSlider->setValue(ui->horizontalSlider->value());

    char d[32];
    sprintf(d,"%d",ui->horizontalSlider->value());
    ui->lineEdit->setText(d);
}

void Dialog::on_radioButton_2_clicked(bool checked)
{
    if(checked==false)return;

    ui->horizontalSlider->setRange(200,1800);
    ui->horizontalSlider->setTickInterval(100);
    ui->horizontalSlider->setValue(680);
    _applyfreq(ui->horizontalSlider->value()*1000);
    ui->horizontalSlider->setValue(ui->horizontalSlider->value());
    char d[32];
    sprintf(d,"%d",ui->horizontalSlider->value());
    ui->lineEdit->setText(d);
}

void Dialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    //QString m =
    _thr->change((const char*)ui->comboBox->currentText().toUtf8());

}

void Dialog::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBox->count())
    {
        QStringList ql = arg1.split(" ");
        int f = ql.at(0).toInt();
        int fe;
        if(ql.at(1)=="Khz"){
            fe = f + 100;
        }
        else {
            f *= 1000;
            fe = f + 10 * 1000;
        }

        ui->horizontalSlider->setRange(f,fe);
        ui->horizontalSlider->setTickInterval(100);
        ui->horizontalSlider->setValue((f+fe)/2);
    }
}

void Dialog::on_pushButton_clicked()
{
    QString arg1 = ui->comboBox_2->currentText();
    QStringList ql = arg1.split(" ");

    ui->listWidget->clear();

    int* posts = scan_freq(76000,110000);
    int* pi = posts;
    while(*pi++)
    {
        if(*pi)
        {
            int fk = *pi/1000;
            ui->listWidget->addItem(QString::number(fk));
        }
    }
    ui->comboBox_2->removeItem(ui->comboBox_2->currentIndex());
}

void Dialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int freq = item->text().toInt();

    ui->horizontalSlider->setRange(freq-2,freq+2);
    ui->horizontalSlider->setTickInterval(6);
    ui->horizontalSlider->setValue(freq);
    char d[32];
    sprintf(d,"%d",ui->horizontalSlider->value());
    ui->lineEdit->setText(d);

    _thr->change((const char*)ui->lineEdit->text().toUtf8());
}
