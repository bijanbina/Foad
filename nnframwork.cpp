#include "nnframwork.h"

NNframwork::NNframwork(QWidget *parent) :
    QWidget(parent)
{
    myPlot      = new QwtPlot;
    info_layout = new QGridLayout;
    Mlayout     = new QVBoxLayout;
    LocalNN     = new EKGNN;
    NNMenu      = new QMenu("Neural Network");
    //Set Default Value
    EpochCount = 2000;
    EpochNum   = 0;
    //Plot
    PlotConfig("Neural Network Train Plot",EpochCount);
    //Layout
    createInfo();
    Mlayout->addWidget(myPlot);
    Mlayout->addLayout(info_layout);
    Mlayout->setStretch(0,3);
    Mlayout->setStretch(1,1);
    setLayout(Mlayout);
    //Menu
    createMenu();
    //Signal And Slot
    createConnection();
}

void NNframwork::createInfo()
{
    TrainBtn           = new GButton(GButton::Orenge,"Start Train");
    OpenBtn            = new GButton(GButton::Green,"Open Neural Network");
    SaveBtn            = new GButton(GButton::Blue,"Save Neural Network");
    i_lastE            = new InfoWidget("Last Error : ","",InfoWidget::LightGreen);
    i_inputNNum        = new InfoWidget("Input Neurons Number : ","",InfoWidget::White);
    i_outputNNum       = new InfoWidget("Output Neurons Number : ","",InfoWidget::White);
    i_hiddentNN        = new InfoWidget("Hidden Neurons Number : ","",InfoWidget::White);
    i_EpochNum         = new InfoWidget("Epoch Number : ","",InfoWidget::Orenge);
    i_HiddenAF         = new InfoWidget("Hidden Layer\nActivation Function : ","",InfoWidget::LightBlue);
    i_OutputAF         = new InfoWidget("Output Layer\nActivation Function : ","",InfoWidget::LightBlue);
    i_learningRate     = new InfoWidget("learning Rate : ","",InfoWidget::LightGreen);
    i_learningMomentum = new InfoWidget("learning Momentum : ","",InfoWidget::Orenge);

    //Add Widget To layout
    info_layout->addWidget(i_inputNNum,0,0);
    info_layout->addWidget(i_learningMomentum,0,1);
    info_layout->addWidget(i_lastE,0,2);
    info_layout->addWidget(i_HiddenAF,0,3);
    info_layout->addWidget(i_outputNNum,1,0);
    info_layout->addWidget(i_EpochNum,1,1);
    info_layout->addWidget(i_learningRate,1,2);
    info_layout->addWidget(i_OutputAF,1,3);
    info_layout->addWidget(i_hiddentNN,2,0);
    info_layout->addWidget(TrainBtn,2,1);
    info_layout->addWidget(OpenBtn,2,2);
    info_layout->addWidget(SaveBtn,2,3);
    info_layout->setRowStretch(0,1);
    info_layout->setRowStretch(1,1);
    info_layout->setRowStretch(2,1);
}

void NNframwork::createConnection()
{
    connect(LocalNN,           SIGNAL(report(double)),this,SLOT(updateEpoch(double)));
    connect(OpenBtn,           SIGNAL(click()),this,SLOT(openClicked()));
    connect(SaveBtn,           SIGNAL(click()),this,SLOT(SaveClicked()));
    connect(TrainBtn,          SIGNAL(click()),this,SLOT(TrainClicked()));
    connect(A_EpochNum,        SIGNAL(triggered()),this,SLOT(askEpochNum()));
    connect(A_learningRate,    SIGNAL(triggered()),this,SLOT(asklearningRate()));
    connect(A_learningMomentum,SIGNAL(triggered()),this,SLOT(asklearningMomentum()));
    connect(A_inputNNum,       SIGNAL(triggered()),this,SLOT(askinputNNum()));
    connect(A_outputNNum,      SIGNAL(triggered()),this,SLOT(askoutputNNum()));
    connect(A_hiddentNN,       SIGNAL(triggered()),this,SLOT(askhiddentNN()));
    connect(A_HiddenAF,        SIGNAL(triggered()),this,SLOT(askHiddenAF()));
    connect(A_OutputAF,        SIGNAL(triggered()),this,SLOT(askOutputAF()));
}

