#include "r_feature.h"

r_feature::r_feature(vector<Ekg_Data> input )
{
   // sigInfo = FalseDetection(input);
    sigInfo = input ;

    //-------------pwave initialize--------------
    pr        = vector<double> (sigInfo.size());
    //-------------------------------------------

    R2R       = vector<double> (sigInfo.size());
    Q2T       = vector<double> (sigInfo.size());
    st_seg    = vector<double> (sigInfo.size());

    //------------QRS initialize------------------
    QRSwidth  = vector<double> (sigInfo.size());
    Qamp      = vector<double> (sigInfo.size());
    ekgPower  = vector<double> (sigInfo.size());
    Ramp      = vector<double> (sigInfo.size());
    Samp      = vector<double> (sigInfo.size());
    //---------------------------------------------
    Tamp      = vector<double> (sigInfo.size());
    Pamp      = vector<double> (sigInfo.size());
    //------------Duration Initialize--------------
    Pduration  = vector<double> (sigInfo.size());
    st_seg     = vector<double> (sigInfo.size());
    Q_pathologic = vector<double> (sigInfo.size());
    elevated_ST  = vector<double> (sigInfo.size());
    depressed_ST = vector<double> (sigInfo.size());
    Pwave_area   = vector<double> (sigInfo.size());
    Qduration    = vector<double> (sigInfo.size());
    pr_seg       = vector<double> (sigInfo.size());
    QTc          = vector<double> (sigInfo.size());


    //high level features:
    Uwave = vector <double> (sigInfo.size());
    NormalSinusRhythm = vector <bool> (sigInfo.size());


    pcount=0;
    qcount=0;
    qscount=0;
    rcount=0;
    scount=0;
    tcount=0;
    //----------------det init------------------------
    for(int j =0 ; j < sigInfo.size() ; j++)
    {
        if (sigInfo[j].q.detect != NOTDETECTED)
            qcount++;
        if (sigInfo[j].qs.detect != NOTDETECTED)
            qscount++;
        if (sigInfo[j].r.detect != NOTDETECTED)
            rcount++;
        if (sigInfo[j].s.detect != NOTDETECTED)
            scount++;
        if (sigInfo[j].t.detect != NOTDETECTED)
            tcount++;
        pcount += sigInfo[j].Pcount;
    }


    for(int i = 0 ; i < sigInfo.size(); i++)
    {
        //ADD R Voltage
        if (sigInfo[i].r.detect != NOTDETECTED)
            Ramp[i] = sigInfo[i].r.voltage;
        else
            Ramp[i] = NOTDETECTED;
        //ADD P
        if (sigInfo[i].Pcount == 1)
        {
            Pamp[i] = sigInfo[i].p[0].voltage;
            Pduration[i] = sigInfo[i].p[0].end - sigInfo[i].p[0].end;
        }
        else
        {
            pr[i] = NOTDETECTED;
            Pamp[i] = NOTDETECTED;
            Pduration[i] = NOTDETECTED;
        }
        if (sigInfo[i].Pcount == 1 && sigInfo[i].r.detect != NOTDETECTED)
        {

            pr[i] = sigInfo[i].r.detect - sigInfo[i].p[0].start;
            pr_seg[i] = sigInfo[i].r.detect - sigInfo[i].p[0].end;
        }
        else
        {
            pr[i] = NOTDETECTED;
            pr_seg[i] = NOTDETECTED;

        }
        //Add QRS Width
        if (sigInfo[i].q.detect != NOTDETECTED)
        {
            Qamp[i] = sigInfo[i].q.voltage;
            Samp[i] = sigInfo[i].s.voltage;
            QRSwidth[i] = sigInfo[i].s.end - sigInfo[i].q.start;
            ekgPower[i] = Ramp[i] - Qamp[i];
        }
        else
        {
            QRSwidth[i] = sigInfo[i].qs.start - sigInfo[i].qs.end;
        }
        //ADD R-R
        R2R[i] = sigInfo[i].RR;
        //ADD QT
        if(sigInfo[i].t.detect != NOTDETECTED)
        {
            if (sigInfo[i].q.detect != NOTDETECTED)
            {
                Q2T[i] = sigInfo[i].q.start - sigInfo[i].t.end;
                QTc[i] = Q2T[i]/sqrt(R2R[i]);
            }
            else
            {
                Q2T[i] = sigInfo[i].qs.start - sigInfo[i].t.end;
                QTc[i] = Q2T[i]/sqrt(R2R[i]);
            }
            Tamp[i] = sigInfo[i].t.voltage;
        }
        else
        {
            Q2T[i] = NOTDETECTED;
            QTc[i] = NOTDETECTED;
        }
        if(sigInfo[i].t.detect != NOTDETECTED)
        {
            if (sigInfo[i].s.detect != NOTDETECTED)
                st_seg[i] = sigInfo[i].s.end - sigInfo[i].t.start;
            else
               st_seg[i] = sigInfo[i].qs.end - sigInfo[i].t.start;
        }
        else
           st_seg[i] = NOTDETECTED;
        if (sigInfo[i].q.detect != NOTDETECTED)
        {
            Qduration[i] = sigInfo[i].q.end - sigInfo[i].q.start;
        }
        else
        {
            Qduration[i] = NOTDETECTED;
        }
        if (Qduration[i] != NOTDETECTED && sigInfo[i].qs.detect == NOTDETECTED )
        {
            if (Qduration[i]/200 > 0.04 || abs(Qamp[i]) > Ramp[i]/3)
                Q_pathologic[i]=Qduration[i];
            else
                Q_pathologic[i]=NOTDETECTED;
        }
        else
            Qduration[i]=NOTDETECTED;
        Uwave[i]=sigInfo[i].u.voltage;
        Pwave_area[i]=sigInfo[i].p_area;

    }

    ekgPower = FalseDetection(ekgPower);
    pr = FalseDetection(pr);
    R2R = FalseDetection(R2R);
    Q2T = FalseDetection(Q2T);
    QRSwidth = FalseDetection(QRSwidth);
    Qamp = FalseDetection(Qamp);
    Ramp = FalseDetection(Ramp);
    Samp = FalseDetection(Samp);
    Tamp = FalseDetection(Tamp);
    Pamp = FalseDetection(Pamp);
    st_seg = FalseDetection(st_seg);
    hbr = 12000/Miangin(R2R);
    setrhythm_data();
    set_atr();
    normalize();
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
    temp.Qamp = normaldata.Q_amp ;
    temp.pdetected = normaldata.pdetected ;
    temp.sqsdetected = normaldata.sqsdetected ;
    temp.tdetected = normaldata.tdetected;
    temp.Ramp = normaldata.R_amp;
    temp.disease = "not";
    return temp;
}
void r_feature::set_atr()
{
    atrribute.PR_interval=Miangin(pr);
    atrribute.RR_interval = Miangin(R2R);
    atrribute.QT_interval = Miangin(Q2T);
    atrribute.QRScomplex_interval = Miangin(QRSwidth);
    atrribute.Q_amp = Miangin(Qamp);
    atrribute.P_amp = Miangin(Pamp);
    atrribute.R_amp = Miangin(Ramp);
    atrribute.S_amp = Miangin(Samp);
    atrribute.T_amp = Miangin(Tamp);
    atrribute.EKGpower=Miangin(ekgPower);
    atrribute.Heart_beat_ven = hbr ;
}
EKG_atr r_feature::getFueture()
{
    return atrribute;
}
double r_feature::make_normal(vector<double> data)
{
    double mean = Miangin(data);
    double normal_data;
    double enheraf = sqrt(getVariance(data));
    normal_data = enheraf/mean ;
        return normal_data;
}
double r_feature::make_normal(vector<int> data)
{
    double mean = Miangin(data);
    double normal_data;
    double enheraf = sqrt(getVariance(data));
    normal_data = enheraf/mean ;
        return normal_data;
}
void r_feature::normalize()
{
    normaldata.R_amp=(Miangin(Ramp)-20)/600;
    if (normaldata.R_amp > 1)
        normaldata.R_amp = 1;
    else if(normaldata.R_amp <0 )
        normaldata.R_amp=0;
    normaldata.tdetected = tcount / hbr ;
    normaldata.pdetected = pcount / hbr ;
    normaldata.sqsdetected = (scount + qscount) / hbr ;
    normaldata.Q_amp = (Miangin(Qamp) +57)/87;
    if (normaldata.Q_amp > 1)
        normaldata.Q_amp = 1;
    else if(normaldata.Q_amp <0 )
        normaldata.Q_amp=0;
}
void r_feature::setrhythm_data()
{
    //rhythminit.EKGPower.resize(ekgPower.size());

    rhythminit.EKGPower.assign(ekgPower.begin(),ekgPower.end());
    rhythminit.P2R.assign(pr.begin(),pr.end());
    rhythminit.Parea.assign(Pwave_area.begin(),Pwave_area.end());
    rhythminit.QRSWidth.assign(QRSwidth.begin(),QRSwidth.end());
    rhythminit.heartrate = hbr;
    rhythminit.RRvar= getVariance(R2R);
    rhythminit.varP2R = getVariance(pr);
    rhythminit.RR.assign(R2R.begin(),R2R.end());
    rhythminit.NSRvec.assign(NormalSinusRhythm.begin() , NormalSinusRhythm.end());

}
rhythm_data r_feature::getRhythm_data()
{
    return rhythminit;
}





