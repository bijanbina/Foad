#include "interceptw.h"

/*--------------------------------------------------------------------------------------------
|                                         PLOT                                               |
---------------------------------------------------------------------------------------------*/
void InterceptW::plot(double *Signal,double *Detect ,int size)
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
    myPlot->setAxisMaxMajor(QwtPlot::xBottom, 10);
    EKG_Grid->setMajPen(QPen(QColor(125,0,0,200), 1, Qt::SolidLine));
    EKG_Grid->setMinPen(QPen(QColor(125,0,0,60), 0.2, Qt::SolidLine));
    EKG_Grid->attach(myPlot);
    //--------------- Add Curves ----------------
    Signal_curves = new QwtPlotCurve(trUtf8("Signal"));
    Detect_curves = new QwtPlotCurve(trUtf8("Detect"));
    //--------------- Preparing -----------------
    zoomer->zoom(0);
    myPlot->setTitle(trUtf8("EKG Signal"));
    myPlot->setAxisTitle(myPlot->xBottom,trUtf8("Time (s)"));
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
    if (A_Ekg_signal)
        Signal_curves->attach(myPlot);
    if (A_Det_signal)
        Detect_curves->attach(myPlot);
    if (A_Fil_signal)
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
void InterceptW::qCurve()
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
void InterceptW::rCurve()
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
void InterceptW::sCurve()
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
InterceptW::InterceptW(QWidget *parent) :QWidget(parent)
{
    Scanner  = new QScan;
    numbers  = new QPushButton;
    //Set Defualt Value
    SigTime                          = 5;
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
    A_Ekg_signal                     = true;
    A_Det_signal                     = true;
    A_Fil_signal                     = false;
    A_Plot_show                      = true;
    B_AskComplex                     = true;
    //Plot
    InterCeptWindow = new SigDetect;
    myPlot          = new QwtPlot();
    interceptPlot   = new QwtPlot();
    EKG_Grid        = new QwtPlotGrid();
    Signal_curves   = new QwtPlotCurve(trUtf8("Signal"));
    Detect_curves   = new QwtPlotCurve(trUtf8("Detect"));
    s_curves        = new QwtPlotCurve;
    q_curves        = new QwtPlotCurve;
    r_curves        = new QwtPlotCurve;

    myPlot       ->setTitle(trUtf8("EKG Signal"));
    interceptPlot->setTitle(trUtf8("Intercept Plot"));
    myPlot       ->canvas()->setFrameStyle(0);
    interceptPlot->canvas()->setFrameStyle(0);

    interceptPlot->setAxisTitle(myPlot->xBottom, trUtf8("Time (s)"));
    interceptPlot->setAxisTitle(myPlot->yLeft, trUtf8("Voltage (mV)"));
    myPlot->setAxisTitle(myPlot->xBottom,trUtf8("Time (s)"));

    zoomer = new QwtPlotZoomer(myPlot->canvas());
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);

    //Info Box
    p_info = new infoBoxW;

    //Create Layout
    CreateLayout();
    if (!disList.isExist())
            Warning(("\nYou doesn't create a disease list\nfor do this go to EKG menu and click on \"Add Desase\""));

    //match signals and slots
    createSandS();
}

void InterceptW::B_AskComplex_change(bool val)
{
    B_AskComplex = val;
}

