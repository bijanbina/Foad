#ifndef FEUTUREEXTRACTION_H
#define FEUTUREEXTRACTION_H

#include "wekawriter.h"
class FeutureExtraction
{
public:
    FeutureExtraction(vector<Ekg_Data> input);
    weka_data getWeka();
private:
    double getVariance(vector<double> fbuffer);
    double getVariance(vector<int> fbuffer);
    double Miangin(vector<double> fbuffer);
    double Miangin(vector<int> fbuffer);
    vector<Ekg_Data> FalseDetection(vector<Ekg_Data> fbuffer);
    vector<double> FalseDetection(vector<double> fbuffer);
    vector<int> FalseDetection(vector<int> fbuffer);
    vector<double> f_rate();
    vector<Ekg_Data> sigInfo;
    int maxHBR;
    int maxABR;
    vector<int> pr , widthQRS , R2R , Q2T ;
    vector<double> rate , Qamp , Ramp , Samp , Tamp , Pamp, ekgPower;
    //Feutue:
    double varQRS , varQT , varPR , varR2R , varQamp , varRamp, varSamp , varTamp,varPamp , VARekgPower;
};

#endif // FEUTUREEXTRACTION_H
