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
    double st_seg;
    double pdetected;
    double qdetected;
    double rdetected;
    double qsdetected;
    double sqsdetected;
    double sdetected;
    double tdetected;


};
struct detection_check{
    double p ;
    double q ;
    double qs;
    double r ;
    double s ;
    double t ;
};
struct rhythm_data{
    double RRvar;
    vector <int> QRSWidth;
    double heartrate;
    vector<int> Parea;
    double Pentity;  // spell like P + entity
    vector<int> peak2through;
    double varParea; //variance of P area
    vector<int> EKGPower;
    vector<int> P2R;
    double varP2R;
    vector<int> RR;
    vector <bool> NSRvec;
};

class r_feature
{
public:
    r_feature(vector<Ekg_Data> input);
    EKG_atr getFueture();
    weka_data getWeka();
    rhythm_data getRhythm_data();
private:
    rhythm_data rhythminit;
    void setrhythm_data();
    void normalize();
    normal_data normaldata;
    double make_normal(vector<double> data);
    double make_normal(vector<int> data);
    EKG_atr atrribute;
    void set_atr();
    void f_avr_rate();
    double Miangin(vector<double> rbuffer);
    vector<double> rate , Qamp , Ramp , Samp , Tamp , Pamp, ekgPower , QTc;
    double getVariance(vector<double> fbuffer);
    double getVariance(vector<int> fbuffer);
    double Miangin(vector<int> fbuffer);
    vector<Ekg_Data> FalseDetection(vector<Ekg_Data> fbuffer);
    vector<double> FalseDetection(vector<double> fbuffer);
    vector<int> FalseDetection(vector<int> fbuffer);
    vector<Ekg_Data> sigInfo;
    int maxHBR;
    int maxABR;
    double hbr;
    vector<double> pr , QRSwidth , R2R , Q2T , st_seg , Q_pathologic , Uwave , elevated_ST , depressed_ST , Pwave_area , Qduration , Pduration, pr_seg;
    vector<bool> NormalSinusRhythm;
    //Feutue:
    unsigned int pcount,qcount,qscount,rcount,scount,tcount;
};

#endif



