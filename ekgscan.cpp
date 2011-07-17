#include "ekgscan.h"

EkgScan::EkgScan(QObject *parent)
    : QThread(parent)
{
    saneWidget = new KSaneWidget();
}
void EkgScan::imageReady()
{
    scanImage = QImage("scan.tiff");
    QTransform imageCorrector;
    imageCorrector = imageCorrector.rotate(90);
    scanImage = scanImage.transformed(imageCorrector,Qt::SmoothTransformation);
    scanImage.save("scan.png");
    emit ScanFinished();
}
void EkgScan::run()
{
    system(Command.toStdString().c_str());
    imageReady();
}
bool EkgScan::selDevice()
{
    Command = "";

    device = saneWidget->selectDevice();
    if (device.isEmpty())
        return false;

    Command = "scanimage -x 210 -y 297 --resolution 300 --format=tiff -d ";
    Command += device;
    Command += " >scan.tiff";

    return true;
}









