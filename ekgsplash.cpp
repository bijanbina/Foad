#include "ekgsplash.h"

EKGsplash::EKGsplash(QWidget *parent) :
    QMainWindow(parent)
{
    initialize(SHOW_TIME);
}

EKGsplash::EKGsplash(int showTime,QWidget *parent):
    QMainWindow(parent)
{
    initialize(showTime);
}

void EKGsplash::showEvent ( QShowEvent * event )
{
    ShowTimer->start();
    fadeTimer->start();
}

void EKGsplash::initialize (int second)
{
    mainwidget  = new QLabel;
    ShowTimer   = new QTimer;
    fadeTimer   = new QTimer;
    fadeOpacity = 0;
    //Load Image
    QPixmap pm = QPixmap(":/Splash");
    QPixmap maskImage = QPixmap(":/Mask");
    //Set Window Transparent
    setMask(maskImage.mask());
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    setWindowOpacity(fadeOpacity);
    //Add Widget
    mainwidget->setPixmap(pm);
    setCentralWidget(mainwidget);
    //Modify Timer
    ShowTimer->setInterval(second * 1000);
    fadeTimer->setInterval(second * 5);
    //Create Signal and Slot
    connect(fadeTimer,SIGNAL(timeout()),this,SLOT(nextFade()));
    connect(ShowTimer,SIGNAL(timeout()),this,SLOT(timer_Stop()));
}

void EKGsplash::timer_Stop()
{
    emit splashShowed();
    ShowTimer->stop();
    fadeTimer->stop();
    close();
}

void EKGsplash::nextFade()
{
    if (fadeOpacity < 1)
    {
        fadeOpacity += 0.01;
        setWindowOpacity(fadeOpacity);
    }
    else
        fadeTimer->stop();
}











