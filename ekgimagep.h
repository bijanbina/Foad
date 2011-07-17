/***********************************Besm allah rahman rahim******************************************
FILE:  EKGImageP.h
AUTHOR:	Bijan Binaee
FULL NAME: EKG IMAGE PROCESSING
CREATED ON:     3/2/2011
REVISED:	7/16/2011
  ___________________________________________________________________________

EKGImageP.h The image processing function implement here
Copywrite (C) 2011 Bijan Binaee

This file is free software; you can redistribute it and/or modify it under
the terms of the GNU Library General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

This software is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU Library General Public License for more
details.

You should have received a copy of the GNU Library General Public License along
with this library; if not, write to the Free Software Foundation, Inc., 59
Temple Place - Suite 330, Boston, MA 02111-1307, USA.

You may contact the author by e-mail (bijan@binaee.ir) or postal mail
(Iran Tehran Tehran Jahan Ara ST 23th , pelak : 47 second floor).  For updates to this software, please visit our website
(http://www.binaee.ir).
  __________________________________________________________________________
  Revisions:
        7/16: Detachment From scanner function
*****************************************************************************/
#ifndef EKGIMP_H
#define EKGIMP_H

#include "GButton.h"

struct ImageInfo
{
    int count;
    int red;
    int blue;
    int green;
};

class EKGImageP
{
public:
    EKGImageP();
    vector<double>    getSignal();
    int               findNazdik(int coloumn , int value , QImage image );
    void              loadPic(QString path);
private:
    bool              BlackPixel(ImageInfo input);
    bool              HavaWidth(vector<QImage> input,int width);
    bool              isblack(QColor color);
    int               isColor(QColor color);
    void              destroyGrid(QColor color);
    int               getRow(int row);
    double            mm2Pixel;                                          //use to convert between milimetr and pixel
    vector<QImage>    VSegmentation(QImage input);
    vector<QImage>    HSegmentation(QImage input);
    vector<ImageInfo> getPicInfo(QImage input);
    vector<ImageInfo> getVInfo(QImage input);
    QImage            bijooFilter(QImage input);
    QImage            crop2grid(QImage input);
    QColor            getColumn(int Column);
    QImage            scanImage;
    vector<double>    bSignal;
    vector<double>    proccessor();
};

#endif // EKGIMP_H
