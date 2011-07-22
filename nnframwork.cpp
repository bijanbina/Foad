#include "nnframwork.h"

NNframwork::NNframwork(QWidget *parent) :
    QWidget(parent)
{
    myPlot = new QwtPlot();
}

void createLayout()
{

}

void NNframwork::createInfo(QString name , int value)
{
    QString valueBuffer;
    valueBuffer.setNum(value);
    createInfo(name,valueBuffer);
}
void NNframwork::createInfo(QString name , QString value)
{
    H_Layout = new QHBoxLayout;
    label = new QLabel(name);
    fWidget = new QGroupBox;
    H_Layout->addWidget(label);
    label = new QLabel(value);
    H_Layout->addWidget(label);
    fWidget->setLayout(H_Layout);
}
