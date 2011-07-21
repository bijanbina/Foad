#include "ekgnn.h"

EKGNN::EKGNN(QObject *parent) :
    QObject(parent)
{
    isStarted = false;
}

EKGNN::EKGNN(QString path,QObject *parent) :
    QObject(parent)
{
    ann = fann_create_from_file(path.toStdString().c_str());
    isStarted = true;
}

void EKGNN::Train(QString path,int epoch)
{
    if(!isStarted)
        return;
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

vector<double> EKGNN::getOutPut(weka_data inputData,int outputSize)
{
    //for copy array to vector
    vector<double> output(outputSize);
    if(!isStarted)
        return output;
    fann_type *calc_out;
    fann_type input[5];
    input[0] = inputData.sqsdetected;
    input[1] = inputData.pdetected;
    input[2] = inputData.tdetected;
    input[3] = inputData.Ramp;
    input[4] = inputData.Qamp;
    calc_out = fann_run(ann, input);
    //copy array to vector
    for(int i = 0;i<outputSize;i++)
    output[i] = calc_out[i];
    return output;
}

void EKGNN::save(QString path)
{
    if(!isStarted)
        return;
    const char *savePath = path.toStdString().c_str();
    fann_save(ann, savePath);
}

void EKGNN::create(int num_layers, int num_input,int num_neurons_hidden,int num_output)
{
    ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
    isStarted = true;
}



