void InterceptW::A_Fil_change(bool val)
{
    A_Fil_signal = val;
    if (A_Fil_signal)
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

void InterceptW::A_Plot_change(bool val)
{
    A_Plot_show = val;
    myPlot->setVisible(A_Plot_show);
}

void InterceptW::A_Sig_change(bool val)
{
    A_Ekg_signal = val;
    if (A_Ekg_signal)
        Signal_curves->attach(myPlot);
    else
        Signal_curves->detach();
    myPlot->replot();
}

void InterceptW::A_Det_change(bool val)
{
    A_Det_signal = val;
    if (A_Det_signal)
        Detect_curves->attach(myPlot);
    else
        Detect_curves->detach();
    myPlot->replot();
}
void InterceptW::CreateLayout()
{
    Main_Layout = new QVBoxLayout;
    Info_Layout = new QHBoxLayout;

    //---Set Layout---
    Info_Layout->addWidget(interceptPlot,1);

    QSpacerItem *space = new QSpacerItem(10,10);
    jodaKonnande = new QFrame();
    jodaKonnande->setFrameShape(QFrame::VLine);
    jodaKonnande->setFrameShadow(QFrame::Sunken);
    Info_Layout->addItem(space);
    space = new QSpacerItem(10,10);
    Info_Layout->addWidget(jodaKonnande);
    Info_Layout->addItem(space);
    update_info();
    Info_Layout->addWidget(myPlot,2);
    //Add To main
    Main_Layout->addLayout(Info_Layout,2);

    //
    jodaKonnande = new QFrame();
    jodaKonnande->setFrameShape(QFrame::HLine);
    jodaKonnande->setFrameShadow(QFrame::Raised);
    Main_Layout->addSpacing(5);
    Main_Layout->addWidget(jodaKonnande);
    Main_Layout->addSpacing(5);
    //
    Main_Layout->addWidget(p_info,1);

    //set main layout
    setLayout(Main_Layout);
}
void InterceptW::scan()
{
    Scanner->show();
}
void InterceptW::scanFinished()
{
    imageProc.loadPic("scan.png");
    Intercept(imageProc.getSignal(),true,true);
    update_info();
}
void InterceptW::openImage()
{
    QString title = "Open Scanned Signal";
    QFileDialog::Options options;
    QString selectedFilter;
    QString files = QFileDialog::getOpenFileName(this, title,"","Picture (*.png *.jpg *.tiff)",&selectedFilter,options);
    if(!files.isEmpty())
    {
        imageProc.loadPic(files);
        Intercept(imageProc.getSignal(),true,true);
        update_info();
    }
}
void InterceptW::openRecord()
{
    QString title = trUtf8("Open Record");
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
void InterceptW::openTrain()
{
    QString title = trUtf8("Open Train File");
    QFileDialog::Options options;
    QString selectedFilter;
    QString files = QFileDialog::getOpenFileName(this, title,"","Trained File (*.dat)",&selectedFilter,options);
    if(!files.isEmpty())
    {
        QFileInfo recordFile = QFileInfo(files);
        SigRecord = recordFile.baseName();
    }
}
//! update information of the infobox
void InterceptW::update_info()
{
    p_info->setRecord(SigRecord);
    p_info->setTime(SigTime);
    p_info->setAge(EKG_age);
    p_info->setP(localFeature.P_amp);
    p_info->setPR(localFeature.PR_interval);
    p_info->setQ(localFeature.Q_amp);
    p_info->setR(localFeature.R_amp);
    p_info->setS(localFeature.S_amp);
    p_info->setT(localFeature.T_amp);
    p_info->setQT(localFeature.QT_interval);
    p_info->setQRS(localFeature.QRScomplex_interval);
    p_info->setPower(localFeature.EKGpower);
    p_info->setBeatRate(localFeature.Heart_beat_ven);
    p_info->setRR(localFeature.RR_interval);
    p_info->setDisease(localWeka.disease);
}
/*--------------------------------------------------------------------------------------------
|                                  Signal Proccessing                                         |
---------------------------------------------------------------------------------------------*/
weka_data InterceptW::Intercept(vector<double> sig , bool getPlot, bool getIntercept)
{
    //Check Signal
    if(sig.size() < 1)
    {
        Warning(trUtf8("Signal is not correct"));
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
        if (B_AskComplex || Sig_Arr.size() < 15)
            ComplexID = askComplex(Sig_Arr.size()) - 1; //-1 because it's zero base but input is 1 base!
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
        SigDetect *localDETECTION = new SigDetect(SBFD,false ,interceptPlot,i, EKG_age,Sig_Arr[i].start);
        localInfo.push_back(localDETECTION->getInfo());
        delete localDETECTION;
    }
    //If in intercept Mode:
    if (ComplexID != -1)
    {
        r_feature FW (localInfo);
        localFeature = FW.getFueture();
        InterCeptWindow = new SigDetect(Complex,true ,interceptPlot,ComplexID, EKG_age,Sig_Arr[ComplexID].start);
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
void InterceptW::Detection_Click()
{
    if (!sigReader.open(SigRecord))
        return;
    if (!sigReader.read(SigTime))
        return;
    Signal = sigReader.getSignal();
    Intercept(Signal,true,true);
    update_info();
}
/*--------------------------------------------------------------------------------------------
|                                   Menu And Dialog                                          |
---------------------------------------------------------------------------------------------*/
void InterceptW::setDB_Path()
{
    QString buffer = QFileDialog::getExistingDirectory(this,trUtf8("DataBase Set Path"),sigReader.getDB());
    if (!buffer.isEmpty())
        sigReader.setDB(buffer);
}
void InterceptW::setRecordNum()
{
//! [0]
    bool ok;
    QString buffer = QInputDialog::getText(this,trUtf8("set Record Number"),trUtf8("Record Number"), QLineEdit::Normal,SigRecord, &ok);
    if (ok)
    {
        SigRecord = buffer;
        Detection_Click();
    }
//! [0]
}
void InterceptW::setSigTime()
{
//! [0]
    bool ok;
    int i = QInputDialog::getInt(this, trUtf8("Set Signal Time"),
                                 trUtf8("Time"), SigTime, 0, 100000, 1, &ok);
    if (ok)
        SigTime = i;
}
void InterceptW::setAge()
{
//! [0]
    bool ok;
    int i = QInputDialog::getInt(this, trUtf8("Set EKG Age"),
                                 trUtf8("Age"), EKG_age, 0, 150, 1, &ok);
    if (ok)
        EKG_age = i;
}
void InterceptW::OpenError(char *recornNum)
{
    QString strm=trUtf8("Could not open signal number ");
    strm+=recornNum;
    strm+=" !";
    strm+=trUtf8("\nPlease Check .hea and .dat of signal File ");
    Warning(strm);
}
vector<QString> InterceptW::getFiles(char *patch)
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
vector<QString> InterceptW::getUnDiz(char *patch, vector<QString> values)
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
int InterceptW::askComplex(int end)
{
//! [0]
    QString buffer = QString(trUtf8("Complex Number To Intercept"));
    bool ok;
    int i = QInputDialog::getInt(this, buffer,buffer,3, 1, end, 1, &ok);
    if (ok)
        return i;
    else
        return -1;
}
void InterceptW::Warning(QString text)
{
    QString title = trUtf8("Warning");
    QMessageBox *warnWindow = new QMessageBox(QMessageBox::Warning,title,text);
    warnWindow->setIconPixmap(QPixmap(":/warning"));
    warnWindow->setWindowIcon(QIcon(":/warning"));
    warnWindow->show();
}
void InterceptW::addDisease()
{
    bool ok;
    QString title = trUtf8("Add Disease");
    QString Label = trUtf8("Disease name:");
    QString DiseaseName = QInputDialog::getText(this, title,Label, QLineEdit::Normal, QString(), &ok);
    if (ok)
        disList.addItem(DiseaseName);
}
void InterceptW::setDisease()
{
    bool ok;
    QString title = trUtf8("Set Disease");
    QString Label = trUtf8("Disease name:");
    if(disList.isExist())
    {
        vector<QString> dizList = disList.getList();
        QStringList items;
        for (int i = 0 ; i < dizList.size();i++)
            items << dizList[i];
        QString item = QInputDialog::getItem(this, title,Label, items, 0, false, &ok);
    }
    else
        Warning(trUtf8("\nYou doesn't create a disease list\nfor do this go to EKG menu and click on \"Add Desase\""));
}
//! match signal and slot
void InterceptW::createSandS()
{
    connect(p_info, SIGNAL(scan_click())  ,this,SLOT(scan()));
    connect(p_info, SIGNAL(open_click())  ,this,SLOT(openRecord()));
    connect(p_info, SIGNAL(start_click()) ,this,SLOT(Detection_Click()));
    connect(Scanner,SIGNAL(scanFinished()),this,SLOT(scanFinished()));
}











