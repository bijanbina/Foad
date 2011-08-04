#include "ekgsplash.h"

int main(int argc, char *argv[])
{
    QApplication EKG(argc, argv);

    EKGsplash *splashScreen = new EKGsplash();
    splashScreen->show();

    EKGWindow *ekgwindow = new EKGWindow();

    QObject::connect(splashScreen,SIGNAL(splashShowed()),ekgwindow,SLOT(show()));

    return EKG.exec();
}


