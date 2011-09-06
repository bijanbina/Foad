/***********************************Besm allah rahman rahim******************************************
FILE:  NNframwork.h
AUTHOR:	Bijan Binaee
FULL NAME: Neural Network Framwork
CREATED ON:     07/22/2011
REVISED:	07/22/2011
  ___________________________________________________________________________

NNframwork.h Widget that can controll neural network and show result of NN
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
#ifndef NNFRAMWORK_H
#define NNFRAMWORK_H

#include "infoboxw.h"

class NNframwork : public QWidget
{
    Q_OBJECT
public:
    explicit NNframwork(QWidget *parent = 0);
    QMenu   *getMenu();
private slots:
    void askEpochNum();
    void asklearningRate();
    void asklearningMomentum();
    void askinputNNum();
    void askoutputNNum();
    void askhiddenNN();
    void askHiddenAF();
    void askOutputAF();
    void openClicked();
    void SaveClicked();
    void TrainClicked();
    void updateEpoch(double value);
private:
    //Function
    void createMenu();
    void createInfo();
    void UpdateData();
    void createLayout();
    void createConnection();
    void Warning(QString text);
    void plot(vector<double> data);
    void PlotConfig(QString plotName,int xSize);
    //Plot
    QwtPlot       *myPlot;
    QwtPlotCurve  *Train_curves;
    QwtSymbol      sym;
    //Layout
    QGridLayout   *info_layout;
    QVBoxLayout   *Mlayout;                      //Main Layout
    InfoWidget    *i_lastE;
    InfoWidget    *i_inputNNum;
    InfoWidget    *i_outputNNum;
    InfoWidget    *i_hiddentNN;
    InfoWidget    *i_EpochNum;
    InfoWidget    *i_HiddenAF;     //Hidden Activation Function
    InfoWidget    *i_OutputAF;     //Hidden Activation Function
    InfoWidget    *i_learningRate;
    InfoWidget    *i_learningMomentum;
    GButton       *TrainBtn;
    GButton       *OpenBtn;
    GButton       *SaveBtn;
    //Menu
    QAction       *A_inputNNum;
    QAction       *A_outputNNum;
    QAction       *A_hiddentNN;
    QAction       *A_EpochNum;
    QAction       *A_HiddenAF;     //Hidden Activation Function
    QAction       *A_OutputAF;     //Hidden Activation Function
    QAction       *A_learningRate;
    QAction       *A_learningMomentum;
    //Neural Network
    EKGNN         *LocalNN;
    int            EpochNum;
    int            EpochCount;
    vector<double> EpochError;
    NNSetting      NNParametr;
    //Menu
    QMenu        *NNMenu;

};

#endif // NNFRAMWORK_H
