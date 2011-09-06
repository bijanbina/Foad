#ifndef QSCAN_H
#define QSCAN_H

#include "ekgscan.h"

class QScan : public QMainWindow
{
    Q_OBJECT
public:
    explicit QScan(QWidget *parent = 0);

signals:
    void scanFinished();

private slots:
    void         imageUpdate();
    void         scanPushed();
    void         stopUpdate();

private:
    void                 MakeCenter();
    void                 Warning(QString text);
    EkgScan              *Scanner;
    QWidget              *MWidget;
    QTimer               *updator;
    QVBoxLayout          *Mlayout;
    QHBoxLayout          *Hlayout;
    QPushButton          *ScanBtn;
    QProgressBar         *Progress;
    QGraphicsScene       *preViewer;
    QGraphicsView        *Viewer;
    QGraphicsPixmapItem  *image;
};

#endif // QSCAN_H
