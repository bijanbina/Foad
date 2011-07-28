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

typedef fann_activationfunc_enum AF_Enum;

struct NNSetting
{
    int     HiddenNeuronsNum;
    int     LayerNum;
    int     OutputNeuronsNum;
    int     InputNeuronsNum;
    double  LearningRate;
    double  LearningMomentum;
    AF_Enum HiddenAF;
    AF_Enum OutputAF;
    QString HiddenAFName;
    QString OutputAFName;
};

class EKGNN : public QThread
{
    Q_OBJECT
public:
    explicit       EKGNN(QObject *parent = 0);
    explicit       EKGNN(QString path,QObject *parent = 0);
    void           Train(QString path,int epoch);
    void           open(QString path);
    vector<double> getOutPut(weka_data inputData,int outputSize);
    void           save(QString path);
    void           create(int num_layers, int num_input,int num_neurons_hidden,int num_output);
    void           create(NNSetting parm);
    int            getLayerNum();
    int            getInputNum();
    int            getHiddenNeuronsNum();
    int            getOutputNeuronsNum();
    int            getNeuronsNum();
    double         getLearningRate();
    double         getLearningMomentum();
    bool           operator !();
    bool           Started();
    QString        getHiddenAF();
    QString        getOutputAF();
    AF_Enum        getHiddenAF_Enum();
    AF_Enum        getOutputAF_Enum();
    NNSetting      getSetting();
    QStringList    getAFlist();
    void           setLearningRate(double Learning_Rate);
    void           setLearningMomentum(double Learning_Momentom);
    void           setHiddenAF(AF_Enum AF);
    void           setOutputAF(AF_Enum AF);
private:
    void           run();
    char const    *trainPatch;
    bool           isStarted;
    struct fann   *ann;
    int            epochNum;
    NNSetting      Parametr;
signals:
    void           report(double value);

};

#endif // EKGNN_H
