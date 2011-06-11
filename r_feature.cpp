#include "r_feature.h"
r_feature::r_feature(vector<Ekg_Data> input )
{
    sigInfo = FalseDetection(input);
    //maxHBR = 206.3 - 0.711 * input[0].age;
    //maxABR = 120 + maxHBR;
    pr = vector<int> (sigInfo.size());
    R2R = vector<int> (sigInfo.size());
    Q2T = vector<int> (sigInfo.size());
    widthQRS = vector<int> (sigInfo.size());
    Qamp = vector<double> (sigInfo.size());
    ekgPower = vector<double> (sigInfo.size());
    Ramp = vector<double> (sigInfo.size());
    Samp = vector<double> (sigInfo.size());
    Tamp = vector<double> (sigInfo.size());
    Pamp = vector<double> (sigInfo.size());
    for(int i = 0 ; i < sigInfo.size(); i++)
    {
        //ADD R Voltage
        Ramp[i] = sigInfo[i].r.voltage;
        //ADD P
        if (sigInfo[i].Pcount == 1)
        {
            Pamp[i] = sigInfo[i].p[0].voltage;
            pr[i] = sigInfo[i].r.detect - sigInfo[i].p[0].detect;
        }
        else
        {
            pr[i] = NOTDETECTED;
            Pamp[i] = NOTDETECTED;
        }
        //Add QRS Width
        if (sigInfo[i].q.detect != NOTDETECTED)
        {
            Qamp[i] = sigInfo[i].q.voltage;
            Samp[i] = sigInfo[i].s.voltage;
            widthQRS[i] = sigInfo[i].s.end - sigInfo[i].q.start;
            ekgPower[i] = Ramp[i] - Qamp[i];
        }
        else
        {
            widthQRS[i] = sigInfo[i].qs.start - sigInfo[i].qs.end;
        }
        //ADD R-R
        R2R[i] = sigInfo[i].RR;
        //ADD QT
        if(sigInfo[i].t.detect != NOTDETECTED)
        {
            if (sigInfo[i].q.detect != NOTDETECTED)
                Q2T[i] = sigInfo[i].q.detect - sigInfo[i].t.detect;
            else
                Q2T[i] = sigInfo[i].qs.detect - sigInfo[i].t.detect;
            Tamp[i] = sigInfo[i].t.voltage;
        }
        else
            Q2T[i] = NOTDETECTED;
    }
    pr = FalseDetection(pr);
    R2R = FalseDetection(R2R);
      Q2T = FalseDetection(Q2T);
    widthQRS = FalseDetection(widthQRS);
    Qamp = FalseDetection(Qamp);
    Ramp = FalseDetection(Ramp);
    Samp = FalseDetection(Samp);
    Tamp = FalseDetection(Tamp);
    Pamp = FalseDetection(Pamp);
    varQRS = getVariance(widthQRS) / Miangin(widthQRS);
    varQT = getVariance(Q2T) / Miangin(Q2T);
    varPR = getVariance(pr) / Miangin(pr);
    varR2R = getVariance(R2R) / Miangin(R2R);
    varQamp = fabs(getVariance(Qamp) / Miangin(Qamp));
    varRamp = getVariance(Ramp) / Miangin(Ramp);
    varSamp = fabs(getVariance(Samp) / Miangin(Samp));//9128111966
    varTamp = fabs(getVariance(Tamp) / Miangin(Tamp));
    varPamp = getVariance(Pamp) / Miangin(Pamp);
    VARekgPower = getVariance(ekgPower) / Miangin(ekgPower);
    set_atr();
    normaldata.P_amp=make_normal(Pamp);
    normaldata.Q_amp=make_normal(Qamp);
    normaldata.R_amp=make_normal(Ramp);
    normaldata.S_amp=make_normal(Samp);
    normaldata.T_amp=make_normal(Tamp);
    normaldata.EKGpower=make_normal(ekgPower);
    normaldata.PR_interval=make_normal(pr);
    normaldata.RR_interval = make_normal(R2R);
    normaldata.QT_interval = make_normal(Q2T);
    normaldata.QRScomplex_interval = make_normal(widthQRS);
}
double r_feature::getVariance(vector<double> fbuffer)
{
    if (fbuffer.size() > 0)
    {
        double s = 0;
        double variance = 0;
        for (int i = 0 ; i < fbuffer.size() ; i++)
        {
            s += fbuffer[i];
        }
        s /= fbuffer.size();
        for (int i = 0 ; i < fbuffer.size() ; i++)
        {
            variance += pow(fbuffer[i] - s,2);
        }
        variance /= fbuffer.size();
        variance = sqrt(variance);
        return variance;
    }
    return NOTDETECTED;
}
double r_feature::getVariance(vector<int> fbuffer)
{
    if (fbuffer.size() > 0)
    {
        double s = 0;
        double variance = 0;
        for (int i = 0 ; i < fbuffer.size() ; i++)
        {
            s += fbuffer[i];
        }
        s /= fbuffer.size();
        for (int i = 0 ; i < fbuffer.size() ; i++)
        {
            variance += pow(fbuffer[i] - s,2);
        }
        variance /= fbuffer.size();
        variance = sqrt(variance);
        return variance;
    }
    return NOTDETECTED;
}
double r_feature::Miangin(vector<double> rbuffer)
{
    double s = 0, tedad = 0;
    for (int i = 0 ; i < rbuffer.size(); i++)
    {
        if (rbuffer[i] != DETECTED)
        {
            tedad++;
            s += rbuffer[i];
        }
    }
    if (tedad != 0)
        s /= tedad;
    return s;
}

