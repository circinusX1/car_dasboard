/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
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
#include <iostream>
#include <QIcon>
#include <QDebug>
#include <unistd.h>
#include <stdlib.h>
#include "imagez.h"
#include "mysett.h"

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
Imagez::Imagez()
{
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
Imagez::Imagez(const char* path, const char* image)
{
    load_image(path, image);
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
Imagez::~Imagez()
{

}

//-----------------------------------------------------------------------------
bool Imagez::load_image(const char* path,
                        const char* cimage,
                        bool force,
                        const char* format,
                        Qt::ImageConversionFlags flags)
{
    cond_if(cimage[0]==0, return false);

    QString image  = cimage;
    MySett& config = MySett::config();
    QString fp;

    qDebug() << "LOADING: " << cimage << "\n";

    if(!strstr(cimage,".png"))
    {
         image+=".png";
    }

    fp = path;
    cond_if((!fp.isEmpty() && !fp.endsWith("/")), fp+="/");
    fp+=image;
    if(0==::access(fp.toUtf8(),0))
    {
        return QPixmap::load(fp, format, flags);
    }

    fp = config.images()+image;
    if(0==::access(fp.toUtf8(),0))
    {
        return QPixmap::load(fp, format, flags);
    }

    fp = "/usr/share/app-install/icons/"; fp += image;
    if(0==::access(fp.toUtf8(),0))
    {
        bool br = QPixmap::load(fp, format, flags);
        return br;
    }

    std::cout << "cannot load: " << image.toStdString() << "\n";

    if(force)
    {
        if(fp.isEmpty() || fp=="0")
            return false;

        if(0==::access(fp.toUtf8(),0))
        {
            bool br = QPixmap::load(fp, format, flags);
            return br;
        }else
        {
            std::cout << "cannot load: " << fp.toStdString() << "\n";
        }
    }

    return false;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
#define VA_NEXT(var, type)      ((var) = (type) va_arg(args, type))
bool Imagez::load_images(const char* first, ...)
{
    bool br=false;
    va_list args;
    const char *string;

    va_start(args, first);
    for (string = first; string != 0; VA_NEXT(string, const char *))
    {
        if((br=load_image("",string)))
            break;
    }
    va_end(args);
    return br;
}

/*--------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------*/
bool Imagez::fromrgb(int rgb)
{
    fill(QColor(rgb));
    return true;
}

