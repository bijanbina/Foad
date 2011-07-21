/* EKG Neural Network File that use fann library can detect disase from EKG Feature
  author:Bijan Binaee , Reza Faramarzi
  date : 2011 27 June
  __________________________________________________________________________
  Revisions:
        7/21: Add new function(Train + signals + save)
*/
#ifndef EKGNN_H
#define EKGNN_H
#include "qscan.h"


class EKGNN : public QObject
{
    Q_OBJECT
public:
    explicit EKGNN(QObject *parent = 0);
    explicit EKGNN(QString path,QObject *parent = 0);
    void Train(QString path,int epoch);
    void openNN(QString path);
    vector<double> getOutPut(weka_data inputData,int outputSize);
    void save(QString path);
    void create(int num_layers, int num_input,int num_neurons_hidden,int num_output);
private:
    char const *trainPatch;
    bool isStarted;
    struct fann *ann;
signals:
    void report(int value);

};

#endif // EKGNN_H
