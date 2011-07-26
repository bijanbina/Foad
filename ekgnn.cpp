#include "ekgnn.h"

EKGNN::EKGNN(QObject *parent) :
    QThread(parent)
{
    isStarted = false;
}

EKGNN::EKGNN(QString path,QObject *parent) :
    QThread(parent)
{
    ann = fann_create_from_file(path.toStdString().c_str());
    isStarted = true;
}

void EKGNN::Train(QString path,int epoch)
{
    if(!isStarted)
        return;
    trainPatch = path.toStdString().c_str();
    epochNum = epoch / UPDATE_PER_EPCOCH;
}

void EKGNN::open(QString path)
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

int EKGNN::getLayerNum()
{
    if(!isStarted)
        return 0;
    return fann_get_num_layers(ann);
}

int EKGNN::getInputNum()
{
    if(!isStarted)
        return 0;
    return fann_get_num_input(ann);
}

int EKGNN::getHiddenNeuronsNum()
{
    if(!isStarted)
        return 0;
    int total = getNeuronsNum();
    int ioNeurons = getOutputNeuronsNum() + getInputNum();
    return total - ioNeurons;
}

int EKGNN::getOutputNeuronsNum()
{
    if(!isStarted)
        return 0;
    return fann_get_num_output(ann);
}

int EKGNN::getNeuronsNum()
{
    if(!isStarted)
        return 0;
    return fann_get_total_neurons(ann);
}

double EKGNN::getLearningRate()
{
    if(!isStarted)
        return 0;
    return fann_get_learning_rate(ann);
}

double EKGNN::getlearningMomentum()
{
    if(!isStarted)
        return 0;
    return fann_get_learning_momentum(ann);
}

bool EKGNN::operator!()
{
    return !isStarted;
}

bool EKGNN::Started()
{
    return isStarted;
}

QString EKGNN::getHiddenAF()
{
    int a = fann_get_activation_function(ann,1,0);
    QString returnData;
    switch (a)
    {
    case FANN_LINEAR:
        returnData = "Linear";
        break;
    case FANN_THRESHOLD:
        returnData = "Treshold";
        break;
    case FANN_THRESHOLD_SYMMETRIC:
        returnData = "Treshold Symmetric";
        break;
    case FANN_SIGMOID:
        returnData = "Sigmoid";
        break;
    case FANN_SIGMOID_STEPWISE:
        returnData = "Sigmoid Stepwise";
        break;
    case FANN_SIGMOID_SYMMETRIC:
        returnData = "Sigmoid Symmetric";
        break;
    case FANN_SIGMOID_SYMMETRIC_STEPWISE:
        returnData = "Sigmoid Symmetric Stepwise";
        break;
    case FANN_GAUSSIAN:
        returnData = "Gaussian";
        break;
    case FANN_GAUSSIAN_SYMMETRIC:
        returnData = "Gaussian Symmetric";
        break;
    case FANN_GAUSSIAN_STEPWISE:
        returnData = "Gaussian Stepwise";
        break;
    case  FANN_ELLIOT:
        returnData = "Elliot";
        break;
    case FANN_ELLIOT_SYMMETRIC:
        returnData = "Elliot Symmetric";
        break;
    case FANN_LINEAR_PIECE:
        returnData = "Linear Piece";
        break;
    case FANN_LINEAR_PIECE_SYMMETRIC:
        returnData = "Linear Symmetric";
        break;
    case FANN_SIN_SYMMETRIC:
        returnData = "Sinus Symmetric";
        break;
    case FANN_COS_SYMMETRIC:
        returnData = "Cosinus Symmetric";
        break;
    case FANN_SIN:
        returnData = "Sinus";
        break;
    case FANN_COS:
        returnData = "Cosinus";
        break;
    }
    return returnData;
}

QString EKGNN::getOutputAF()
{
    int layerNum = getLayerNum();
    int a = fann_get_activation_function(ann,layerNum - 1,0);
    QString returnData;
    switch (a)
    {
    case FANN_LINEAR:
        returnData = "Linear";
        break;
    case FANN_THRESHOLD:
        returnData = "Treshold";
        break;
    case FANN_THRESHOLD_SYMMETRIC:
        returnData = "Treshold Symmetric";
        break;
    case FANN_SIGMOID:
        returnData = "Sigmoid";
        break;
    case FANN_SIGMOID_STEPWISE:
        returnData = "Sigmoid Stepwise";
        break;
    case FANN_SIGMOID_SYMMETRIC:
        returnData = "Sigmoid Symmetric";
        break;
    case FANN_SIGMOID_SYMMETRIC_STEPWISE:
        returnData = "Sigmoid Symmetric Stepwise";
        break;
    case FANN_GAUSSIAN:
        returnData = "Gaussian";
        break;
    case FANN_GAUSSIAN_SYMMETRIC:
        returnData = "Gaussian Symmetric";
        break;
    case FANN_GAUSSIAN_STEPWISE:
        returnData = "Gaussian Stepwise";
        break;
    case  FANN_ELLIOT:
        returnData = "Elliot";
        break;
    case FANN_ELLIOT_SYMMETRIC:
        returnData = "Elliot Symmetric";
        break;
    case FANN_LINEAR_PIECE:
        returnData = "Linear Piece";
        break;
    case FANN_LINEAR_PIECE_SYMMETRIC:
        returnData = "Linear Symmetric";
        break;
    case FANN_SIN_SYMMETRIC:
        returnData = "Sinus Symmetric";
        break;
    case FANN_COS_SYMMETRIC:
        returnData = "Cosinus Symmetric";
        break;
    case FANN_SIN:
        returnData = "Sinus";
        break;
    case FANN_COS:
        returnData = "Cosinus";
        break;
    }
    return returnData;
}

void EKGNN::run()
{
    fann_train_data *data = fann_read_train_from_file(trainPatch);
    //Start Learn
    for(int i =1;i<epochNum;i++)
    {
        emit report(fann_train_epoch(ann,data));
        for(int j = 1;j<UPDATE_PER_EPCOCH;j++)
            fann_train_epoch(ann,data);
    }

}









