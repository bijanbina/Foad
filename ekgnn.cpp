#include "ekgnn.h"

EKGNN::EKGNN(QString path,QObject *parent) :
    QObject(parent)
{
    ann = fann_create_from_file(path.toStdString().c_str());
    isStarted = true;
}

void EKGNN::Train(QString path,int epoch)
{
    trainPatch = path.toStdString().c_str();
    fann_train_data *data = fann_read_train_from_file(trainPatch);
    //Start Learn
    for(int i =1;i<epoch;i++)
        emit report(fann_train_epoch(ann,data));
}

void EKGNN::openNN(QString path)
{
    ann = fann_create_from_file(path.toStdString().c_str());
    isStarted = true;
}

double EKGNN::getOutPut(weka_data inputData)
{
    if(!isStarted)
        return 0;
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
