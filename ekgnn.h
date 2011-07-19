/* EKG Neural Network File that use fann library can detect disase from EKG Feature
  author:Bijan Binaee , Reza Faramarzi
  date : 2011 27 June
*/
#ifndef EKGNN_H
#define EKGNN_H
#include "qscan.h"


class EKGNN : public QObject
{
    Q_OBJECT
public:
    explicit EKGNN(QString path,QObject *parent = 0);
    void Train(QString path,int epoch);
    void openNN(QString path);
    double getOutPut(weka_data inputData);

private:
    char const *trainPatch;
    bool isStarted;
    struct fann *ann;
signals:
    void report(int value);

};

#endif // EKGNN_H
