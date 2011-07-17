#include "ekgwindow.h"

int main(int argc, char *argv[])
{
    QApplication EKG(argc, argv);
    EKGWindow w;
    w.show();

    return EKG.exec();
}


