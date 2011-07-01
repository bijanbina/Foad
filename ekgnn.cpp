#include "ekgnn.h"

EKGNN::EKGNN()
{
    trainPatch = "";
    NNPatch = "";
}

void EKGNN::setTrain(QString path)
{
    trainPatch = path;
}

void EKGNN::setNNPath(QString path)
{
    NNPatch = path;
}

void EKGNN::openNN()
{
    if (NNPatch.isEmpty())
        return;
    ann = fann_create_from_file("xor_float.net");
    isStarted = true;
}

double EKGNN::getOutPut(weka_data inputData)
{
    fann_type *calc_out;
    fann_type input[5];
    input[0] = inputData.sqsdetected;
    input[1] = inputData.pdetected;
    input[2] = inputData.tdetected;
    input[3] = inputData.Ramp;
    input[4] = inputData.Qamp;
    calc_out = fann_run(ann, input);
    return *calc_out;
}
