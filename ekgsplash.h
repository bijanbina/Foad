#ifndef EKGSPLASH_H
#define EKGSPLASH_H

#include "ekgwindow.h"
#define SHOW_TIME 5

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
private:
    void       showEvent ( QShowEvent  * event );
    void       initialize (int second);
    QTimer    *ShowTimer;
    QLabel    *mainwidget;
};

#endif // EKGSPLASH_H
