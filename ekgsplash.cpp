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
}

void EKGsplash::initialize (int second)
{
    mainwidget = new QLabel;
    ShowTimer  = new QTimer;
    //Load Image
    QPixmap pm = QPixmap(":/Splash");
    QPixmap maskImage = QPixmap(":/Mask");
    //Set Window Transparent
    setMask(maskImage.mask());
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    //Add Widget
    mainwidget->setPixmap(pm);
    setCentralWidget(mainwidget);
    //Modify Timer
    ShowTimer->setInterval(second * 1000);
    //Create Signal and Slot
    connect(ShowTimer,SIGNAL(timeout()),this,SLOT(timer_Stop()));
}

void EKGsplash::timer_Stop()
{
    emit splashShowed();
    close();
}













