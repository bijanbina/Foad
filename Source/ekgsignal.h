/***********************************Besm allah rahman rahim******************************************
FILE:  ekgSignal.h
AUTHOR:	Bijan Binaee
FULL NAME: ekg signal reader (platform base)
CREATED ON:     07/22/2011
REVISED:	07/22/2011
  ___________________________________________________________________________

ekgSignal.h read ekg signal from standard wfdb database
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
#ifndef EKGSIGNAL_H
#define EKGSIGNAL_H
#include "listrw.h"
class EKGSignal
{
public:
    EKGSignal();
    void  setDB(QString path);
    char *getDB();
    char *getRecord();
    bool  open(QString record);
    bool  read(int time , int Starttime = 0);
    long int   nsamp();
    vector<double> getSignal();
private:
    int  NextSample(int *vout,int nosig,int ifreq, int ofreq,bool init);
    int  gcd(int x, int y);
    char RecordNumber[10];
    char DB[100];
    bool exist;
    WFDB_Siginfo sigInfo;
    vector<double> signal;
    ///
    int InputFileSampleFrequency;
    int EKG_age ,ADCZero ,ADCUnit;
};

#endif // EKGSIGNAL_H