void NNframwork::updateEpoch(double value)
{
    EpochNum += UPDATE_PER_EPCOCH;
    i_lastE->setValue(value);
    i_EpochNum->setValue(EpochNum);
    EpochError.push_back(value);
    plot(EpochError);
}

void NNframwork::openClicked()
{
    QString title = "Open Train File";
    QFileDialog::Options options;
    QString selectedFilter;
    QString path = QFileDialog::getOpenFileName(this, title,"","Neural Network File(*.net)",&selectedFilter,options);
    if(!path.isEmpty())
    {
        LocalNN->open(path);
        NNParametr = LocalNN->getSetting();
        UpdateData();
    }
}

void NNframwork::SaveClicked()
{
    if(!*LocalNN)
    {
        Warning("Please First open or create a neural network");
        return;
    }
}

void NNframwork::TrainClicked()
{
    if(!*LocalNN)
    {
        Warning("Please First open or create a neural network");
        return;
    }

    QString title = "Open Train File";
    QFileDialog::Options options;
    QString selectedFilter;
    QString path = QFileDialog::getOpenFileName(0, title,"","Trained File (*.dat)",&selectedFilter,options);
    if(!path.isEmpty())
    {
        LocalNN->Train(path,EpochCount);
        LocalNN->start();
    }
}

QMenu *NNframwork::getMenu()
{
    return NNMenu;
}

void NNframwork::Warning(QString text)
{
    QString title = "Warning";
    QMessageBox *warnWindow = new QMessageBox(QMessageBox::Warning,title,text);
    warnWindow->setIconPixmap(QPixmap(":/warning"));
    warnWindow->setWindowIcon(QIcon(":/warning"));
    warnWindow->show();
}

void NNframwork::plot(vector<double> data)
{
    int length = data.size();
    double X[length];
    double Y[length];
    for (int i = 0 ; i < length;i++)
    {
        X[i] = i * UPDATE_PER_EPCOCH;
        Y[i] = 1 - data[i];
    }
    //--------update curves data------------
    Train_curves->setData(X,Y,length);
    // finally, refresh the plot
    myPlot->replot();
}

void NNframwork::PlotConfig(QString plotName,int xSize)
{
    myPlot->setTitle(plotName);
    myPlot->setAxisTitle(myPlot->xBottom, "Epoch Number");
    myPlot->setAxisTitle(myPlot->yLeft, "Correctly Classified Instances");
    myPlot->canvas()->setFrameStyle(0);
    myPlot->setCanvasBackground(QColor(255,255,255));
    //------------- Set Axis Scale-------------------
    myPlot->setAxisScale(QwtPlot::xBottom, 0, xSize);
    myPlot->setAxisScale(QwtPlot::yLeft, 0, 1);
    //--------------- Create Curves ----------------
    Train_curves = new QwtPlotCurve("Train");
    Train_curves->setRenderHint(QwtPlotItem::RenderAntialiased);
    //-------------- Add Symbol -------------------
    sym.setStyle(QwtSymbol::Ellipse);
    sym.setPen(QColor(Qt::red));
    sym.setBrush(QColor(Qt::yellow));
    sym.setSize(7);
    //Train_curves->setSymbol(sym);
    //-----------------Set pen-----------------------
    QPen ekgPen(Qt::blue);
    ekgPen.setWidthF(0.5);
    ekgPen.setJoinStyle(Qt::RoundJoin);
    ekgPen.setCapStyle(Qt::RoundCap);
    Train_curves->setPen(ekgPen);
    //------------- Attach Curves -------------------
    Train_curves->attach(myPlot);
}

