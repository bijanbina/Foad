/***********************************Besm allah rahman rahim******************************************
FILE:  InfoWidget.h
AUTHOR:	Bijan Binaee
FULL NAME: Neural Network Framwork
CREATED ON:     07/22/2011
REVISED:	07/22/2011
  ___________________________________________________________________________

InfoWidget.h Widget that show info in a beautiful frame
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
        07/22: Create Class
*/

#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include "qrsdet.h"

class InfoWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = 0);
    explicit InfoWidget(QString name , int value , QWidget *parent = 0);
    explicit InfoWidget(QString name , QString value , QWidget *parent = 0);
    void setName(QString name);
    void setValue(double value);
    void setValue(int value);
    void setValue(QString value);
    QString getValue();
signals:

public slots:

private:
    QHBoxLayout  *H_Layout;
    QLabel       *Valuelabel;
    QLabel       *Namelabel;
};

#endif // INFOWIDGET_H
