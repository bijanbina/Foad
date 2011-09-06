/***********************************Besm allah rahman rahim******************************************
FILE:  infoboxw.h
AUTHOR:	Bijan Binaee
FULL NAME: Information Box Widget
CREATED ON:     07/22/2011
REVISED:	07/22/2011
  ___________________________________________________________________________

infoboxw.h Widget that show all information abuoy ekg
also use for controll application
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
(Iran Tehran Tehran Jahan Ara ST 23th , pelak : 47 second floor).
For updates to this software, please visit our website
(http://www.binaee.ir).
  __________________________________________________________________________
  Revisions:
        07/22: Create Class
*/
#ifndef INFOBOXW_H
#define INFOBOXW_H

#include "kiwidget.h"

class infoBoxW : public QWidget
{
    Q_OBJECT
public:
    explicit infoBoxW(QWidget *parent = 0);
    QString  Record();
    QString  Time();
    QString  Age();
    QString  P();
    QString  PR();
    QString  Q();
    QString  R();
    QString  S();
    QString  T();
    QString  QT();
    QString  QRS();
    QString  Power();
    QString  BeatRate();
    QString  RR();
    QString  Disease();
    void     setRecord(QString value);
    void     setTime(int value);
    void     setAge(int value);
    void     setP(int value);
    void     setPR(int value);
    void     setQ(int value);
    void     setR(int value);
    void     setS(int value);
    void     setT(int value);
    void     setQT(int value);
    void     setQRS(int value);
    void     setPower(int value);
    void     setBeatRate(int value);
    void     setRR(int value);
    void     setDisease(QString value);
signals:
    void scan_click();
    void open_click();
    void start_click();
private slots:
    void p_scan_click();
    void p_open_click();
    void p_start_click();
private:
//Variable:
//! K is prefix for all KIWidget
    KIWidget *KRecord;
    KIWidget *KTime;
    KIWidget *KAge;
    KIWidget *KP;
    KIWidget *KPR;
    KIWidget *KQ;
    KIWidget *KR;
    KIWidget *KS;
    KIWidget *KT;
    KIWidget *KQT;
    KIWidget *KQRS;
    KIWidget *KPower;
    KIWidget *KBeatRate;
    KIWidget *KRR;
    KIWidget *KDisease;
//Buttons:
    GButton *scanBTN;
    GButton *openBTN;
    GButton *startBTN;
//Layout
    QGridLayout *layout;
};

#endif // INFOBOXW_H
