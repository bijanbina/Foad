#ifndef EKGSIGNAL_H
#define EKGSIGNAL_H
#include "listrw.h"
class EKGSignal
{
public:
    EKGSignal();
    void setDB(QString path);
    char *getDB();
    char *getRecord();
    bool open(QString record);
    bool read(int time , int Starttime = 0);
    vector<double> getSignal();
private:
    int NextSample(int *vout,int nosig,int ifreq, int ofreq,bool init);
    int gcd(int x, int y);
    char RecordNumber[10];
    char DB[100];
    bool exist;
    WFDB_Siginfo sigInfo;
    vector<double> signal;
    ///
    int InputFileSampleFrequency;
    int EKG_age ,ADCZero ,ADCUnit;
};

#endif // EKGSIGNAL_H
