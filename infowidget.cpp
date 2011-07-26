#include "infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QGroupBox(parent)
{
    H_Layout = new QHBoxLayout;
    Namelabel = new QLabel("");
    H_Layout->addWidget(Namelabel);
    Valuelabel = new QLabel("");
    H_Layout->addWidget(Valuelabel);
    setLayout(H_Layout);
}

InfoWidget::InfoWidget(QString name , int value , QWidget *parent):
    QGroupBox(parent)
{
    //convert value to string
    QString valueBuffer;
    valueBuffer.setNum(value);

    H_Layout = new QHBoxLayout;
    Namelabel = new QLabel(name);
    H_Layout->addWidget(Namelabel);
    Valuelabel = new QLabel(valueBuffer);
    H_Layout->addWidget(Valuelabel);
    setLayout(H_Layout);
}

InfoWidget::InfoWidget(QString name , QString value , QWidget *parent):
    QGroupBox(parent)
{
    H_Layout = new QHBoxLayout;
    Namelabel = new QLabel(name);
    H_Layout->addWidget(Namelabel);
    Valuelabel = new QLabel(value);
    H_Layout->addWidget(Valuelabel);
    setLayout(H_Layout);
}

void InfoWidget::setName(QString name)
{
    Namelabel->setText(name);
}

void InfoWidget::setValue(int value)
{
    QString valueBuffer;
    valueBuffer.setNum(value);
    setValue(valueBuffer);
}

void InfoWidget::setValue(double value)
{
    QString valueBuffer;
    valueBuffer.setNum(value);
    setValue(valueBuffer);
}

void InfoWidget::setValue(QString value)
{
    Valuelabel->setText(value);
}

QString InfoWidget::getValue()
{
    return Valuelabel->text();
}