void NNframwork::createMenu()
{
    A_EpochNum         = NNMenu->addAction("Set Epoch Count");
    A_learningRate     = NNMenu->addAction("Set Learning Rate");
    A_learningMomentum = NNMenu->addAction("Set Learning Momentom");
    A_inputNNum        = NNMenu->addAction("Set Input Neurons Number");
    A_outputNNum       = NNMenu->addAction("Set Output Neurons Number");
    A_hiddentNN        = NNMenu->addAction("Set Hidden Neurons Number");
    A_HiddenAF         = NNMenu->addAction("Set Hidden Layer Activation Function");
    A_OutputAF         = NNMenu->addAction("Set Output Layer Activation Function");
}

void NNframwork::askEpochNum()
{
    bool ok;
    int i = QInputDialog::getInt(this, "Set Epoch Count","Epoch", EpochCount, 0, 100000, 1, &ok);
    if (ok)
        EpochCount = i;
}

void NNframwork::asklearningRate()
{
    bool ok;
    double i = QInputDialog::getDouble(this, "Set Learning Rate","Learning Rate", EpochCount, 0, 1, 1, &ok);
    if (ok)
    {
        NNParametr.LearningRate = i;
        LocalNN->setLearningRate(NNParametr.LearningRate);
        UpdateData();
    }
}

void NNframwork::asklearningMomentum()
{
    bool ok;
    double i = QInputDialog::getDouble(this, "Set Learning Momentom","Learning Momentom", EpochCount, 0, 1, 1, &ok);
    if (ok)
    {
        NNParametr.LearningMomentum = i;
        LocalNN->setLearningMomentum(NNParametr.LearningMomentum);
        UpdateData();
    }
}

void NNframwork::askinputNNum()
{
    bool ok;
    int i = QInputDialog::getInt(this, "Set Input Neurons Number","Neurons Number", EpochCount, 0, 100000, 1, &ok);
    if (ok)
    {
        NNParametr.InputNeuronsNum = i;
        Warning("This value make effective if you recreate your Neural network");
    }
}

void NNframwork::askoutputNNum()
{
    bool ok;
    int i = QInputDialog::getInt(this, "Set Output Neurons Number","Neurons Number", EpochCount, 0, 100000, 1, &ok);
    if (ok)
    {
        NNParametr.OutputNeuronsNum = i;
        Warning("This value make effective if you recreate your Neural network");
    }
}

void NNframwork::askhiddenNN()
{
    bool ok;
    int i = QInputDialog::getInt(this, "Set Hidden Neurons Number","Neurons Number", EpochCount, 0, 100000, 1, &ok);
    if (ok)
    {
        NNParametr.HiddenNeuronsNum = i;
        Warning("This value make effective if you recreate your Neural network");
    }
}

void NNframwork::askHiddenAF()
{
    bool ok;
    QString title = "set Hidden Activation Function";
    QString Label = "Activation Function:";
    QStringList items = LocalNN->getAFlist();
    QString item = QInputDialog::getItem(this, title,Label, items, 0, false, &ok);
    if(ok)
    {
        NNParametr.HiddenAF = (AF_Enum)items.indexOf(item);
        LocalNN->setHiddenAF(NNParametr.HiddenAF);
        UpdateData();
    }
}

void NNframwork::askOutputAF()
{
    bool ok;
    QString title = "set Output Activation Function";
    QString Label = "Activation Function:";
    QStringList items = LocalNN->getAFlist();
    QString item = QInputDialog::getItem(this, title,Label, items, 0, false, &ok);
    if(ok)
    {
        NNParametr.OutputAF = (AF_Enum)items.indexOf(item);
        LocalNN->setOutputAF(NNParametr.OutputAF);
        UpdateData();
    }
}

void NNframwork::UpdateData()
{
    i_inputNNum->setValue(LocalNN->getInputNum());
    i_outputNNum->setValue(LocalNN->getOutputNeuronsNum());
    i_hiddentNN->setValue(LocalNN->getHiddenNeuronsNum());
    i_HiddenAF->setValue(LocalNN->getHiddenAF());
    i_OutputAF->setValue(LocalNN->getOutputAF());
    i_learningRate->setValue(LocalNN->getLearningRate());
    i_learningMomentum->setValue(LocalNN->getLearningMomentum());
    i_lastE->setValue(0);
    i_EpochNum->setValue(0);
}































