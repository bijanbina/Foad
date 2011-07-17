#include "qscan.h"

QScan::QScan(QWidget *parent) :
    QMainWindow(parent)
{
    //--------- Initialiize ----------
    updator   = new QTimer;
    ScanBtn   = new QPushButton("scan");
    Scanner   = new EkgScan;
    MWidget   = new QWidget;
    Mlayout   = new QVBoxLayout;
    Hlayout   = new QHBoxLayout;
    Progress  = new QProgressBar;
    preViewer = new QGraphicsScene;
    Viewer    = new QGraphicsView(preViewer);
    //-------- set Value ------------
    updator->setInterval(1000);

    QColor background(0x70,0x70,0x70);
    QBrush backBrush(background);
    Viewer->setBackgroundBrush(backBrush);

    QPixmap scanImage(210,300);
    scanImage.fill();
    image = preViewer->addPixmap(scanImage);
    //---------- Layout -------------
    Hlayout->addWidget(Progress);
    Hlayout->addWidget(ScanBtn);

    Mlayout->addWidget(Viewer);
    Mlayout->addLayout(Hlayout);
    MWidget->setLayout(Mlayout);
    setCentralWidget(MWidget);
    //----------- Window -------------
    resize(300,345);
    setWindowIcon(QIcon(":/icon"));
    MakeCenter();
    //------- Signal & Slot ----------
    connect(updator,SIGNAL(timeout()),this,SLOT(imageUpdate()));
    connect(ScanBtn,SIGNAL(clicked()),this,SLOT(scanPushed()));
}

void QScan::imageUpdate()
{
    //Load Image
    QPixmap buffer;
    //buffer.fromImage(recovered);
    //Resize
    //double nesbat = 100.0 / buffer.width();
    //buffer = buffer.scaled(100,nesbat * buffer.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    //Show up
    //image->setPixmap(buffer);
}

void QScan::scanPushed()
{
    if(Scanner->selDevice())
        Scanner->start();
    else
    {
        Warning("Scanner not detected!");
        return;
    }
    updator->start();
    connect(Scanner,SIGNAL(finished()),this,SLOT(stopUpdate()));
}

void QScan::stopUpdate()
{
    updator->stop();
    emit scanFinished();
    close();
}

void QScan::Warning(QString text)
{
    QString title = "Warning";
    QMessageBox *warnWindow = new QMessageBox(QMessageBox::Warning,title,text);
    warnWindow->setIconPixmap(QPixmap(":/warning"));
    warnWindow->setWindowIcon(QIcon(":/warning"));
    warnWindow->show();
}

void QScan::MakeCenter()
{
    QRect available_geom = QDesktopWidget().availableGeometry();
    QRect current_geom = frameGeometry();
    setGeometry(available_geom.width() / 2 - current_geom.width() / 2,
                available_geom.height() / 2 - current_geom.height() / 2,
                current_geom.width(),current_geom.height());
}





















