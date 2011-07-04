#include "qrsdet.h"

QRSDet::QRSDet(double *array , int size)
{
    signal = vector<double> (size);
    detected = vector<double> (0);
    for(int i = 0; i < size;i++)
        signal[i] = array[i];
}

vector<double> QRSDet::getDetected()
{
    int size = signal.size();
    filter = getFilter();
    detected = vector<double> (size,0);
    double threshold = filter[MAX(filter)] / 2;
    if(abs(filter[MIN(filter)] / 2) > threshold)
        threshold = filter[MIN(filter)] / 2;
    if (threshold > 0)
    {
        for(int i = 0; i < size;i++)
            if(filter[i] < threshold)
                filter[i] = 0;
    }
    else
    {
        for(int i = 0; i < size;i++)
            if(filter[i] > threshold)
                filter[i] = 0;
    }
    filter = pow2(filter);
    int mMax = MAX(filter);
    while(filter[mMax] > 0)
    {
        int k = mMax;
        while(filter[k] > 0)
        {
            filter[k] = 0;
            k--;
        }
        k = mMax + 1;
        while(filter[k] > 0)
        {
            filter[k] = 0;
            k++;
        }
        int detected_place  = mMax + EKG_SHIFT;
        if(!(detected_place < size))
            detected_place = size;
        detected[detected_place] = 1;
        mMax = MAX(filter);
    }
    return detected;
}

vector<double> QRSDet::getFilter()
{
    //Variuble
    int size = signal.size();
    vector<double> filter(size);
    fftw_plan Filter_Plan;
    fftw_complex input_complex[size];
    //Fill Complex
    for(int i = 0;i<size;i++)
    {
        input_complex[i][0] = signal[i] ;
        input_complex[i][1] = 0.0;
    }
    //FFt aNd Filtering
    Filter_Plan = fftw_plan_dft_1d(size, input_complex, input_complex, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(Filter_Plan);
    for(int i = 0;i<round(size * 5 / SAMPLE_RATE);i++)
        input_complex[i][0] = input_complex[i][1] = 0;
    for(int i = size - round(size * 5 / SAMPLE_RATE) ;i<size;i++)
        input_complex[i][0] = input_complex[i][1] = 0;
    Filter_Plan = fftw_plan_dft_1d(size, input_complex, input_complex, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(Filter_Plan);
    for (int i = 0;i<size;i++)
        filter[i] = input_complex[i][0] / size;
    //Free Memory
    fftw_destroy_plan(Filter_Plan);
    return filter;
}

int QRSDet::MAX(vector<double> input)
{
    int mMax = 0;
    for(int i = 0; i < input.size();i++)
        if (input[i] > input[mMax])
            mMax = i;
    return mMax;
}

int QRSDet::MIN(vector<double> input)
{
    int mMin = 0;
    for(int i = 0; i < input.size();i++)
        if (input[i] < input[mMin])
            mMin = i;
    return mMin;
}

vector<double> QRSDet::pow2(vector<double> input)
{
    for(int i = 0; i < input.size();i++)
        input[i] = pow(input[i],2);
    return input;
}

vector<EkgComplex> QRSDet::getComplex()
{
    vector<EkgComplex> Sig_Complex(0);
    EkgComplex listBuffer;
    int k = 0;
    if (detected.size() < 1)
        detected = getDetected();
    for(int i = 0;i<detected.size();i++)
    {
        if(detected[i] == 1)
        {
            if (k > 0)
            {
                listBuffer.start = listBuffer.end;
                listBuffer.end = i;
                Sig_Complex.push_back(listBuffer);
            }
            else
            {
                listBuffer.end = i;
                k++;
            }
        }
    }
    return Sig_Complex;
}























