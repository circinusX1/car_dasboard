/***************************************************************************
                          TrayIcon.cpp -  description
                             -------------------
    begin                : Sat Apr 12 00:32:39 CEST 2003
    copyright            : (C) 2003 by Pawel Pustelnik
    email                : pawelpus@o2.pl
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
 /* MOST OF THIS CODE IS FROM KADU PROJECT : WWW.KADU.NET */

#include "TrayIcon.h"
#include "qtradio.h"

#include <qapplication.h>
#include <qcursor.h>
#include <qstring.h>
#include <qx11info_x11.h>
#include <qmenu.h>
#include <qtimer.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

bool dock=TRUE;
QString npss;
int ile=0;
static XErrorHandler old_handler = 0;
static int dock_xerror = 0;

//KDERadio *Radio;

static int dock_xerrhandler(Display* dpy, XErrorEvent* err)
{
    dock_xerror = err->error_code;
    return old_handler(dpy, err);
}

static void trap_errors()
{
    dock_xerror = 0;
    old_handler = XSetErrorHandler(dock_xerrhandler);
}

static bool untrap_errors()
{
    XSetErrorHandler(old_handler);
    return (dock_xerror == 0);
}

static bool send_message(
    Display* dpy, /* display */
    Window w,     /* sender (tray icon window) */
    long message, /* message opcode */
    long data1,   /* message data 1 */
    long data2,   /* message data 2 */
    long data3    /* message data 3 */
)
{
    XEvent ev;

    memset(&ev, 0, sizeof(ev));
    ev.xclient.type = ClientMessage;
    ev.xclient.window = w;
    ev.xclient.message_type = XInternAtom (dpy, "_NET_SYSTEM_TRAY_OPCODE", False );
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = CurrentTime;
    ev.xclient.data.l[1] = message;
    ev.xclient.data.l[2] = data1;
    ev.xclient.data.l[3] = data2;
    ev.xclient.data.l[4] = data3;

    trap_errors();
    XSendEvent(dpy, w, False, NoEventMask, &ev);
    XSync(dpy, False);
    return untrap_errors();
};

#define SYSTEM_TRAY_REQUEST_DOCK    0
#define SYSTEM_TRAY_BEGIN_MESSAGE   1
#define SYSTEM_TRAY_CANCEL_MESSAGE  2

TrayIcon::TrayIcon()
	: QLabel(NULL,  Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
        this->setAttribute(Qt::WA_MouseNoMask);

	if (!dock)//dock=TRUE=>dock window
		return;

	QPixmap pix(QString(share_dir()) + "/icons/qtradio.png");
	// setBackgroundMode(Qt::X11ParentRelative);
	setMinimumSize(pix.size());
	QLabel::setPixmap(pix);
	resize(pix.size());
	//setMouseTracking(true);
	npss = Radio->Ticker->station_name;
	if ( npss != "" )
	{
		npss.append(" : ");
		npss.append( QString::number(Radio->freq,'f',2) );
		npss.append(" MHz");
	}
	this->setToolTip(npss);
	update();

	icon_timer = new QTimer(this);
	blink = FALSE;
	//QObject::connect(icon_timer, SIGNAL(timeout()), this, SLOT(changeIcon()));

	//hint = new TrayHint(0);

	Display *dsp = QX11Info::display();
	WId win = winId();

	// Okno potrzebne dla WindowMakera, ale przydaje
	// siê te¿ w KDE 3.1, gdzie je¶li chocia¿ na chwilê
	// nie poka¿emy okna g³ównego na ekranie wystêpuje
	// "efekt klepsydry"
	WMakerMasterWidget=new QWidget(0);
	WMakerMasterWidget->setGeometry(-10,-10,0,0);

	// SPOSÓB PIERWSZY
	// System Tray Protocol Specification
	// Dzia³a pod KDE 3.1 i GNOME 2.x
	Screen *screen = XDefaultScreenOfDisplay(dsp);
	int screen_id = XScreenNumberOfScreen(screen);
	char buf[32];
	snprintf(buf, sizeof(buf), "_NET_SYSTEM_TRAY_S%d", screen_id);
	Atom selection_atom = XInternAtom(dsp, buf, FALSE);
	XGrabServer(dsp);
	Window manager_window = XGetSelectionOwner(dsp, selection_atom);
	if (manager_window != None)
		XSelectInput(dsp, manager_window, StructureNotifyMask);
	XUngrabServer(dsp);
	XFlush(dsp);
	if (manager_window != None)
		send_message(dsp, manager_window, SYSTEM_TRAY_REQUEST_DOCK, win, 0, 0);
	
	// SPOSÓB DRUGI
	// Dzia³a na KDE 3.0.x i pewnie na starszych
	// oraz pod GNOME 1.x
	int r;
	int data = 1;
	r = XInternAtom(dsp, "KWM_DOCKWINDOW", FALSE);
	XChangeProperty(dsp, win, r, r, 32, 0, (uchar *)&data, 1);
	r = XInternAtom(dsp, "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR", FALSE);
	XChangeProperty(dsp, win, r, XA_WINDOW, 32, 0, (uchar *)&data, 1);
			
	// SPOSÓB TRZECI
	// Dzia³a pod Window Maker'em
	WId w_id = WMakerMasterWidget->winId();
	XWMHints *hints;
	hints = XGetWMHints(dsp, w_id);
	hints->icon_window = win;
	hints->window_group = w_id;
	hints->flags |= WindowGroupHint | IconWindowHint;
	XSetWMHints(dsp, w_id, hints);
	XFree( hints );
};

