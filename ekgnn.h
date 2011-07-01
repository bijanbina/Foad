/* EKG Neural Network File that use fann library can detect disase from EKG Feature
  author:Bijan Binaee , Reza Faramarzi
  date : 2011 27 June
*/
#ifndef EKGNN_H
#define EKGNN_H
#include "ekgscan.h"
class EKGNN
{
public:
    EKGNN();
    void setTrain(QString path);
    void setNNPath(QString path);
    void openNN();
    double getOutPut(weka_data inputData);
private:
    QString trainPatch;
    QString NNPatch;
    bool isStarted;
    struct fann *ann;
};

#endif // EKGNN_H
