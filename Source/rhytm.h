#ifndef RHYTM_H
#define RHYTM_H
#include "header.h"
struct rhythmstatus{
    bool tachycardia;
    bool bradycardia;
    bool normalrate;
    vector<int> PVCvec;
    vector<int> PACvec;
    bool regular;
    vector<int> NSR;

};
class rhytm
{
public:
    rhytm(rhythm_data inputdata);
    rhythmstatus state;//what is patient state ?
    static int PVCth = 100;//spell : pvc + th'reshold'  " the threshold that we say the beat is PVC "
    static int PACth = 100;//as same as up  : PAC threshold
    static int regth = 100;// regularity threshold

};

#endif // RHYTM_H
