#include "kiwidget.h"

KIWidget::KIWidget(QWidget *parent) :
    QGroupBox(parent)
{
    Standardinit("","");
}
KIWidget::KIWidget(QString name,QWidget *parent):
    QGroupBox(parent)
{
    Standardinit(name,"");
}

KIWidget::KIWidget(QString name,QString value,QWidget *parent):
    QGroupBox(parent)
{
    Standardinit(name,value);
}

KIWidget::KIWidget(QString name,double value,QWidget *parent):
    QGroupBox(parent)
{
    QString buffer;
    buffer.setNum(value);
    Standardinit(name,buffer);
}

void KIWidget::Standardinit(QString name,QString value)
{
    Layout = new QHBoxLayout;
    Layout->setAlignment(Qt::AlignRight);
    Layout->setDirection(QBoxLayout::RightToLeft);
    NameLabel = new QLabel(name);
    ValueLabel = new QLabel(value);
    //add widget to layout
    Layout->addWidget(NameLabel);
    Layout->addWidget(ValueLabel);
    setLayout(Layout);
}

void KIWidget::setName(QString name)
{
    NameLabel->setText(name);
}

void KIWidget::setValue(double value)
{
    QString buffer;
    buffer.setNum(value);
    ValueLabel->setText(buffer);
}

void KIWidget::setValue(int value)
{
    QString buffer;
    buffer.setNum(value);
    ValueLabel->setText(buffer);
}

void KIWidget::setValue(QString value)
{
    ValueLabel->setText(value);
}

QString KIWidget::getValue()
{
    return ValueLabel->text();
}

QString KIWidget::getName()
{
    return NameLabel->text();
}



























