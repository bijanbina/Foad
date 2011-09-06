/***********************************Besm allah rahman rahim******************************************
FILE:  interceptW.h
AUTHOR:	Bijan Binaee
FULL NAME: intercept (mode) Widget (platform base)
CREATED ON:     07/22/2011
REVISED:	07/22/2011
  ___________________________________________________________________________

interceptW.h Widget to show intercept pannel
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

#ifndef INTERCEPTW_H
#define INTERCEPTW_H

#include "nnframwork.h"
#define TIMEREPEAT 60
#define TRAiNTIME 8 * SAMPLE_RATE
#define ASKCOMPLEX false

class InterceptW : public QWidget
{
    Q_OBJECT
public:
    explicit InterceptW(QWidget *parent = 0);
    weka_data Intercept(vector<double> sig , bool getPlot = false, bool getIntercept = false);
signals:

public slots:
    void setAge();
    void setDB_Path();
    void setSigTime();
    void addDisease();
    void setDisease();
    void setRecordNum();
    void Detection_Click();
    void openTrain();
    void scan();
    void openImage();
    void openRecord();
    void scanFinished();
    //menu slots
    void A_Sig_change(bool val);
    void A_Det_change(bool val);
    void A_Plot_change(bool val);
    void A_Fil_change(bool val);
    void B_AskComplex_change(bool val);

private:
    void update_info();
    int  askComplex(int end);
    void Warning(QString text);
    void createSandS();//Create Signal and Slot
    void OpenError(char *recordNum);
    void CreateLayout();
    vector<QString>   getFiles(char *patch);
    void plot(double *Signal,double *Detect,int size);
    vector<QString>   getUnDiz(char *patch, vector<QString> values);
    //Layout
    QSpacerItem *space;
    QVBoxLayout *Main_Layout;
    QHBoxLayout *Info_Layout;
    //Plot
    QwtPlot       *myPlot;
    QwtPlot       *interceptPlot;
    QwtPlotZoomer *zoomer;
    QwtPlotCurve  *Signal_curves;
    QwtPlotCurve  *Detect_curves;
    QwtPlotGrid   *EKG_Grid;
    QwtPlotCurve  *q_curves;
    QwtPlotCurve  *r_curves;
    QwtPlotCurve  *s_curves;
    QwtPlotCurve  *p_curves;
    QwtPlotCurve  *t_curves;
    QwtPlotCurve  *u_curves;
    //Functions for Plot
    void qCurve();
    void rCurve();
    void sCurve();
    void pCurve();
    void tCurve();
    //EKG
    EKG_atr          localFeature;
    ListRW           disList;                //Disease List
    weka_data        localWeka;
    vector<double>   Signal;
    EKGSignal        sigReader;
    int              ventricularrate();
    SigDetect       *InterCeptWindow;
    vector<Ekg_Data> localInfo;
    int              InputFileSampleFrequency;
    int              EKG_age ,ADCZero ,ADCUnit;
    //Gui
    double           Percentage , FileMarhale;
    double           filePercentage;
    QPushButton     *numbers;
    QMessageBox     *message;
    QFileDialog     *filebrowser;
    QFrame          *jodaKonnande;
    //Signal Info
    int              SigTime;
    QString          SigRecord;
    //Scanner
    QScan           *Scanner;
    //Image Processor
    EKGImageP        imageProc;
    //InfoBox Widget
    infoBoxW        *p_info;//Private info box
    //Menu Value:
    bool A_Ekg_signal;
    bool A_Det_signal;
    bool A_Fil_signal;
    bool A_Plot_show;
    bool B_AskComplex;
};

#endif // INTERCEPTW_H
