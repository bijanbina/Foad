/* EKG Neural Network File that use fann library can detect disase from EKG Feature
  author:Bijan Binaee , Reza Faramarzi
  date : 2011 27 June
  __________________________________________________________________________
  Revisions:
        07/21: Add new function(Train + signals + save)
        07/22: Add getLayerNum + getInputNum + getHiddenNeuronsNum + getOutputNeuronsNum
        07/27: Create NNSetting Struct;
*/
#ifndef EKGNN_H
#define EKGNN_H
#define UPDATE_PER_EPCOCH 10
#include "qscan.h"

struct NNSetting
{
    int HiddenNeuronsNum;
    int LayerNum;
    int OutputNeuronsNum;
    int InputNeuronsNum;
}

class EKGNN : public QThread
{
    Q_OBJECT
public:
    explicit EKGNN(QObject *parent = 0);
    explicit EKGNN(QString path,QObject *parent = 0);
    void     Train(QString path,int epoch);
    void     open(QString path);
    vector<double> getOutPut(weka_data inputData,int outputSize);
    void     save(QString path);
    void     create(int num_layers, int num_input,int num_neurons_hidden,int num_output);
    int      getLayerNum();
    int      getInputNum();
    int      getHiddenNeuronsNum();
    int      getOutputNeuronsNum();
    int      getNeuronsNum();
    double   getLearningRate();
    double   getlearningMomentum();
    bool     operator !();
    bool     Started();
    QString  getHiddenAF();
    QString  getOutputAF();
private:
    void         run();
    char const  *trainPatch;
    bool         isStarted;
    struct fann *ann;
    int          epochNum;
signals:
    void report(double value);

};

#endif // EKGNN_H
