/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     matchbox, egg, antico, xfwm4, metacity
--------------------------------------------------------------------------------

 This file is part of DASHSASH dashboard

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
*/
/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/


#include "qwidgeticon.h"
#include <QPainter>
#include <QIcon>

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
WidgetIcon::WidgetIcon(QWidget *parent)
    : QWidget(parent)
{
    _icon.label = "";
    _icon.fullText = "";
    _icon.scaleFactor = 64;
    _icon.drawBorder = false;
    _icon.elideMode = true;
    _icon.state = Normal;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
WidgetIcon::WidgetIcon(const QString &name, const QString &label, int scale,
            QWidget *parent)
    : QWidget(parent),_name(name)
{

    _icon.elideMode = true;
    set_text(label);
    _icon.scaleFactor = scale;
    _icon.drawBorder = false;
    // TODO:
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
WidgetIcon::WidgetIcon(const QString &label, const QPixmap &copy, int scale, QWidget *parent)
    : QWidget(parent)
{
    _icon.scaleFactor = scale;
    _icon.drawBorder = false;
    _icon.elideMode = true;
    _icon.state = Normal;
    _fit_font();
    set_text(label);
    int s = scale - 4;
    _icon.pix = QPixmap(copy.scaled(QSize(s, s), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    _icon.gray_pix = QIcon(_icon.pix).pixmap(QSize(s, s), QIcon::Disabled);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
WidgetIcon::~WidgetIcon()
{

}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::set_text(const QString &label)
{
    if (_icon.elideMode) {
        _icon.fullText = label;
        _fit_text();
    } else {
        _icon.label = label;
        _icon.fullText = label;
    }
    update();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::_fit_font()
{
    QFont f = QFont(font());
    if (_icon.scaleFactor == 128) {
        f.setPointSize(12);
        f.setBold(true);
    } else 	if (_icon.scaleFactor == 64) {
        f.setPointSize(10);
        f.setBold(true);
    } else if (_icon.scaleFactor == 42) {
        f.setPointSize(9);
        f.setBold(true);
    } else if (_icon.scaleFactor == 32) {
        f.setPointSize(8);
        f.setBold(false);
    }
    else if (_icon.scaleFactor == 16) {
        f.setPointSize(7);
        f.setBold(false);
    }
    //f.setWeight(75);
    setFont(f);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
QSize WidgetIcon::sizeHint() const
{
    return (QSize(_icon.scaleFactor * 2, _icon.scaleFactor + 5 + fontMetrics().height() * 2));
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
int WidgetIcon::width()
{
    return _icon.scaleFactor * 2;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
int WidgetIcon::height()
{
    return _icon.scaleFactor + 5 + fontMetrics().height() * 2;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::_fit_text()
{
    QFontMetrics fm(fontMetrics());
    int charWidth = fm.width('D');
    int labelWidth = width();
    int lineWidth = labelWidth / charWidth;

    QString first, second;

    QStringList lines = _icon.fullText.split("\n");
    if (lines.size() >= 2) {
        first = lines.at(0);
        lines.removeFirst();
        second = lines.join(" ");
    } else {
        first = _icon.fullText.left(lineWidth); // left(20)
        int remaining = _icon.fullText.size() - lineWidth; // 30 - 20 = 10
        if (remaining > 0)
            second = _icon.fullText.right(remaining); // right(10)
    }

    if (!second.isEmpty()) {
        if (fm.width(second) >= lineWidth) {
            _icon.label = first + "\n" + fm.elidedText(second, Qt::ElideMiddle, labelWidth);
            setToolTip(_icon.fullText);
        } else {
            _icon.label = first + "\n" + second;
        }
    } else {
        _icon.label = first + "\n";
        setToolTip(QString());
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::paint_border(bool draw)
{
    _icon.drawBorder = draw;
    update();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::set_size(int factor)
{
    Q_UNUSED(factor);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::_draw_image()
{
    if (!_icon.pix.isNull()) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QRect rect = painter.window();

        int left = (rect.width() - _icon.pix.width()) / 2;

        if (_icon.drawBorder) {
            QColor base;
            base = QColor(palette().window().color());
            painter.fillRect(5, 4, rect.width()-7, rect.height()-6, QBrush(base.darker(180)));
            painter.setPen(QPen(QBrush(QColor(254, 254, 254)), 3));
            painter.drawRoundedRect(4, 3, rect.width()-6, rect.height()-5, 4, 4);
        }

        QRect pixRect(left, 7, _icon.pix.width(), _icon.pix.height());
        if (_icon.state == Normal)
            painter.drawPixmap(pixRect, _icon.pix);
        else
            painter.drawPixmap(pixRect, _icon.gray_pix);
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::_draw_text()
{
    if (!_icon.label.isEmpty()) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QRect rect = painter.window();
        QStringList lines = _icon.label.split("\n");

        int y = _icon.scaleFactor + 4;
        QRect offset(rect.left(), y, rect.width(), rect.height() - y);
        if (_icon.drawBorder)
            painter.setPen(QColor(255, 255, 255));
        painter.drawText(offset, Qt::AlignHCenter | Qt::AlignTop, lines.at(0));

        if (lines.size() > 1) {
            y += fontMetrics().height();
            QRect offset(rect.left(), y, rect.width(), rect.height() - y);
            painter.drawText(offset, Qt::AlignHCenter | Qt::AlignTop, lines.at(1));
        }
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    _draw_image();
    _draw_text();
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        _icon.state = Selected;
        update();
    }
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
void WidgetIcon::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        _icon.state = Normal;
        update();
        emit clicked();
    }
}
