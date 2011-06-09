#ifndef SIGDETECT_H
#define SIGDETECT_H
#include "bdac.h"

//Defines:
#define DETECTED 1001
#define NOTDETECTED -1

//QRS Detection Header

struct QFeuture
{
    int voltage;
    int start;
    int detect;
    int height;
    int width;
    int end;
};

struct Ekg_Data
{
    QFeuture qs;
    vector<QFeuture> p;
    QFeuture q;
    QFeuture r;
    QFeuture s;
    QFeuture t;
    int start;
    int u;
    int delta;
    int age;
    int Rythm;
    int Pcount;
    int RR;
};

class SigDetect
{
public:
    SigDetect();
    SigDetect(vector<double> signal , bool getPlot , QwtPlot *plotwidget ,  int id , int EKG_Age , int start);
    Ekg_Data getInfo();
    //QwtPlot getPlot();
private:
    void plot(double *Signal,double *Detect ,int size);
    void ZeroLine();
    void ZeroShib();
    bool AllDetected();
    void Pow2();
    void ABS();
    void SQRT();
    int MAX();
    double Miangin(vector<double>);
    double getLine(vector<double>);
    double getShib(vector<double> fbuffer);
    double getVariance(vector<double>);
    void GUSSIOn(int reduce);
    void findBeat();
    bool findR();
    void findDelta();
    void findQS();
    void findQ();
    void findS();
    bool findP();
    bool findT();
    void MK_QRS_Zero();
    void MK_First_Zero();
    void TPrepare();
    //Functions for Plot
    void qCurve();
    void rCurve();
    void sCurve();
    void pCurve();
    void tCurve();
    //Signal Data
    vector <double> MainSig;
    vector <double> buffer;
    vector <int> sigBeat;
    Ekg_Data sigInfo;

    //Detector Plot
    QwtPlot *myPlot;
    QwtPlotZoomer *zoomer;
    QwtPlotCurve *Signal_curves;
    QwtPlotCurve *Detect_curves;
    QwtPlotCurve *q_curves;
    QwtPlotCurve *r_curves;
    QwtPlotCurve *s_curves;
    QwtPlotCurve *p_curves;
    QwtPlotCurve *t_curves;
    QwtPlotCurve *u_curves;

    QwtPlotGrid *EKG_Grid;
    //
    void NODetect(double replace);
    void setDetect(vector<double> input);
    vector<double> getDetect();
    void gussian(int reduce);
    vector<double> fastSmooth(vector<double> input , int width);
};

#endif // SIGDETECT_H
