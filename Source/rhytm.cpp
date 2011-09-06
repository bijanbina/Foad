#include "rhytm.h"



rhytm::rhytm(rhythm_data inputdata)
{
    int difbuffer;
    if (inputdata.heartrate > 100)
        state.tachycardia = true ;
    else if(inputdata.heartrate < 60)
        state.bradycardia = true ;
    else
        state.normalrate = true ;
    //----------have we pvc ? ------------
    for(int i = 0 ;  i < inputdata.RR.size(); i++ )
    {
        difbuffer = inputdata.RR[i+1] - inputdata.RR[i];
        if (abs(difbuffer) > PVCth )
        {
            state.PVCvec.push_back(i);
            i++;
        }
    }
    //----------have we PAC ? --------------
    for(int j = 0 ; j < inputdata.P2R ; j++)
    {
        difbuffer =inputdata.P2R[i+1] - inputdata.P2R[i];
        if (abs(difbuffer) > PACth)
        {
            state.PACvec.push_back(i);
            i++;
        }
    }
   //-----------is rhythm regular ? -----------
    if(inputdata.RRvar > regth)
        regular = false;
    else
        regular = true ;
    //-----------NSR processing -------------
    for(int var=0 ; var < inputdata.NSRvec.size() ; var++ )
    {
        if (inputdata.NSRvec[i] == true )
            state.NSR.push_back(i);
    }

}
