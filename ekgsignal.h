#ifndef EKGSIGNAL_H
#define EKGSIGNAL_H
#include "listrw.h"
class EKGSignal
{
public:
    EKGSignal(QString path);
    bool open(int record);
    bool read(int time , int startTime = 0);
private:
    int gcd(int x, int y);
    int NextSample(int *vout,int nosig,int ifreq, int ofreq,bool init);
    char RecordNumber[10];
    char DB[100];
    int sigTime;
    bool exist;
    WFDB_Siginfo sigInfo;
    ///
    double beatcount;
    BeatDetection *LocalBDAC;
    QRSDetection *LocalDetector;
    int InputFileSampleFrequency;
    int EKG_age ,ADCZero ,ADCUnit;
};

#endif // EKGSIGNAL_H
