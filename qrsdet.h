/*****************************************************************************
FILE:  qrsdet.h
AUTHOR:	Patrick S. Hamilton
REVISED:	4/16/2002
  ___________________________________________________________________________

qrsdet.h QRS detector parameter definitions
Copywrite (C) 2000 Patrick S. Hamilton

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

You may contact the author by e-mail (pat@eplimited.com) or postal mail
(Patrick Hamilton, E.P. Limited, 35 Medford St., Suite 204 Somerville,
MA 02143 USA).  For updates to this software, please visit our website
(http://www.eplimited.com).
  __________________________________________________________________________
  Revisions:
	4/16: Modified to allow simplified modification of digital filters in
   	qrsfilt().
*****************************************************************************/


#define SAMPLE_RATE	200	/* Sample rate in Hz. */
#define MS_PER_SAMPLE	( (double) 1000/ (double) SAMPLE_RATE)
#define MS10	((int) (10/ MS_PER_SAMPLE + 0.5))
#define MS25	((int) (25/MS_PER_SAMPLE + 0.5))
#define MS30	((int) (30/MS_PER_SAMPLE + 0.5))
#define MS80	((int) (80/MS_PER_SAMPLE + 0.5))
#define MS95	((int) (95/MS_PER_SAMPLE + 0.5))
#define MS100	((int) (100/MS_PER_SAMPLE + 0.5))
#define MS125	((int) (125/MS_PER_SAMPLE + 0.5))
#define MS150	((int) (150/MS_PER_SAMPLE + 0.5))
#define MS160	((int) (160/MS_PER_SAMPLE + 0.5))
#define MS175	((int) (175/MS_PER_SAMPLE + 0.5))
#define MS195	((int) (195/MS_PER_SAMPLE + 0.5))
#define MS200	((int) (200/MS_PER_SAMPLE + 0.5))
#define MS220	((int) (220/MS_PER_SAMPLE + 0.5))
#define MS250	((int) (250/MS_PER_SAMPLE + 0.5))
#define MS300	((int) (300/MS_PER_SAMPLE + 0.5))
#define MS360	((int) (360/MS_PER_SAMPLE + 0.5))
#define MS450	((int) (450/MS_PER_SAMPLE + 0.5))
#define MS1000	SAMPLE_RATE
#define MS1500	((int) (1500/MS_PER_SAMPLE))
#define DERIV_LENGTH	MS10
#define LPBUFFER_LGTH ((int) (2*MS25))
#define HPBUFFER_LGTH MS125

#define WINDOW_WIDTH	MS80			// Moving window integration width.
#define	FILTER_DELAY (int) (((double) DERIV_LENGTH/2) + ((double) LPBUFFER_LGTH/2 - 1) + (((double) HPBUFFER_LGTH-1)/2) + PRE_BLANK)  // filter delays plus 200 ms blanking delay
#define DER_DELAY	WINDOW_WIDTH + FILTER_DELAY + MS100

#ifndef QrSDET_INCLUDED
#define QrSDET_INCLUDED
//C++ Standard Header
#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <algorithm>
//QT Header
#include <QtGui>
#include <QFile>
#include <QLabel>
#include <QtCore>
#include <QLocale>
#include <QMenuBar>
#include <QTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QMainWindow>
#include <QDockWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QTranslator>
#include <QTextStream>
#include <QFileDialog>
#include <QLibraryInfo>
#include <QInputDialog>
#include <QProgressBar>
#include <QApplication>
#include <QDesktopWidget>

//WFDB Header
#include <wfdb/wfdb.h>
#include <wfdb/ecgcodes.h>
#include <wfdb/ecgmap.h>
//QWT Plot Header
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_map.h>
#include <qwt_scale_draw.h>
#include <qwt_double_rect.h>
#include <qwt_symbol.h>
#include <qwt_color_map.h>
//Internal Header

//Pthread
#include <pthread.h>
//#include "wekawriter.h"
//Define
#define PRE_BLANK	MS195
#define MIN_PEAK_AMP	7 // Prevents detections of peaks smaller than 150 uV.

using namespace std;

//New Class Implement By Bijan Binaee on March 2011
class QRSDetection
{
public:
    QRSDetection();
    int Detect(int datum );
    int getCounter();
    vector <double> getDetected();
    vector <double> getFilter();
private:
    //------Function--------
    int Peak( int datum, int init ) ;
    int mean(int *array, int datnum) ;
    int thresh(int qmean, int nmean) ;
    int BLSCheck(int *dBuf,int dbPtr,int *maxder) ;
    //------Variable--------
    double TH ;
    int DDBuffer [92] , DDPtr ;	/* Buffer holding derivative data. */
    int Dly;
    int qrsbuf[8], noise[8], rrbuf[8] ;
    int maxder, lastmax ;
    int initBlank, initMax ;
    int preBlankCnt, tempPeak ;
    int count;
    int det_thresh, qpkcnt ;
    int rsetBuff[8], rsetCount ;
    int nmean, qmean, rrmean ;
    int sbpeak, sbloc, sbcount ;
    vector <double> detcted_sig;
    vector <double> filter_sig;
};

#endif
