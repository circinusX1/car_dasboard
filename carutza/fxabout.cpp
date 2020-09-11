#include <QFile>
#include "fxabout.h"
#include "ui_fxabout.h"

FxAbout::FxAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FxAbout)
{
    ui->setupUi(this);

    bool b = false;

    QFile file("/proc/cpuinfo");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QMap <QString, QString> map;
    while (true)
    {
        QString line = file.readLine();
        if (line.isEmpty()) break;
        QStringList data = (line).split(':');

        if (data.size() == 2)
        {
            map.insert(data[0].trimmed(), data[1].trimmed());
            b = true;
        }
    }
    file.close();

    if (b)
    {
        ui->label->setText(map.value("vendor_id"));
        ui->label_2->setText(map.value("model name"));
        ui->label_3->setText(map.value("cpu MHz") + " MHz");
    }

}

FxAbout::~FxAbout()
{
    delete ui;
}
