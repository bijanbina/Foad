/***********************************Besm allah rahman rahim******************************************
FILE:  Header.h
AUTHOR:	Bijan Binaee
CREATED ON:     5/16/2010
REVISED:	7/11/2011
  ___________________________________________________________________________

Header.h All header use in foad project are list here
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
        7/11: Add TBB for multi threading
*****************************************************************************/


#define SAMPLE_RATE	            200.0	/* Sample rate in Hz. */
#define DEFAULT_EPOCH_COUNT         2000
#define TIMEREPEAT 60
#define TRAiNTIME 8 * SAMPLE_RATE
#define ASKCOMPLEX false
#define WIDTH  1000
#define HEIGHT 640

#ifndef QrSDET_INCLUDED
#define QrSDET_INCLUDED

//C++ Standard Header
#include <sys/types.h>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdarg>
#include <errno.h>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
//QT Header
#include <QtGui>
#include <QFile>
#include <QLabel>
#include <QMutex>
#include <QtCore>
#include <QLocale>
#include <QPointer>
#include <QMenuBar>
#include <QTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QSplitter>
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
#include <QTranslator>
#include <QDesktopWidget>
#include <QVarLengthArray>
//WFDB Header
#include <wfdb/wfdb.h>
#include <wfdb/ecgcodes.h>
#include <wfdb/ecgmap.h>
//QWT Plot Header
#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_symbol.h>
#include <qwt_color_map.h>
#include <qwt_scale_map.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_double_rect.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_spectrogram.h>
//Pthread
#include <pthread.h>
//Intel Multithread
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
//Neural Network
#include <floatfann.h>
//Fast Fourier Transform
#include <fftw3.h>
//Scanner (libksane)
#include <libksane/ksane.h>
//Name Space
using namespace std;
using namespace tbb;
using namespace KSaneIface;
//Configurations:
#define FOAD_LANGUAGE "fa"
//#define FOAD_LANGUAGE en
#endif
