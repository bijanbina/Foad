#ifndef EKGSPLASH_H
#define EKGSPLASH_H

#include "ekgwindow.h"
#define SHOW_TIME 4

class EKGsplash : public QMainWindow
{
    Q_OBJECT
public:
    explicit   EKGsplash(QWidget *parent = 0);
    explicit   EKGsplash(int showTime,QWidget *parent = 0);
signals:
    void       splashShowed();
private slots:
    void       timer_Stop();
    void       nextFade();
private:
    void       showEvent ( QShowEvent  * event );
    void       initialize (int second);
    double     fadeOpacity;
    QTimer    *ShowTimer;
    QTimer    *fadeTimer;
    QLabel    *mainwidget;
};

#endif // EKGSPLASH_H
