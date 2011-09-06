#include "ekgsignal.h"

EKGSignal::EKGSignal()
{
    strcpy(DB, "DB");
}

char *EKGSignal::getDB()
{
    return DB;
}

char *EKGSignal::getRecord()
{
    return RecordNumber;
}

void EKGSignal::setDB(QString path)
{
    strcpy(DB, path.toStdString().c_str());
}

int EKGSignal::gcd(int x, int y)
{
    while (x != y)
    {
        if (x > y)
            x-=y;
        else
            y -= x;
    }
    return (x);
}
bool EKGSignal::open(QString record)
{
    //----------------------Variable------------------------
    WFDB_Siginfo signal_info[2] ;
    strcpy(RecordNumber, record.toStdString().c_str());
    //------------------Configure Database------------------
    setwfdb(DB) ;
    //----------------------Check Signal--------------------
    if(isigopen(RecordNumber,signal_info,2) < 1)
    {
        exist = false;
        return false;
    }
    sigInfo = signal_info[0];
    exist = true;
    return true;
}
bool EKGSignal::read(int time , int Starttime)
{
    if (!exist) //is signal opened?
        return false;
    //----------------------Variable------------------------
    int BufferSize = time * SAMPLE_RATE , ecg[2];
    signal = vector<double> (BufferSize);
    long SampleCount = 0;
    //--------------Rading Signal Attributes----------------
    ADCZero = sigInfo.adczero ;
    ADCUnit = sigInfo.gain ;
    InputFileSampleFrequency = sampfreq(RecordNumber) ;
    //-------------------------Initialize-------------------
    NextSample(ecg,2,InputFileSampleFrequency,SAMPLE_RATE,true) ;
    SampleCount = 0 ;
    int SampleNumber = 0;
    //--------------------Start Reading Signal--------------
    while((NextSample(ecg,2,InputFileSampleFrequency,SAMPLE_RATE,false) >= 0) && (SampleCount < BufferSize))
    {
        SampleNumber++;
        if (SampleNumber >= Starttime )
        {
            ecg[0] -= ADCZero ;
            ecg[0] *= SAMPLE_RATE;
            if (ADCUnit != 0)
                ecg[0] /= ADCUnit ;
            else
                ecg[0] /= WFDB_DEFGAIN ;
            signal[SampleCount] = ecg[0] ;
            SampleCount++;
        }
    }
    //----------------------Check Signal--------------------
    if(SampleCount < time)
        return false;
    return true;
}

vector<double> EKGSignal::getSignal()
{
    return signal;
}

long int EKGSignal::nsamp()
{
    return sigInfo.nsamp;
}

int EKGSignal::NextSample(int *vout,int nosig,int ifreq, int ofreq,bool init)
{
    int i ;
    static int m, n, mn, ot, it, vv[WFDB_MAXSIG], v[WFDB_MAXSIG], rval ;

    if(init)
    {
        i = gcd(ifreq, ofreq);
        m = ifreq/i;
        n = ofreq/i;
        mn = m*n;
        ot = it = 0 ;
        getvec(vv) ;//readsample of the signal
        rval = getvec(v) ;
    }
    else
    {
        while(ot > it)
        {
            for(i = 0; i < nosig; ++i)
                vv[i] = v[i] ;
            rval = getvec(v) ;
            if (it > mn)
            {
                it -= mn;
                ot -= mn;
            }
            it += n;
        }
        for( i = 0; i < nosig; i++ )
            vout[i] = vv[i] + (ot%n)*(v[i]-vv[i])/n;
        ot += m;
    }

    return(rval) ;
}



















