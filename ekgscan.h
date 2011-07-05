#ifndef EKGSCAN_H
#define EKGSCAN_H
#include "GButton.h"

#define endSignalWidth 100

struct scanInfo
{
    int count;
    int red;
    int blue;
    int green;
};

class EkgScan
{
public:
    EkgScan();
    void scan();
    vector<double> proccessor();
    vector<double> getSignal();
    int findNazdik(int coloumn , int value , QImage image );
    void loadPic(QString path);
    bool endSignal(QImage image);
private:
    bool   isblack(QColor color);
    int    isColor(QColor color);
    void   destroyGrid(QColor color);
    int    getRow(int row);
    vector<QImage> VSegmentation(QImage input);
    vector<QImage> HSegmentation(QImage input);
    QColor getColumn(int Column);
    QImage scanImage;
    vector<double> bSignal;
};

#endif // EKGSCAN_H
