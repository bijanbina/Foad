/***********************************Besm allah rahman rahim******************************************
FILE:  sigdemo.h
AUTHOR:	Bijan Binaee
FULL NAME: Information Box Widget
CREATED ON:     07/22/2011
REVISED:	07/22/2011
  ___________________________________________________________________________

sigdemo.h Widget for demo the detection of signal
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

#ifndef SIGDEMO_H
#define SIGDEMO_H


#include "sigdetect.h"

class SigDemo : public QObject
{
    Q_OBJECT
public:
    SigDemo(QObject *parent = 0);
    SigDemo(vector<double> signal , QwtPlot *plotwidget ,  int id , int EKG_Age , int start , QObject *parent = 0);
    Ekg_Data getInfo();
private:
    void   plot(vector<double> Signal);
    void   plot(double *Signal ,int size);
    void   prepare_plot();
    void   mark(int index);
    void   newMark(QString name);
    void   disMark(int index);
    void   updatePlot(vector<double> Signal);
    void   updatePlot(double *Signal ,int size);
    void   ZeroLine(bool animate);
    void   ZeroShib(bool animate);
    bool   AllDetected();
    double Miangin(vector<double>);
    double getLine(vector<double>);
    double getShib(vector<double> fbuffer);
    double getVariance(vector<double>);
    void   GUSSIOn(int reduce);
    void   findBeat();
    bool   findR();
    void   findDelta();
    void   findQS();
    void   findQ();
    void   findS();
    bool   findP();
    bool   findT();
    void   MK_QRS_Zero(int replace);
    void   MK_First_Zero();
    void   TPrepare();
    //Functions for Plot
    void   qCurve();
    void   rCurve();
    void   sCurve();
    void   pCurve();
    void   tCurve();
    void   prepare_curve(QwtPlotCurve *curve,QColor brush,bool bold = false);
    //Signal Data
    vector <double> MainSig;
    vector <double> buffer;
    double         *XSignal;//X axis of signal
    vector <int> sigBeat;
    Ekg_Data sigInfo;

    //Detector Plot
    QwtPlot *myPlot;
    QwtPlotZoomer *zoomer;
    QwtPlotCurve *Signal_curves;
    QwtPlotCurve *mark_curves;
    QwtPlotCurve *q_curves;
    QwtPlotCurve *r_curves;
    QwtPlotCurve *s_curves;
    QwtPlotCurve *p_curves;
    QwtPlotCurve *t_curves;
    QwtPlotCurve *u_curves;
    QwtPlotCurve *pBold_curves;
    QwtPlotCurve *tBold_curves;

    QwtPlotGrid *EKG_Grid;
    //Geometry Function
    void Pow2();
    void ABS();
    void SQRT();
    int MAX();
    double getShib(double point1,double point2);
    //
    void Warning  (QString text);
    void NODetect (double replace);
    void setDetect(vector<double> input);
    void gussian  (int reduce);
    vector<double> getDetect();
    vector<double> fastSmooth(vector<double> input , int width);
    //Mark Variable:
    vector<double> mark_temp_x;
    vector<double> mark_temp_y;
};

#endif // SIGDEMO_H
