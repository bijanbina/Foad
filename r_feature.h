#ifndef R_FEATURE_H
#define R_FEATURE_H
#include "diseawriter.h"
struct EKG_atr
{
    double P_amp;
    double Q_amp;
    double R_amp;
    double S_amp;
    double T_amp;
    double QT_interval;
    double PR_interval;
    double QRScomplex_interval;
    double EKGpower;
    double Heart_beat_ven;
    double RR_interval;
};
struct normal_data
{
    double P_amp;
    double Q_amp;
    double R_amp;
    double S_amp;
    double T_amp;
    double QT_interval;
    double PR_interval;
    double QRScomplex_interval;
    double EKGpower;
    double Heart_beat_ven;
    double RR_interval;

};

class r_feature
{
public:
    r_feature(vector<Ekg_Data> input);
    EKG_atr getFueture();
    weka_data getWeka();
private:
    void writefeature(vector<double> feature);
    normal_data normaldata;
    double make_normal(vector <double> data);
    double make_normal(vector<int> data);
    EKG_atr atrribute;
    void set_atr();
    void f_avr_rate();
    double Miangin(vector<double> rbuffer);
    vector<double> rate , Qamp , Ramp , Samp , Tamp , Pamp, ekgPower;
    double getVariance(vector<double> fbuffer);
    double getVariance(vector<int> fbuffer);
    double Miangin(vector<int> fbuffer);
    vector<Ekg_Data> FalseDetection(vector<Ekg_Data> fbuffer);
    vector<double> FalseDetection(vector<double> fbuffer);
    vector<int> FalseDetection(vector<int> fbuffer);
    vector<Ekg_Data> sigInfo;
    int maxHBR;
    int maxABR;
    vector<int> pr , widthQRS , R2R , Q2T ;
    //Feutue:
    double varQRS , varQT , varPR , varR2R , varQamp , varRamp, varSamp , varTamp,varPamp , VARekgPower;
};

#endif



