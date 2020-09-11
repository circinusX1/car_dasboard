#include "clockwindow.h"
#include "ui_clockwindow.h"
#include "../include/dashdb_proxy.h"

ClockWindow::ClockWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClockWindow)
{
    ui->setupUi(this);
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(notify_update()));
    _timer->start(5000);
    _format = "hh:mm, MMM d, ddd";
     hide();
}

ClockWindow::~ClockWindow()
{
    _timer->stop();
    delete _timer;
    delete ui;
}


void ClockWindow::click_on(const QString& s)
{
    qDebug() << "reveived show \n";
    show();
}

void ClockWindow::on_pushButton_clicked()
{
    hide();
}

void ClockWindow::setup(dashdbIf* client)
{
    _client  = client;
    notify_update();
}

void ClockWindow::notify_update()
{
    if(_client)
    {
        QDateTime current = QDateTime::currentDateTime();
        QVariantMap mp;
        mp.insert("text", current.toString(_format));
        _client->notify(QString("panel2,@clock_applet"), mp);
    }
}
