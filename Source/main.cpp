#include "ekgsplash.h"

int main(int argc, char *argv[])
{
    QApplication Foad(argc, argv);

    QTranslator myappTranslator;
    QString app_name = "foad_";
    myappTranslator.load("foad_fa.qm");
    Foad.installTranslator(&myappTranslator);

    QApplication::setLayoutDirection(Qt::RightToLeft);
    Foad.setLayoutDirection(Qt::RightToLeft);
    EKGsplash *splashScreen = new EKGsplash();
    splashScreen->show();

    EKGWindow *ekgwindow = new EKGWindow();

    QObject::connect(splashScreen,SIGNAL(splashShowed()),ekgwindow,SLOT(show()));

    return Foad.exec();
}


