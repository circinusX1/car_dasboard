/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits    antico
--------------------------------------------------------------------------------
 This file is part of CARUTZA dashboard

    CARUTZA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CARUTZA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CARUTZA.  If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
Credits
--------------------------------------------------------------------------------
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com
*/


#ifndef __DEFS_H
#define __DEFS_H

#include <QApplication>
#include <QPushButton>
#include <QMap>
#include <QLabel>
#include <QPainter>
#include <QPalette>
#include <QCheckBox>
#include <QVariant>
#include <QCursor>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QColor>
#include <QBitmap>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QHash>
#include <QSizePolicy>
#include <QAction>
#include <QVariant>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QIcon>
#include <QWidget>
#include <QSpinBox>
#include <QByteArray>
#include <QBrush>
#include <QFrame>
#include <QX11Info>
#include <QDesktopWidget>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QSettings>
#include <QTabWidget>
#include <QDirModel>
#include <QTreeView>
#include <QMessageBox>
#include <QStringList>
#include <QListWidget>
#include <QSignalMapper>
#include <QFileDialog>
#include <QColorDialog>
#include <QGroupBox>
#include <QSpacerItem>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QTimer>
#include <QDateTime>
#include <QStyleFactory>
#include <QToolBox>
#include <QToolButton>
#include <QShortcut>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <X11/keysym.h>

#define DPY() QX11Info::display()
#define ROOT_XWIN() QX11Info::appRootWindow()


#ifdef None
#   undef None   // 0	/* for windows that are not mapped */
#   define X11_None    0L
#endif

#endif
