/***************************************************************************
                          recording.cpp  -  description
                             -------------------
    begin                : Sat Jun 21 2003
    copyright            : (C) 2003 by Pawel Pustelnik
                         : (C) 2013 by Oleh Nykyforchyn
    email                : oleh.nyk@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "shortcut.h"
#include "info.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qkeysequence.h>
#include <qevent.h>
#include <qframe.h>

/*
 *  Constructs a Shortcut as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Shortcut::Shortcut( QWidget* parent,  Qt::WFlags fl )
    : QDialog( parent, fl )

{
    this->setAttribute(Qt::WA_QuitOnClose, FALSE);

    textLabel2 = new QLabel( this );
    textLabel2->setGeometry( QRect( 10, 20, 160, 20 ) );


    textLabel1 = new QLabel( this );
    textLabel1->setGeometry( QRect( 180, 20, 120, 20 ) );
    textLabel1->setFrameShape( QLabel::Box );
    languageChange();
    resize( QSize(389, 62).expandedTo(minimumSizeHint()) );

}


void Shortcut::keyPressEvent( QKeyEvent *k )
{
//   DOES NOT WORK. WHY?
//
//    if (k->type() == QEvent::KeyRelease)
//    {
//        textLabel1->setText("");
//        return;
//    }

    if (k->key() == Qt::Key_Escape)
    {
        textLabel1->setText("");
        Change_shortcut();
        return;
    }

    if ( k->key() == 65535 || k->key() == 4129 || k->key() == 4131)
        return;

    QString s = "";
    unsigned int state = ((QInputEvent*)(k))->modifiers();

    if ((state & Qt::ControlModifier) || (k->key() == Qt::Key_Control))
        s = "Ctrl+";
    if ((state & Qt::AltModifier) || (k->key() == Qt::Key_Alt))
        s.append("Alt+");
    if ((state & Qt::ShiftModifier) || (k->key() == Qt::Key_Shift))
        s.append("Shift+");

    if ( (k->key() != Qt::Key_Control) &&
         (k->key() != Qt::Key_Alt) &&
         (k->key() != Qt::Key_Shift) )
    {
        QKeySequence ks( k->key() );
        QString kn = ks;
        s.append(kn);
        textLabel1->setText( s );
        Change_shortcut();
    }
    else
        textLabel1->setText( s );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Shortcut::~Shortcut()
{
    // no need to delete child widgets, Qt does it all for us
    // fprintf(stderr, "Shortcut deleted\n");
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Shortcut::languageChange()
{
    setWindowTitle( tr( "Choose new shortcut" ) );
    textLabel2->setText( tr( "New shortcut :" ) );

    textLabel1->setText( QString::null );
}

void Shortcut::Change_shortcut()
{

  Shortcut::hide();

}
