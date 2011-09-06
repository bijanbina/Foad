#ifndef QRSDET_H
#define QRSDET_H
#define EKG_SHIFT 22
#include "ekgnn.h"

struct EkgComplex
{
    int start;
    int end;
};


class QRSDet
{
public:
    QRSDet(double *array , int size);
    QRSDet(vector<double> array);
    vector<double> getDetected();
    vector<double> getFilter();
    vector<EkgComplex> getComplex();
private:
    int MAX(vector<double> input);
    int MIN(vector<double> input);
    vector<double> pow2(vector<double> input);
    vector<double> signal;
    vector<double> filter;
    vector<double> detected;
};

#endif // QRSDET_H

