#include "clockwindow.h"
#include <QApplication>
#include <QObject>
#include <QDebug>
#include "../include/dashdb_proxy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClockWindow w;

    dashdbIf* client = new dashdbIf("com.zirexix.carowa", "/", QDBusConnection::sessionBus(), 0);
    w.setup(client);
    w.hide();
    QObject::connect(client, SIGNAL(user_click(QString)), &w, SLOT(click_on(QString)));
    return a.exec();
}

