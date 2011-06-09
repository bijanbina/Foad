#include "ekgsignal.h"

EKGSignal::EKGSignal(QString path)
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
bool EKGSignal::open(int record)
{
    //----------------------Variable------------------------
    WFDB_Siginfo signal_info[2] ;
    //
    QString recordBuffer;
    recordBuffer.setNum(record);
    strcpy(RecordNumber, recordBuffer.toStdString().c_str());
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
}
bool read(int time , int startTime = 0)
{

}



















