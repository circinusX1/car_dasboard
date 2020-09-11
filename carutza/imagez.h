/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
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

*/

#ifndef IMAGEZ_H
#define IMAGEZ_H

#include <QPixmap>

class Imagez : public QPixmap
{
public:
    explicit Imagez(int x, int y, int format):QPixmap(x,y){Q_UNUSED(format);};
    explicit Imagez(const char* path, const char* image);
    explicit Imagez();
    virtual ~Imagez();
    bool load_image(const char* path, const char* image, bool force=true, const char *format = 0,
                    Qt::ImageConversionFlags flags = Qt::AutoColor);
    bool load_images(const char*, ...);
    Imagez& operator=(const QPixmap& p){return (Imagez&)QPixmap::operator=(p);}
    bool fromrgb(int rgb);
private:

};


#endif // IMAGEZ_H