double r_feature::Miangin(vector<int> fbuffer)
{
    double s = 0, tedad = 0;
    for (int i = 0 ; i < fbuffer.size(); i++)
    {
        if (fbuffer[i] != DETECTED)
        {
            tedad++;
            s += fbuffer[i];
        }
    }
    if (tedad != 0)
        s /= tedad;
    return s;
}
vector<Ekg_Data> r_feature::FalseDetection(vector<Ekg_Data> fbuffer)
{
    for (int i = 0 ; i < fbuffer.size(); i++)
    {
        if(fbuffer[i].r.detect == NOTDETECTED && fbuffer[i].qs.detect == NOTDETECTED)
            fbuffer.erase(fbuffer.begin()+i);
    }
    return fbuffer;
}
vector<double> r_feature::FalseDetection(vector<double> fbuffer)
{
    for (int i = 0 ; i < fbuffer.size(); i++)
    {
        if(fbuffer[i] == NOTDETECTED)
            fbuffer.erase(fbuffer.begin()+i);
    }
    return fbuffer;
}
vector<int> r_feature::FalseDetection(vector<int> fbuffer)
{
    for (int i = 0 ; i < fbuffer.size(); i++)
    {
        if(fbuffer[i] == NOTDETECTED)
        {
            fbuffer.erase(fbuffer.begin()+i);
            i--;
        }
    }
    return fbuffer;
}
weka_data r_feature::getWeka()
{
    weka_data temp;
    temp.PR = Miangin(normaldata.PR_interval);
    temp.Q2T = Miangin(normaldata.QT_interval);
    temp.QRS = Miangin(normaldata.QRScomplex_interval);
    temp.Ramp = Miangin(normaldata.R_amp);
    temp.rate = Miangin(normaldata.Heart_beat_ven);
    temp.EKGpower = Miangin(normaldata.EKGpower);
    temp.disease = "not";
    return temp;
}
void r_feature::set_atr()
{
    atrribute.PR_interval=Miangin(pr);
    atrribute.RR_interval = Miangin(R2R);
    atrribute.QT_interval = Miangin(Q2T);
    atrribute.QRScomplex_interval = Miangin(widthQRS);
    atrribute.Q_amp = Miangin(Qamp);
    atrribute.P_amp = Miangin(Pamp);
    atrribute.R_amp = Miangin(Ramp);
    atrribute.S_amp = Miangin(Samp);
    atrribute.T_amp = Miangin(Tamp);
    atrribute.EKGpower=Miangin(ekgPower);
    atrribute.Heart_beat_ven = 12000.0 / atrribute.RR_interval ;
}
EKG_atr r_feature::getFueture()
{
    return atrribute;
}
vector <double> r_feature::make_normal(vector<double> data)
{
    double mean = Miangin(data);
    vector <double> normal_data (data.size());
    double enheraf = sqrt(getVariance(data));
    for (int i=0 ; i<data.size() ; i++)
    {
        normal_data.push_back((fabs(data[i] - mean ))/enheraf);
    }
        return normal_data;
}
vector <double> r_feature::make_normal(vector<int> data)
{
    double mean = Miangin(data);
    vector <double> normal_data (data.size());
    double enheraf = sqrt(getVariance(data));
    for (int i=0 ; i<data.size() ; i++)
    {
        normal_data.push_back((fabs(data[i] - mean ))/enheraf);
        if (normal_data[i] > 1.0)
            normal_data[i] = 1;
    }
        return normal_data;
}
