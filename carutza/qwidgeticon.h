/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits    egg, antico
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
egg:        @author José Expósito
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com
*/

#ifndef __WidgetIcon_H
#define __WidgetIcon_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QMouseEvent>

class _IcoHolder;
class WidgetIcon : public QWidget
{
    Q_OBJECT
public:
    struct _IcoData
    {
        QString label;
        QString fullText;
        QPixmap pix;
        QPixmap gray_pix;
        int scaleFactor;
        bool drawBorder;
        bool elideMode;
        int state;
    };
    enum IconState {
        Normal,
        Disabled,
        Selected,
        Prelight
    };

    WidgetIcon(QWidget *parent = 0);
    explicit WidgetIcon(const QString &name,
                        const QString &label,
                        int set_size = 64,
                        QWidget *parent = 0);
    explicit WidgetIcon(const QString &label, const QPixmap &copy,
                        int set_size = 64, QWidget *parent = 0);
    virtual ~WidgetIcon();
    virtual QSize   sizeHint () const;
    void            paint_border(bool);
    void            set_size(int factor);
    int             width();
    int             height();

public slots:
    void            set_text(const QString &label);

signals:
    void clicked();

protected:
    void        _draw_image();
    void        _draw_text();
    void        _fit_text();
    void        _fit_font();

protected:
    void paintEvent (QPaintEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    _IcoData     _icon;
    QString      _name;
};

#endif

