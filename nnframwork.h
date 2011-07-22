#ifndef NNFRAMWORK_H
#define NNFRAMWORK_H

#include "qrsdet.h"

class NNframwork : public QWidget
{
    Q_OBJECT
public:
    explicit NNframwork(QWidget *parent = 0);

signals:

public slots:

private:
    //Function
    void createLayout();
    void createInfo(QString name , int value);
    void createInfo(QString name , QString value);
    //Plot
    QwtPlot      *myPlot;
    QwtPlotCurve *Signal_curves;
    //Layout
    QGridLayout  *info_layout;
    QVBoxLayout  *Mlayout;               //Main Layout
    QHBoxLayout  *H_Layout;              //use for create info
    QGroupBox    *fWidget;               //use for create info
    QPushButton  *button;
    QLabel       *label;
    //Neural Network
    EKGNN        *LocalNN;
};

#endif // NNFRAMWORK_H
