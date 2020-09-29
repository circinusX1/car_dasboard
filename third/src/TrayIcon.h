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
#ifndef _TRAYICON_H_
#define _TRAYICON_H_

#include <qwidget.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qevent.h>

class QWidget;
class QTimer;

class TrayIcon : public QLabel
{
	Q_OBJECT

	private:
		//friend class TrayHint;
		QWidget* WMakerMasterWidget;
		//TrayHint *hint;
		QTimer *icon_timer;
		bool blink;

	protected:
		void setPixmap(const QPixmap& pixmap);
		virtual void resizeEvent(QResizeEvent* e);
		virtual void enterEvent(QEvent* e);
		virtual void mousePressEvent(QMouseEvent*);

	public:
		TrayIcon();
		~TrayIcon();
		QPoint trayPosition();
		void show();
		void setType(QPixmap &pixmap);

	public slots:
		
};


extern TrayIcon *trayicon;

#endif