TrayIcon::~TrayIcon()
{
	delete WMakerMasterWidget;
	//delete hint;
	//kdebug("TrayIcon::~TrayIcon()\n");
}

QPoint TrayIcon::trayPosition()
{
	return mapToGlobal(QPoint(0,0));
};

void TrayIcon::show()
{
	QLabel::show();
	WMakerMasterWidget->show();
	// Je¶li WindowMaker nie jest aktywny okno
	// nie powinno zostaæ widoczne
	if(XInternAtom(QX11Info::display(),"_WINDOWMAKER_WM_PROTOCOLS",true)==0)
		WMakerMasterWidget->hide();
};

void TrayIcon::setPixmap(const QPixmap& pixmap)
{
	QLabel::setPixmap(pixmap);
	WMakerMasterWidget->setWindowIcon(pixmap);
	repaint();
};

void TrayIcon::setType(QPixmap &pixmap)
{
	if (!dock)
		return;
	setPixmap(pixmap);
}




/*void TrayIcon::connectSignals() {
	QObject::connect(dockppm, SIGNAL(activated(int)), this, SLOT(dockletChange(int)));
}*/

void TrayIcon::resizeEvent(QResizeEvent*)
{
//	if (icon)
//		icon->resize(size());
//	resize(size());
};

void TrayIcon::enterEvent(QEvent* e)
{
	if (!qApp->focusWidget())
	{
		XEvent ev;
		memset(&ev, 0, sizeof(ev));
		ev.xfocus.display = QX11Info::display();
		ev.xfocus.type = FocusIn;
		ev.xfocus.window = winId();
		ev.xfocus.mode = NotifyNormal;
		ev.xfocus.detail = NotifyAncestor;
		unsigned long time = QX11Info::appTime();
		QX11Info::setAppTime(1);
		qApp->x11ProcessEvent( &ev );
		QX11Info::setAppTime(time);
	};
	QWidget::enterEvent(e);
};

void TrayIcon::mousePressEvent(QMouseEvent * e)
{
	if (!dock)
		return;

        QMenu *st = Radio->style_dock;
	
	if (e->button() == Qt::LeftButton)
	{
		switch (Radio->isVisible()) {
			case FALSE:
				if (ile == 0)
				{
					if ( Radio->power_on )
					{
						Radio->load_settings();
						Radio->load_skin();
					}
					ile = 1;					
				}
				Radio->show();//now show window
				Radio->setFocus();
				break;
			case TRUE:
				Radio->hide();
				break;
			}
		return;
	}

	if (e->button() == Qt::RightButton) 
	{
		st->exec(QCursor::pos());
		return;
	}

}

TrayIcon *trayicon = NULL;

