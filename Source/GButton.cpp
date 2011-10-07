#include "GButton.h"

void GButton::initialize(const GColor color)
{
    //create timer for fading effect. go to changeAlpha function for more information
    timer = new QTimer(this);
    connect (timer,SIGNAL(timeout()),this,SLOT(changeAlpha()));
    setColor(color);
    nImage = bImage[0];
    setStatus(normal);
}

GButton::GButton(QWidget *parent): QWidget(parent)
{
    initialize();
}

GButton::GButton(const QString &text, QWidget *parent) : QWidget(parent),bText(text)
{
    initialize();
}

GButton::GButton(const GColor color ,const QString &text, QWidget *parent): QWidget(parent),bText(text)
{
    initialize(color);
}

GButton::~GButton()
{
    delete timer;
}

void GButton::paintEvent(QPaintEvent * pe)
{
	Q_UNUSED(pe);
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing);

	fade(nImage,pImage,painter);
	painter.setOpacity (1);
        painter.setFont(QFont("Oblivious font",10));
        painter.setPen(QColor(0,0,0,200));
        painter.drawText (QRect(0,0,width(),height()),Qt::AlignCenter,bText);
}

void GButton::enterEvent(QEvent * e)
{
    setStatus(enter);
    timer->start(10);
}

void GButton::leaveEvent(QEvent * e)
{
	setStatus(normal);
	timer->start(10);
}

void GButton::mousePressEvent(QMouseEvent * e)
{
	setStatus(push);
	timer->start(10);
}

void GButton::mouseReleaseEvent(QMouseEvent * e)
{
    if (bcolor != GButton::Disable)
        emit click ();
    setStatus(enter);
    timer->start(10);
}

GButton::GColor GButton::getColor ()
{
    return bcolor;
}

void GButton::setColor(GColor color)
{
     bcolor = color;
    switch (bcolor)
    {
    case GButton::Blue:
        bImage[0] = QImage(":/B1");
        bImage[1] = QImage(":/B2");
        bImage[2] = QImage(":/B3");
        break;
    case GButton::Green:
        bImage[0] = QImage(":/G1");
        bImage[1] = QImage(":/G2");
        bImage[2] = QImage(":/G3");
        break;
    case GButton::Red:
        bImage[0] = QImage(":/R1");
        bImage[1] = QImage(":/R2");
        bImage[2] = QImage(":/R3");
        break;
    case GButton::Orenge:
        bImage[0] = QImage(":/O1");
        bImage[1] = QImage(":/O2");
        bImage[2] = QImage(":/O3");
        break;
    case GButton::Disable:
        bImage[0] = QImage(":/D1");
        bImage[1] = QImage(":/D1");
        bImage[2] = QImage(":/D1");
        break;
    }
    setStatus(normal);
    repaint();
}

void GButton::setStatus(Status status)
{
    bStatus = status;
    pImage = nImage;
    switch (bStatus)
    {
    case normal:
        nImage = bImage[0];
        break;
    case enter:
        nImage = bImage[1];
        break;
    case push:
        nImage = bImage[2];
        break;
    }
}

GButton::Status GButton::getStatus() const
{
    return bStatus;
}

void GButton::fade(QImage &NImage,QImage &PImage,QPainter &Painter)
{
    Painter.setOpacity (1);
    // Draw shadow of button
    Painter.drawImage (QRect(0,0,width(),height()),QImage(":/Shadow"));
    // Draw background (last image) of button
    Painter.drawImage (QRect(0,0,width(),height()),PImage);
    // change opacity to create fading effect
    Painter.setOpacity (currentAlpha);
    Painter.drawImage (QRect(0,0,width(),height()),NImage);
}

void GButton::changeAlpha()
{
    if (currentAlpha > 1)
    {
        timer->stop ();
        currentAlpha = 0;
    }
    else
    {
        currentAlpha += 0.02;
        repaint ();
    }
}

