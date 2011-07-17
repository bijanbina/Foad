#ifndef EKGSCAN_H
#define EKGSCAN_H
#include "ekgimagep.h"


class EkgScan : public QThread
{
    Q_OBJECT
public:
                      EkgScan(QObject *parent = 0);
    void              run();
    bool              selDevice();
signals:
    void ScanFinished();
public slots:
    void imageReady();


private:
    QImage            scanImage;
    QString           device;
    QString           Command;
    KSaneWidget      *saneWidget;
    EKGframeWork      GlobeEKG;
};

#endif // EKGSCAN_H
