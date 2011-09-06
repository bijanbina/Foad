#include "infowidget.h"

InfoWidget::InfoWidget(LColor color , QWidget *parent) :
    QWidget(parent)
{
    //Set Widget Color
    setColor(color);
}

InfoWidget::InfoWidget(QString name , int value , LColor color , QWidget *parent):
    QWidget(parent)
{
    //convert value to string
    Value.setNum(value);
    Name = name;
    //Set Widget Color
    setColor(color);
}

InfoWidget::InfoWidget(QString name , QString value , LColor color , QWidget *parent):
    QWidget(parent)
{
    Name  = name;
    Value = value;
    //Set Widget Color
    setColor(color);
}

void InfoWidget::setName(QString name)
{
    Name = name;
    update();
}

void InfoWidget::setValue(int value)
{
    Value.setNum(value);
    setValue(Value);
}

void InfoWidget::setValue(double value)
{
    Value.setNum(value);
    setValue(Value);
}

void InfoWidget::setValue(QString value)
{
    Value = value;
    update();
}

QString InfoWidget::getValue()
{
    return Value;
}

void InfoWidget::setColor(LColor color)
{
    labelColor = color;
    QString path;
    switch(labelColor)
    {
    case Blue:
        path = ":/LBlue";
        break;
    case Green:
        path = ":/LGreen";
        break;
    case Red:
        path = ":/LRed";
        break;
    case Orenge:
        path = ":/LOrenge";
        break;
    case Gold:
        path = ":/LGold";
        break;
    case LightBlue:
        path = ":/LLightBlue";
        break;
    case LightGreen:
        path = ":/LLightGreen";
        break;
    case White:
        path = ":/LWhite";
        break;
    case Purple:
        path = ":/LPurple";
        break;
    case LightPurple:
        path = ":/LLightPurple";
        break;
    }
    LImage = QImage (path);
}

InfoWidget::LColor InfoWidget::getColor()
{
    return labelColor;
}

void InfoWidget::paintEvent(QPaintEvent * pe)
{
    //prepare
    Q_UNUSED(pe);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //Draw Background
    painter.drawImage (QRect(0,0,width(),height()),LImage);
    //Render Text
    QFont writeFont("Oblivious font",8);
    writeFont.setBold(true);
    //painter.setFont(writeFont);
    //painter.setPen(Qt::white);
    painter.drawText (QRect(0,0,width(),height()),Qt::AlignCenter,Name + Value);

    painter.setOpacity (1);
}



























