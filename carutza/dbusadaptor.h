
#ifndef __DBUSADAPTOR_H
#define __DBUSADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QDBusAbstractAdaptor>

class Desktop;
class TheApp;
class Dockbar;

class DBusAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.freedesktop.FLEXCARWOA1._p_dbus_sys")

public:
    DBusAdaptor(QObject *obj);
	~DBusAdaptor();

public slots:
	Q_NOREPLY void changeWallpaper(const QString &path);
	Q_NOREPLY void changeHighlight(int r, int g, int b);
	Q_NOREPLY void changeStyle(const QString &style);
	Q_NOREPLY void changeDockSize(int factor);
	Q_NOREPLY void _bdbl_clk_rule(bool active);
	Q_NOREPLY void setDockAutoHide(bool active);

protected:
};

#endif

