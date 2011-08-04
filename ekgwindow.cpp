#include "ekgwindow.h"

/*--------------------------------------------------------------------------------------------
|                                         PLOT                                               |
---------------------------------------------------------------------------------------------*/
void EKGWindow::plot(double *Signal,double *Detect ,int size)
{
    myPlot->clear();
    myPlot->plotLayout()->setAlignCanvasToScales(true);
    vector<double> xDetect;
    vector<double> yDetect;
    double x[size];
    double s = 0;
    for(int i = 0 ; i < size ; i++)
    {
        x[i] = s;
        if (Detect[i] == 1)
        {
            xDetect.push_back(s);
            yDetect.push_back(Signal[i]);
        }
        s +=0.005;
    }
    double XDetect[xDetect.size()];
    double YDetect[yDetect.size()];
    for (int i = 0 ; i < yDetect.size();i++)
    {
        XDetect[i] = xDetect[i];
        YDetect[i] = yDetect[i];
    }
    //--------------EKG Grid Line-----------------
    EKG_Grid->enableXMin(true);
    EKG_Grid->enableYMin(true);
    myPlot->setAxisMaxMinor(QwtPlot::xBottom, 5);
    myPlot->setAxisMaxMajor(QwtPlot::xBottom, 20);
    EKG_Grid->setMajPen(QPen(QColor(125,0,0,200), 1, Qt::SolidLine));
    EKG_Grid->setMinPen(QPen(QColor(125,0,0,60), 0.2, Qt::SolidLine));
    EKG_Grid->attach(myPlot);
    //--------------- Add Curves ----------------
    Signal_curves = new QwtPlotCurve("Signal");
    Detect_curves = new QwtPlotCurve("Detect");
    //--------------- Preparing -----------------
    myPlot->setTitle("EKG Signal");
    myPlot->setAxisTitle(myPlot->xBottom, "Time (s)");
    myPlot->setAxisTitle(myPlot->yLeft, "Voltage (mV)");
    Signal_curves->setRenderHint(QwtPlotItem::RenderAntialiased);
    Detect_curves->setRenderHint(QwtPlotItem::RenderAntialiased);
    //-------------- Add Symbol -------------------
    QwtSymbol sym;
    sym.setStyle(QwtSymbol::Ellipse);
    sym.setPen(QColor(Qt::red));
    sym.setBrush(QColor(Qt::yellow));
    sym.setSize(7);
    Detect_curves->setSymbol(sym);
    Detect_curves->setStyle(QwtPlotCurve::NoCurve);
    // ------copy the data into the curves-----------
    Signal_curves->setData(x,Signal,size);
    Detect_curves->setData(XDetect,YDetect,yDetect.size());
    //-----------------Set pen-----------------------
    QPen *ekgPen = new QPen(Qt::blue);
    ekgPen->setWidthF(0.5);
    ekgPen->setJoinStyle(Qt::RoundJoin);
    ekgPen->setCapStyle(Qt::RoundCap);
    Signal_curves->setPen(*ekgPen);
    //------------- Attach Curves -------------------
    if (A_Ekg_signal->isChecked())
        Signal_curves->attach(myPlot);
    if (A_Det_signal->isChecked())
        Detect_curves->attach(myPlot);
    if (A_Fil_signal->isChecked())
    {
        qCurve();
        sCurve();
        rCurve();
    }
    //------------- Set Axis Scale-------------------
    myPlot->setAxisScale(QwtPlot::xBottom, 0, s);
    myPlot->setAxisScale(QwtPlot::yLeft, -600, 600);

    zoomer->setMousePattern(QwtEventPattern::MouseSelect4,Qt::RightButton);
    zoomer->setZoomBase();
    zoomer->setRubberBandPen(QPen(QColor(0, 155, 0), 2, Qt::SolidLine));
    zoomer->setTrackerPen(QPen(Qt::red));
    // finally, refresh the plot
    myPlot->replot();
}
void EKGWindow::qCurve()
{
    double x[localInfo.size()], y[localInfo.size()];
    q_curves = new QwtPlotCurve;
    for (int i = 0 ; i < localInfo.size(); i++)
    {
        int M = localInfo[i].q.detect + localInfo[i].start - TRAiNTIME;
        if (M == -1)
        {
            M = localInfo[i].qs.detect + localInfo[i].start;
        }
        //Calculate Q place
        x[i] = M / 200.0;
        y[i] = Signal[M + TRAiNTIME];
    }
    //-------------- Add Symbol -------------------
    QwtSymbol sym;
    sym.setStyle(QwtSymbol::Ellipse);
    sym.setPen(QColor(Qt::red));
    sym.setBrush(QColor(Qt::yellow));
    sym.setSize(8);
    q_curves->setSymbol(sym);
    q_curves->setStyle(QwtPlotCurve::NoCurve);
    // ------copy the data into the curves-----------
    q_curves->setData(x,y,localInfo.size());
    //--------------- Attach Curves ----------------
    q_curves->attach(myPlot);
}
void EKGWindow::rCurve()
{
    double x[localInfo.size()], y[localInfo.size()];
    if(localInfo[0].r.detect!=-1)
    {
        r_curves = new QwtPlotCurve;
        for (int i = 0 ; i < localInfo.size(); i++)
        {
            int M = localInfo[i].r.detect + localInfo[i].start - TRAiNTIME;
            //Calculate R place
            x[i] = M / 200.0;
            y[i] = Signal[M + TRAiNTIME];
        }
        //-------------- Add Symbol -------------------
        QwtSymbol sym;
        sym.setStyle(QwtSymbol::Ellipse);
        sym.setPen(QColor(Qt::red));
        sym.setBrush(QColor(255,100,20));
        sym.setSize(8);
        r_curves->setSymbol(sym);
        r_curves->setStyle(QwtPlotCurve::NoCurve);
        // ------copy the data into the curves-----------
        r_curves->setData(x,y,localInfo.size());
        //--------------- Attach Curves ----------------
        r_curves->attach(myPlot);
    }
}
void EKGWindow::sCurve()
{
    double x[localInfo.size()], y[localInfo.size()];
    s_curves = new QwtPlotCurve;
    for (int i = 0 ; i < localInfo.size(); i++)
    {
        int M = localInfo[i].s.detect + localInfo[i].start - TRAiNTIME;
        if (M == -1)
        {
            M = localInfo[i].qs.detect + localInfo[i].start;
        }
        //Calculate S place
        x[i] = M / 200.0;
        y[i] = Signal[M + TRAiNTIME];
    }
    //-------------- Add Symbol -------------------
    QwtSymbol sym;
    sym.setStyle(QwtSymbol::Ellipse);
    sym.setPen(QColor(Qt::red));
    sym.setBrush(QColor(Qt::green));
    sym.setSize(8);
    s_curves->setSymbol(sym);
    s_curves->setStyle(QwtPlotCurve::NoCurve);
    // ------copy the data into the curves-----------
    s_curves->setData(x,y,localInfo.size());
    //--------------- Attach Curves ----------------
    s_curves->attach(myPlot);
}
/*--------------------------------------------------------------------------------------------
|                                          GUI                                                |
---------------------------------------------------------------------------------------------*/
EKGWindow::EKGWindow(QWidget *parent) :QMainWindow(parent)
{
    Scanner  = new QScan;
    numbers  = new QPushButton;
    NNWidget = new NNframwork;
    //ADD Menu
    createMenu();
    //Set Defualt Value
    mode                             = 1;
    SigTime                          = 20;
    SigRecord                        = "100";
    EKG_age                          = 30;
    localFeature.P_amp               = 0;
    localFeature.Q_amp               = 0;
    localFeature.R_amp               = 0;
    localFeature.S_amp               = 0;
    localFeature.T_amp               = 0;
    localFeature.QT_interval         = 0;
    localFeature.PR_interval         = 0;
    localFeature.QRScomplex_interval = 0;
    localFeature.EKGpower            = 0;
    localFeature.Heart_beat_ven      = 0;
    localFeature.RR_interval         = 0;
    Percentage                       = 0;
    //Plot
    myPlot = new QwtPlot();
    EKG_Grid = new QwtPlotGrid();
    myPlot->setTitle("EKG Signal");
    myPlot->canvas()->setFrameStyle(0);

    Signal_curves = new QwtPlotCurve("Signal");
    Detect_curves = new QwtPlotCurve("Detect");
    s_curves = new QwtPlotCurve;
    q_curves = new QwtPlotCurve;
    r_curves = new QwtPlotCurve;

    zoomer = new QwtPlotZoomer(myPlot->canvas());
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);

    //Button
    ScanButton = new GButton(GButton::Red,"Scan");
    RecordButton = new GButton(GButton::Orenge,"Open Record");

    //Create Layout
    CreateLayout(true);
    //Make center
    resize(WIDTH,HEIGHT);
    QRect available_geom = QDesktopWidget().availableGeometry();
    QRect current_geom = frameGeometry();
    setGeometry(available_geom.width() / 2 - current_geom.width() / 2,
                available_geom.height() / 2 - current_geom.height() / 2,
                current_geom.width(),current_geom.height());
    if (!disList.isExist())
            Warning("\nYou doesn't create a disease list\nfor do this go to EKG menu and click on \"Add Desase\"");
    //connect Signal and slot
    createSandS();
}
void EKGWindow::A_Fil_change()
{
    if (A_Fil_signal->isChecked())
    {
        qCurve();
        sCurve();
        rCurve();
    }
    else
    {
        q_curves->detach();
        s_curves->detach();
        r_curves->detach();
    }
    myPlot->replot();
}
void EKGWindow::A_Plot_change()
{
    if (A_Plot_show->isChecked())
    {
        A_Ekg_signal->setEnabled(true);
        A_Det_signal->setEnabled(true);
        A_Fil_signal->setEnabled(true);
    }
    else
    {
        A_Ekg_signal->setEnabled(false);
        A_Det_signal->setEnabled(false);
        A_Fil_signal->setEnabled(false);
    }
    myPlot->setVisible(A_Plot_show->isChecked());
}
void EKGWindow::A_Sig_change()
{
    if (A_Ekg_signal->isChecked())
        Signal_curves->attach(myPlot);
    else
        Signal_curves->detach();
    myPlot->replot();
}
void EKGWindow::A_Det_change()
{
    if (A_Det_signal->isChecked())
        Detect_curves->attach(myPlot);
    else
        Detect_curves->detach();
    myPlot->replot();
}
void EKGWindow::A_Weka_Save()
{
    QString title = "Save Weka";
    QString path = QFileDialog::getSaveFileName(this,title,"","Train File(*.dat);");
    if(!path.isEmpty())
        LWW >> path;
}
void EKGWindow::CreateLayout(bool newplot)
{
    filePL = new QLabel("File Progress:");
    dbPL = new QLabel("All Progress :");
    Main_Widget = new QWidget;
    Main_Layout = new QVBoxLayout;
    Grid_Layout = new QGridLayout;
    Info_Layout = new QHBoxLayout;
    DBProgress = new QProgressBar;
    fileProgress = new QProgressBar;
    Progress_Layout = new QHBoxLayout;
    if(newplot)
    {
        myPlot = new QwtPlot();
        EKG_Grid = new QwtPlotGrid();
        interceptPlot = new QwtPlot();
        myPlot->setTitle("EKG Signal");
        myPlot->canvas()->setFrameStyle(0);
        interceptPlot->canvas()->setFrameStyle(0);
        interceptPlot->setTitle("Intercept Plot");

        zoomer = new QwtPlotZoomer(myPlot->canvas());
        zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);
    }
    //---Set Layout---
    if(A_Plot_show->isChecked())
    {
        Main_Layout->addWidget(myPlot,1);
    }
    switch(mode)
    {
    case 1:
        {
            Info_Layout->addWidget(interceptPlot,1);
            //
            jodaKonnande = new QFrame();
            jodaKonnande->setFrameShape(QFrame::HLine);
            jodaKonnande->setFrameShadow(QFrame::Raised);
            Main_Layout->addSpacing(5);
            Main_Layout->addWidget(jodaKonnande);
            Main_Layout->addSpacing(5);
            //
            QSpacerItem *space = new QSpacerItem(10,10);
            jodaKonnande = new QFrame();
            jodaKonnande->setFrameShape(QFrame::VLine);
            jodaKonnande->setFrameShadow(QFrame::Sunken);
            Info_Layout->addItem(space);
            space = new QSpacerItem(10,10);
            Info_Layout->addWidget(jodaKonnande);
            Info_Layout->addItem(space);
            Info_Box();
            Info_Layout->addWidget(BoxWidget,1);
            //Add To main
            Main_Widget->setLayout(Info_Layout);
            Main_Layout->addWidget(Main_Widget,1);
            break;
        }
    case 2:
        {
            Progress_Layout->addWidget(filePL);
            Progress_Layout->addWidget(fileProgress);
            Grid_Layout->addLayout(Progress_Layout, Grid_Layout->rowCount(),0);
            Main_Widget = new QWidget;
            Main_Widget->setLayout(Grid_Layout);
            Main_Layout->addWidget(Main_Widget);
            break;
        }
    case 3:
        {
            Progress_Layout->addWidget(filePL);
            Progress_Layout->addWidget(fileProgress);
            Grid_Layout->addLayout(Progress_Layout , Grid_Layout->rowCount(),0);
            Progress_Layout = new QHBoxLayout;
            Progress_Layout->addWidget(dbPL);
            Progress_Layout->addWidget(DBProgress);
            Grid_Layout->addLayout(Progress_Layout, Grid_Layout->rowCount(),0);
            Main_Widget = new QWidget;
            Main_Widget->setLayout(Grid_Layout);
            Main_Layout->addWidget(Main_Widget);
            break;
        }
    }
    //Window
    Main_Widget = new QWidget;
    Main_Widget->setLayout(Main_Layout);
    setCentralWidget(Main_Widget);
    setWindowIcon(QIcon(":/icon"));
    setWindowTitle("Foad");
}
void EKGWindow::createInfo(QString name , int value)
{
    QString valueBuffer;
    valueBuffer.setNum(value);
    Feuture_Layout = new QHBoxLayout;
    fLabel = new QLabel(name);
    fWidget = new QGroupBox;
    Feuture_Layout->addWidget(fLabel);
    fLabel = new QLabel(valueBuffer);
    Feuture_Layout->addWidget(fLabel);
    fWidget->setLayout(Feuture_Layout);
}
void EKGWindow::createInfo(QString name , QString value)
{
    Feuture_Layout = new QHBoxLayout;
    fLabel = new QLabel(name);
    fWidget = new QGroupBox;
    Feuture_Layout->addWidget(fLabel);
    fLabel = new QLabel(value);
    Feuture_Layout->addWidget(fLabel);
    fWidget->setLayout(Feuture_Layout);
}
void EKGWindow::scan()
{
    Scanner->show();
}
void EKGWindow::scanFinished()
{
    imageProc.loadPic("scan.png");
    interceptPlot = new QwtPlot;
    interceptPlot->canvas()->setFrameStyle(0);
    interceptPlot->setTitle("Intercept Plot");
    Intercept(imageProc.getSignal(),true,true);
    CreateLayout(0);
}
void EKGWindow::openImage()
{
    QString title = "Open Scanned Signal";
    QFileDialog::Options options;
    QString selectedFilter;
    QString files = QFileDialog::getOpenFileName(this, title,"","Picture (*.png *.jpg *.tiff)",&selectedFilter,options);
    if(!files.isEmpty())
    {
        imageProc.loadPic(files);
        interceptPlot = new QwtPlot;
        interceptPlot->canvas()->setFrameStyle(0);
        interceptPlot->setTitle("Intercept Plot");
        Intercept(imageProc.getSignal(),true,true);
        CreateLayout(0);
    }
}
void EKGWindow::openRecord()
{
    QString title = "Open Record";
    QFileDialog::Options options;
    QString selectedFilter;
    QFileInfo dbFileInfo = QFileInfo(sigReader.getDB());
    QString files = QFileDialog::getOpenFileName(this, title,dbFileInfo.absoluteFilePath(),
                                                 "Record (*.dat)",&selectedFilter,options);
    if(!files.isEmpty())
    {
        QFileInfo recordFile = QFileInfo(files);
        SigRecord = recordFile.baseName();
        Detection_Click();
    }
}
void EKGWindow::openTrain()
{
    QString title = "Open Train File";
    QFileDialog::Options options;
    QString selectedFilter;
    QString files = QFileDialog::getOpenFileName(this, title,"","Trained File (*.dat)",&selectedFilter,options);
    if(!files.isEmpty())
    {
        QFileInfo recordFile = QFileInfo(files);
        SigRecord = recordFile.baseName();
    }
}
void EKGWindow::createSandS()
{
    connect(A_TrainMode,  SIGNAL(triggered(bool)),this,SLOT(A_TrainMode_change()));
    connect(A_Quit,       SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(A_SetDB,      SIGNAL(triggered(bool)),this,SLOT(setDB_Path()));
    connect(A_Save,       SIGNAL(triggered(bool)),this,SLOT(A_Weka_Save()));
    connect(A_Start,      SIGNAL(triggered(bool)),this,SLOT(Detection_Click()));
    connect(A_Plot_show,  SIGNAL(triggered(bool)),this,SLOT(A_Plot_change()));
    connect(A_Det_signal, SIGNAL(triggered(bool)),this,SLOT(A_Det_change()));
    connect(A_Ekg_signal, SIGNAL(triggered(bool)),this,SLOT(A_Sig_change()));
    connect(A_Fil_signal, SIGNAL(triggered(bool)),this,SLOT(A_Fil_change()));
    connect(A_WekaMode,   SIGNAL(triggered(bool)),this,SLOT(A_WekaMode_change()));
    connect(A_InterMode,  SIGNAL(triggered(bool)),this,SLOT(A_InterMode_change()));
    connect(A_DiseaseMode,SIGNAL(triggered(bool)),this,SLOT(A_DiseaseMode_change()));
    connect(A_SetRecord,  SIGNAL(triggered(bool)),this,SLOT(setRecordNum()));
    connect(A_SetTime,    SIGNAL(triggered(bool)),this,SLOT(setSigTime()));
    connect(A_setAge,     SIGNAL(triggered(bool)),this,SLOT(setAge()));
    connect(A_addDiz,     SIGNAL(triggered(bool)),this,SLOT(addDisease()));
    connect(A_setDiz,     SIGNAL(triggered(bool)),this,SLOT(setDisease()));
    connect(A_Scan,       SIGNAL(triggered(bool)),this,SLOT(scan()));
    connect(A_LoadImage,  SIGNAL(triggered(bool)),this,SLOT(openImage()));
    connect(Scanner,      SIGNAL(scanFinished()),this,SLOT(scanFinished()));
}
void EKGWindow::createMenu()
{
    menu = new QMenuBar;
    //menu->setLayoutDirection(Qt::RightToLeft);
    setMenuBar(menu);
    //File Menu
    QMenu *FileMenu = menu->addMenu("File");
    A_Start = FileMenu->addAction("Start");
    A_Scan = FileMenu->addAction("Scan");
    A_LoadImage = FileMenu->addAction("Open Image");
    A_Save = FileMenu->addAction("Save Weka");
    A_Save->setEnabled(false);
    FileMenu->addSeparator();
    FileMenu->addSeparator();
    //Mode Menu in File
    QMenu *ModeMenu = FileMenu->addMenu("Mode");
    A_InterMode     = ModeMenu->addAction("Intercepting");
    A_DiseaseMode   = ModeMenu->addAction("Set Disease");
    A_WekaMode      = ModeMenu->addAction("Data Constructing");
    A_TrainMode     = ModeMenu->addAction("Train Mode");
    //set Checkable Modes
    A_DiseaseMode->setCheckable(true);
    A_InterMode->setCheckable  (true);
    A_WekaMode->setCheckable   (true);
    A_TrainMode->setCheckable(true);

    A_InterMode->setChecked(true);
    FileMenu->addSeparator();
    A_Quit = FileMenu->addAction("Quit");
    //Plot Menu
    QMenu *plotMenu = menu->addMenu("Plot");
    A_Plot_show     = plotMenu->addAction("Show Plot");
    A_Ekg_signal    = plotMenu->addAction("Ekg Signal");
    A_Det_signal    = plotMenu->addAction("Detected Signal");
    A_Fil_signal    = plotMenu->addAction("Show Feuture");

    A_Plot_show->setCheckable (true);
    A_Ekg_signal->setCheckable(true);
    A_Det_signal->setCheckable(true);
    A_Fil_signal->setCheckable(true);

    A_Plot_show->setChecked (true);
    A_Ekg_signal->setChecked(true);
    A_Det_signal->setChecked(true);
    //Signal Menu
    QMenu *SigMenu  = menu->addMenu("Signal");
    A_SetTime       = SigMenu->addAction("Signal Time");
    A_SetDB         = SigMenu->addAction("Database Path");
    A_SetRecord     = SigMenu->addAction("Record Number");
    B_AskComplex    = SigMenu->addAction("Ask Complex Number");

    B_AskComplex->setCheckable(true);
    B_AskComplex->setChecked(ASKCOMPLEX);
    //Ekg Menu
    QMenu *EKGMenu  = menu->addMenu("EKG");
    A_addDiz        = EKGMenu->addAction("Add Disease");
    A_setDiz        = EKGMenu->addAction("Set Disease");
    A_setAge        = EKGMenu->addAction("Set EKG Age");
    //Neural Network Menu
    NNMenu          = NNWidget->getMenu();
    menu->addMenu(NNMenu);
    NNMenu->setEnabled(false);
    //Help Menu
    QMenu *HelpMenu = menu->addMenu("Help");
    A_Content       = HelpMenu->addAction("Content");
    A_Support       = HelpMenu->addAction("Suppot");
    A_Update        = HelpMenu->addAction("Update");
    A_About         = HelpMenu->addAction("About");
}
void EKGWindow::Info_Box()
{
    BoxWidget = new QWidget;
    QGridLayout *box_layout = new QGridLayout;
    int i = 0;
    createInfo("Record:",SigRecord);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("Signal Time:",SigTime);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("Patient Age:",EKG_age);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("P Amplitude:",localFeature.P_amp);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("Q Amplitude:",localFeature.Q_amp);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("R Amplitude:",localFeature.R_amp);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("S Amplitude:",localFeature.S_amp);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("T Amplitude:",localFeature.T_amp);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("QT:",localFeature.QT_interval);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("PR:",localFeature.PR_interval);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("QRS Width:",localFeature.QRScomplex_interval);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("EKGpower:",localFeature.EKGpower);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("Heart Beat:",localFeature.Heart_beat_ven);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("RR:",localFeature.RR_interval);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;
    createInfo("Disease:",localWeka.disease);
    box_layout->addWidget(fWidget,i/3,i%3);
    i++;

    ScanButton = new GButton(GButton::Red,"Scan");
    RecordButton = new GButton(GButton::Orenge,"Open Record");
    startButton = new GButton(GButton::Green,"Start");

    connect(startButton,SIGNAL(click()),this,SLOT(Detection_Click()));
    connect(RecordButton,SIGNAL(click()),this,SLOT(openRecord()));
    connect(ScanButton,SIGNAL(click()),this,SLOT(scan()));

    box_layout->addWidget(ScanButton,i/3,i%3);
    i++;

    box_layout->addWidget(RecordButton,i/3,i%3);
    i++;

    box_layout->addWidget(startButton,i/3,i%3);
    i++;

    box_layout->setVerticalSpacing(5);
    BoxWidget->setLayout(box_layout);
}
/*--------------------------------------------------------------------------------------------
|                                  Signal Proccessing                                         |
---------------------------------------------------------------------------------------------*/
weka_data EKGWindow::Intercept(vector<double> sig , bool getPlot, bool getIntercept)
{
    //Check Signal
    if(sig.size() < 1)
    {
        Warning("Signal is not correct");
        return weka_data();
    }
    //----------------------Variable------------------------
    localInfo = vector<Ekg_Data> ();
    int BufferSize = sig.size();
    double signal[BufferSize],detected[BufferSize];
    vector<EkgComplex> Sig_Arr;
    //-------------------Copy Signal---------------------
    for(int i = 0 ; i < BufferSize ; i++)
        signal[i] = sig[i];
    //-------------------------Initialize-------------------
    double kgh  = BufferSize;
    //-------------------Progress Bar-----------------------
    Percentage += (kgh / sigReader.nsamp()) * 190.0;
    double percentageBuffer = (kgh / sigReader.nsamp()) * 1.9;
    filePercentage += FileMarhale * percentageBuffer;
    //------------------R Detection Start-------------------
    QRSDet localQRSD = QRSDet(sig);
    vector<double> Detected = localQRSD.getDetected();
    for(int i = 0;i<Detected.size();i++)
        detected[i] = Detected[i];
    //---------------------Create Complex------------------
    Sig_Arr = localQRSD.getComplex();
    if (Sig_Arr.size() < 1)
        return weka_data();
    //---------------------Intercept EKG--------------------
    int ComplexID = -1;
    vector<double> Complex;
    if (getIntercept)
    {
        if (B_AskComplex->isChecked() || Sig_Arr.size() < 15)
            ComplexID = askComplex(Sig_Arr.size());
        else
            ComplexID = 15;
    }
    //------------------- Get Feuture -----------------------
    for (int i = 1 ; i < Sig_Arr.size();i++)
    {
        vector<double> SBFD = vector<double> (Sig_Arr[i].end - Sig_Arr[i].start);//Signal Buffer for Detection
        for (int j = Sig_Arr[i].start ; j < Sig_Arr[i].end ; j++)
        {
            SBFD[j - Sig_Arr[i].start] = signal[j];
        }
        if (i == ComplexID)
            Complex = SBFD;
        SigDetect localDETECTION = SigDetect(SBFD,false ,interceptPlot,i, EKG_age,Sig_Arr[i].start);
        localInfo.push_back(localDETECTION.getInfo());
    }
    //If in intercept Mode:
    if (ComplexID != -1)
    {
        r_feature FW (localInfo);
        localFeature = FW.getFueture();
        InterCeptWindow = SigDetect(Complex,true ,interceptPlot,ComplexID, EKG_age,Sig_Arr[ComplexID].start);
    }
    r_feature LFE(localInfo);
    //----------------------Make plot--------------------
    if (getPlot)
    {
        plot(signal,detected,BufferSize);
    }
    diseaWriter DizReader(sigReader.getDB(),sigReader.getRecord());
    weka_data Return = LFE.getWeka();
    if (DizReader.isExist())
        Return.disease = DizReader.getDisease();
    else
        Return.disease = "normal";
    localWeka = Return;
    return Return;
}
void EKGWindow::Detection_Click()
{
    if (mode == 1)
    {
        if (!sigReader.open(SigRecord))
            return;
        if (!sigReader.read(SigTime))
            return;
        interceptPlot = new QwtPlot;
        interceptPlot->canvas()->setFrameStyle(0);
        interceptPlot->setTitle("Intercept Plot");
        Signal = sigReader.getSignal();
        Intercept(Signal,true,true);
        CreateLayout(0);
    }
    if (mode == 2)
    {
        Percentage = 0;
        vector<QString> FileList;
        FileList = getFiles(sigReader.getDB());
        FileList = getUnDiz(sigReader.getDB(),FileList);
        double marhale = 100.0 / FileList.size();
        for(int i = 0 ; i < FileList.size();i++)
        {
            fileProgress->setValue(Percentage);
            QString DizBuffer = askDiz(FileList[i].toStdString().c_str());
            if(!DizBuffer.isEmpty())
            {
                diseaWriter buffer(sigReader.getDB(),FileList[i].toStdString().c_str()) ;
                buffer.setDisease(DizBuffer);
            }
            Percentage += marhale;
        }
        fileProgress->setValue(100);
    }
    if(mode==3)
    {
        QStringList selectedRecord = AddRecordFile(sigReader.getDB());
        FileMarhale =  100.0 / double(selectedRecord.count());
        filePercentage = 0;
        LWW.setDiseaseList(disList.getList());
        for (int k = 0 ; k < selectedRecord.count();k++)
        {
            filePercentage = double(k) * FileMarhale;
            DBProgress->setValue(filePercentage);
            Percentage = 0;
            SigRecord = selectedRecord[k];
            WekaDo();
        }
        //----------------Set File Name--------------------
        A_Save->setEnabled(true);
        DBProgress->setValue(100);
    }
}
void EKGWindow::WekaDo()
{
    weka_data wekaBuffer;
    if (!sigReader.open(SigRecord))
        return;
    if(A_Plot_show->isChecked())
    {
        fileProgress->setValue(Percentage);
        DBProgress->setValue(filePercentage);
        if (sigReader.read(SigTime))
        {
            Signal = sigReader.getSignal();
            wekaBuffer = Intercept(Signal,true);
            LWW << wekaBuffer;
        }
    }
    while(!wekaBuffer.disease.isEmpty())
    {
        fileProgress->setValue(Percentage);
        DBProgress->setValue(filePercentage);
        if (!sigReader.read(SigTime))
            break;
        Signal = sigReader.getSignal();
        wekaBuffer = Intercept(Signal);
        LWW << wekaBuffer;
    }
    update();
    fileProgress->setValue(100);
}
/*--------------------------------------------------------------------------------------------
|                                   Menu And Dialog                                          |
---------------------------------------------------------------------------------------------*/
void EKGWindow::setDB_Path()
{
    QString buffer = QFileDialog::getExistingDirectory(this,"DataBase Set Path",sigReader.getDB());
    if (!buffer.isEmpty())
        sigReader.setDB(buffer);
}
void EKGWindow::setRecordNum()
{
//! [0]
    bool ok;
    QString buffer = QInputDialog::getText(this,"set Record Number","Record Number", QLineEdit::Normal,SigRecord, &ok);
    if (ok)
    {
        SigRecord = buffer;
        if (mode == 1)
            Detection_Click();
    }
//! [0]
}
void EKGWindow::setSigTime()
{
//! [0]
    bool ok;
    int i = QInputDialog::getInt(this, "Set Signal Time",
                                 "Time", SigTime, 0, 100000, 1, &ok);
    if (ok)
        SigTime = i;
}
void EKGWindow::setAge()
{
//! [0]
    bool ok;
    int i = QInputDialog::getInt(this, "Set EKG Age",
                                 "Age", EKG_age, 0, 150, 1, &ok);
    if (ok)
        EKG_age = i;
}
void EKGWindow::OpenError(char *recornNum)
{
    QString strm="Could not open signal number ";
    strm+=recornNum;
    strm+=" !";
    strm+="\nPlease Check .hea and .dat of signal File ";
    Warning(strm);
}
vector<QString> EKGWindow::getFiles(char *patch)
{
    vector<QString> bufferList = vector<QString>(0);
    DIR *dp;
    struct dirent *dirp;
    dp  = opendir(patch);
    if (dp != NULL)
    {
        while (dirp = readdir (dp))
            bufferList.push_back(QString(dirp->d_name));
        closedir (dp);
    }
    else
      perror ("Couldn't open the directory");
    //Filter Data
    for (int i = 0 ; i < bufferList.size();i++)
    {
        if (bufferList[i].size()<=3)
        {
            bufferList.erase(bufferList.begin()+i);
            i--;
        }
        else
        {
            QString path = patch;
            path += "/";
            path += bufferList[i];
            QFileInfo dizFile(path);
            bufferList[i] = dizFile.baseName();
        }
    }
    sort(bufferList.begin(),bufferList.end());
    //Remove Same Name
    for (int i = 0 ; i < bufferList.size();i++)
    {
         if (bufferList[i] == bufferList[i+1])
         {
                bufferList.erase(bufferList.begin()+i);
                i--;
        }
    }
    return bufferList;
}
vector<QString> EKGWindow::getUnDiz(char *patch, vector<QString> values)
{
    for(int i =0 ; i < values.size();i++)
    {
        diseaWriter bufferd(patch,values[i].toStdString().c_str());
        if (bufferd.isExist())
        {
            values.erase(values.begin()+i);
            i--;
        }
    }
    return values;
}
QString EKGWindow::askDiz(const char* recornNum)
{

//! [0]
    QString title =QString("Set Disease ID of record ");
    QString Label = "Disease name:";
    QString DiseaseName;
    title += recornNum;
    bool ok;
    if(disList.isExist())
    {
        vector<QString> dizList = disList.getList();
        QStringList items;
        for (int i = 0 ; i < dizList.size();i++)
            items << dizList[i];
        DiseaseName = QInputDialog::getItem(this, title,Label, items, 0, false, &ok);
        if (!ok)
            DiseaseName = "";
    }
    else
        Warning("\nYou doesn't create a disease list\nfor do this go to EKG menu and click on \"Add Desase\"");
    return DiseaseName;
}
int EKGWindow::askComplex(int end)
{
//! [0]
    QString buffer =QString("Complex Number To Intercept");
    bool ok;
    int i = QInputDialog::getInt(0, buffer,buffer,3, 0, end, 1, &ok);
    if (ok)
        return i;
    else
        return -1;
}
void EKGWindow::Warning(QString text)
{
    QString title = "Warning";
    QMessageBox *warnWindow = new QMessageBox(QMessageBox::Warning,title,text);
    warnWindow->setIconPixmap(QPixmap(":/warning"));
    warnWindow->setWindowIcon(QIcon(":/warning"));
    warnWindow->show();
}
void EKGWindow::addDisease()
{
    bool ok;
    QString title = "Add Disease";
    QString Label = "Disease name:";
    QString DiseaseName = QInputDialog::getText(this, title,Label, QLineEdit::Normal, QString(), &ok);
    if (ok)
        disList.addItem(DiseaseName);
}
void EKGWindow::setDisease()
{
    bool ok;
    QString title = "Set Disease";
    QString Label = "Disease name:";
    if(disList.isExist())
    {
        vector<QString> dizList = disList.getList();
        QStringList items;
        for (int i = 0 ; i < dizList.size();i++)
            items << dizList[i];
        QString item = QInputDialog::getItem(this, title,Label, items, 0, false, &ok);
    }
    else
        Warning("\nYou doesn't create a disease list\nfor do this go to EKG menu and click on \"Add Desase\"");
}
QStringList EKGWindow::AddRecordFile(QString patch)
{
    QDir dbFinder(patch);
    QString title = "Add EKG File";
    QFileDialog::Options options;
    QString selectedFilter;
    QStringList files = QFileDialog::getOpenFileNames(this, title,dbFinder.absolutePath(),
                                "EKG Files (*.diz)",&selectedFilter,options);
    for (int i = 0 ; i < files.count();i++)
    {
        QFileInfo filterFile(files[i]);
        files[i] = filterFile.baseName();
    }
    return files;
}
/*--------------------------------------------------------------------------------------------
|                                     Mode And GUI                                           |
---------------------------------------------------------------------------------------------*/
void EKGWindow::A_TrainMode_change()
{
    NNMenu->setEnabled(true);
    setCentralWidget(NNWidget);
    A_TrainMode->setEnabled(false);
    ///
    mode = 4;
    if (A_TrainMode->isChecked())
    {
        resize(WIDTH,HEIGHT);
        A_DiseaseMode->setChecked(false);
        A_WekaMode->setChecked   (false);
        A_InterMode->setChecked  (false);
    }
    else
    {
        A_InterMode->setChecked(true);
    }
}
void EKGWindow::A_InterMode_change()
{
    mode = 1;
    if (A_InterMode->isChecked())
    {
        //Change Size
        resize(WIDTH,HEIGHT);
        NNMenu->setEnabled(false);
        A_DiseaseMode->setChecked(false);
        A_WekaMode->setChecked   (false);
        A_TrainMode->setChecked  (false);
    }
    else
    {
        A_WekaMode->setChecked(true);
    }
    CreateLayout(true);
    //resize(WIDTH,HEIGHT);
}
void EKGWindow::A_DiseaseMode_change()
{
    mode = 2;
    resize(600,400);
    CreateLayout(true);
    if (A_DiseaseMode->isChecked())
    {
        //Change Size
        resize(600,400);
        NNMenu->setEnabled(false);
        A_InterMode->setChecked(false);
        A_WekaMode->setChecked(false);
        A_TrainMode->setChecked  (false);
    }
    else
    {
        A_InterMode->setChecked(true);
    }
}
void EKGWindow::A_WekaMode_change()
{
    mode = 3;
    resize(600,450);
    CreateLayout(true);
    if (A_WekaMode->isChecked())
    {
        //Change Size
        resize(600,450);
        NNMenu->setEnabled(false);
        A_InterMode->setChecked(false);
        A_DiseaseMode->setChecked(false);
        A_TrainMode->setChecked  (false);
    }
    else
    {
        A_InterMode->setChecked(true);
    }
}












