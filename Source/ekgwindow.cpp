#include "ekgwindow.h"

/*--------------------------------------------------------------------------------------------
|                                          GUI                                                |
---------------------------------------------------------------------------------------------*/
EKGWindow::EKGWindow(QWidget *parent) :QMainWindow(parent)
{
    //set Default
    mode                             = 1;
    WSigTime                         = 20;
    WSigRecord                       = "100";
    Percentage                       = 0;

    //Create Layout
    CreateLayout();

    //ADD Menu
    createMenu();
    //Make center
    resize(WIDTH,HEIGHT);
    QRect available_geom = QDesktopWidget().availableGeometry();
    QRect current_geom = frameGeometry();
    setGeometry(available_geom.width() / 2 - current_geom.width() / 2,
                available_geom.height() / 2 - current_geom.height() / 2,
                current_geom.width(),current_geom.height());
    if (!disList.isExist())
            Warning(trUtf8("\nYou doesn't create a disease list\nfor do this go to EKG menu and click on \"Add Desase\""));
    //connect Signal and slot
    createSandS();
}
void EKGWindow::A_Plot_change(bool val)
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
    i_panel->A_Plot_change(val);
}
void EKGWindow::A_Weka_Save()
{
    QString title = trUtf8("Save Weka");
    QString path = QFileDialog::getSaveFileName(this,title,"",trUtf8("Train File(*.dat);"));
    if(!path.isEmpty())
        LWW >> path;
}
void EKGWindow::CreateLayout()
{
    //Create Objects
    NNWidget        = new NNframwork;
    filePL          = new QLabel(trUtf8("File Progress:"));
    dbPL            = new QLabel(trUtf8("All Progress :"));
    Main_Layout     = new QGridLayout;
    DBProgress      = new QProgressBar;
    fileProgress    = new QProgressBar;
    Progress_Layout = new QHBoxLayout;
    Main_Widget     = new QWidget;
    i_panel         = new InterceptW;
    //Start making layout
    Main_Layout->addWidget(i_panel,Main_Layout->rowCount(),0);
    //Window
    Main_Widget->setLayout(Main_Layout);
    setWindowIcon(QIcon(":/icon"));
    setWindowTitle(trUtf8("Foad"));
    setCentralWidget(Main_Widget);
    //setLayoutDirection(Qt::RightToLeft);
}
void EKGWindow::openTrain()
{
    QString title = "Open Train File";
    QFileDialog::Options options;
    QString selectedFilter;
    QString files = QFileDialog::getOpenFileName(this, title,"","Trained File (*.dat)",&selectedFilter,options);
    if(!files.isEmpty())
    {
        //QFileInfo recordFile = QFileInfo(files);
        //SigRecord = recordFile.baseName();
    }
}
void EKGWindow::createSandS()
{
    connect(A_TrainMode,  SIGNAL(triggered(bool)),this,SLOT(A_TrainMode_change()));
    connect(A_Quit,       SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(A_SetDB,      SIGNAL(triggered(bool)),i_panel,SLOT(setDB_Path()));
    connect(A_Save,       SIGNAL(triggered(bool)),this,SLOT(A_Weka_Save()));
    connect(A_Start,      SIGNAL(triggered(bool)),this,SLOT(Detection_Click()));
    connect(A_Plot_show,  SIGNAL(triggered(bool)),this,SLOT(A_Plot_change(bool)));
    connect(A_Det_signal, SIGNAL(triggered(bool)),i_panel,SLOT(A_Det_change(bool)));
    connect(A_Ekg_signal, SIGNAL(triggered(bool)),i_panel,SLOT(A_Sig_change(bool)));
    connect(A_Fil_signal, SIGNAL(triggered(bool)),i_panel,SLOT(A_Fil_change(bool)));
    connect(A_WekaMode,   SIGNAL(triggered(bool)),this,SLOT(A_WekaMode_change()));
    connect(A_InterMode,  SIGNAL(triggered(bool)),this,SLOT(A_InterMode_change()));
    connect(A_DiseaseMode,SIGNAL(triggered(bool)),this,SLOT(A_DiseaseMode_change()));
    connect(A_SetRecord,  SIGNAL(triggered(bool)),i_panel,SLOT(setRecordNum()));
    connect(A_SetTime,    SIGNAL(triggered(bool)),i_panel,SLOT(setSigTime()));
    connect(A_setAge,     SIGNAL(triggered(bool)),i_panel,SLOT(setAge()));
    connect(A_addDiz,     SIGNAL(triggered(bool)),i_panel,SLOT(addDisease()));
    connect(A_setDiz,     SIGNAL(triggered(bool)),i_panel,SLOT(setDisease()));
    connect(A_Scan,       SIGNAL(triggered(bool)),i_panel,SLOT(scan()));
    connect(A_LoadImage,  SIGNAL(triggered(bool)),i_panel,SLOT(openImage()));
    connect(B_AskComplex, SIGNAL(triggered(bool)),i_panel,SLOT(B_AskComplex_change(bool)));
}
void EKGWindow::createMenu()
{
    menu = new QMenuBar;
    menu->setLayoutDirection(Qt::RightToLeft);
    setMenuBar(menu);
    //File Menu
    QMenu *FileMenu = menu->addMenu(trUtf8("File"));
    A_Start = FileMenu->addAction(trUtf8("Start"));
    A_Scan = FileMenu->addAction(trUtf8("Scan"));
    A_LoadImage = FileMenu->addAction(trUtf8("Open Image"));
    A_Save = FileMenu->addAction(trUtf8("Save Weka"));
    A_Save->setEnabled(false);
    FileMenu->addSeparator();
    FileMenu->addSeparator();
    //Mode Menu in File
    QMenu *ModeMenu = FileMenu->addMenu(trUtf8("Mode"));
    A_InterMode     = ModeMenu->addAction(trUtf8("Intercepting"));
    A_DiseaseMode   = ModeMenu->addAction(trUtf8("Set Disease"));
    A_WekaMode      = ModeMenu->addAction(trUtf8("Data Constructing"));
    A_TrainMode     = ModeMenu->addAction(trUtf8("Train Mode"));
    //set Checkable Modes
    A_DiseaseMode->setCheckable(true);
    A_InterMode->setCheckable  (true);
    A_WekaMode->setCheckable   (true);
    A_TrainMode->setCheckable(true);

    A_InterMode->setChecked(true);
    FileMenu->addSeparator();
    A_Quit = FileMenu->addAction(trUtf8("Quit"));
    //Plot Menu
    QMenu *plotMenu = menu->addMenu(trUtf8("Plot"));
    A_Plot_show     = plotMenu->addAction(trUtf8("Show Plot"));
    A_Ekg_signal    = plotMenu->addAction(trUtf8("Ekg Signal"));
    A_Det_signal    = plotMenu->addAction(trUtf8("Detected Signal"));
    A_Fil_signal    = plotMenu->addAction(trUtf8("Show Feuture"));

    A_Plot_show->setCheckable (true);
    A_Ekg_signal->setCheckable(true);
    A_Det_signal->setCheckable(true);
    A_Fil_signal->setCheckable(true);

    A_Plot_show->setChecked (true);
    A_Ekg_signal->setChecked(true);
    A_Det_signal->setChecked(true);
    //Signal Menu
    QMenu *SigMenu  = menu->addMenu(trUtf8("Signal"));
    A_SetTime       = SigMenu->addAction(trUtf8("Signal Time"));
    A_SetDB         = SigMenu->addAction(trUtf8("Database Path"));
    A_SetRecord     = SigMenu->addAction(trUtf8("Record Number"));
    B_AskComplex    = SigMenu->addAction(trUtf8("Ask Complex Number"));

    B_AskComplex->setCheckable(true);
    B_AskComplex->setChecked(ASKCOMPLEX);
    i_panel->B_AskComplex_change(ASKCOMPLEX);
    //Ekg Menu
    QMenu *EKGMenu  = menu->addMenu(trUtf8("EKG"));
    A_addDiz        = EKGMenu->addAction(trUtf8("Add Disease"));
    A_setDiz        = EKGMenu->addAction(trUtf8("Set Disease"));
    A_setAge        = EKGMenu->addAction(trUtf8("Set EKG Age"));
    //Neural Network Menu
    NNMenu          = NNWidget->getMenu();
    menu->addMenu(NNMenu);
    NNMenu->setEnabled(false);
    //Help Menu
    QMenu *HelpMenu = menu->addMenu(trUtf8("Help"));
    A_Content       = HelpMenu->addAction(trUtf8("Content"));
    A_Support       = HelpMenu->addAction(trUtf8("Suppot"));
    A_Update        = HelpMenu->addAction(trUtf8("Update"));
    A_About         = HelpMenu->addAction(trUtf8("About"));
}
/*--------------------------------------------------------------------------------------------
|                                  Signal Proccessing                                         |
---------------------------------------------------------------------------------------------*/
void EKGWindow::Detection_Click()
{
    if (mode == 1)
    {
        i_panel->Detection_Click();
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
        WSigTime = getSigTime();
        for (int k = 0 ; k < selectedRecord.count();k++)
        {
            filePercentage = double(k) * FileMarhale;
            DBProgress->setValue(filePercentage);
            Percentage = 0;
            WSigRecord = selectedRecord[k];
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
    if (!sigReader.open(WSigRecord))
        return;
    while(!wekaBuffer.disease.isEmpty())
    {
        fileProgress->setValue(Percentage);
        DBProgress->setValue(filePercentage);
        if (!sigReader.read(WSigTime))
            break;
        WSignal = sigReader.getSignal();
        wekaBuffer = i_panel->Intercept(WSignal);
        LWW << wekaBuffer;
    }
    update();
    fileProgress->setValue(100);
}
/*--------------------------------------------------------------------------------------------
|                                   Menu And Dialog                                          |
---------------------------------------------------------------------------------------------*/
int EKGWindow::getSigTime()
{
//! [0]
    bool ok;
    int i = QInputDialog::getInt(this, "Set Signal Time",
                                 "Time", WSigTime, 0, 100000, 1, &ok);
    if (ok)
        return i;
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
void EKGWindow::Warning(QString text)
{
    QString title = "Warning";
    QMessageBox *warnWindow = new QMessageBox(QMessageBox::Warning,title,text);
    warnWindow->setIconPixmap(QPixmap(":/warning"));
    warnWindow->setWindowIcon(QIcon(":/warning"));
    warnWindow->show();
}
//! find .diz file from a patch
QStringList EKGWindow::AddRecordFile(QString path)
{
    QDir dbFinder(path);
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
    mode = 4;
    if (A_TrainMode->isChecked())
    {
        resize(800,600);
        A_DiseaseMode->setChecked(false);
        A_WekaMode->setChecked   (false);
        A_InterMode->setChecked  (false);
    }
    else
    {
        A_InterMode->setChecked(true);
    }
    update_layout();
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
    update_layout();
    //resize(WIDTH,HEIGHT);
}
void EKGWindow::A_DiseaseMode_change()
{
    mode = 2;
    resize(600,400);
    update_layout();
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
    update_layout();
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
//! update GUI when mode changed
void EKGWindow::update_layout()
{
    //Detach All widget
    filePL->setParent(NULL);
    dbPL->setParent(NULL);
    DBProgress->setParent(NULL);
    fileProgress->setParent(NULL);
    i_panel->setParent(NULL);
    NNWidget->setParent(NULL);
    //Start creating layout
    Progress_Layout = new QHBoxLayout;
    switch(mode)
    {
    case 1:
        Main_Layout->addWidget(i_panel,Main_Layout->rowCount(),0);
        break;
    case 2:
        Progress_Layout->addWidget(filePL);
        Progress_Layout->addWidget(fileProgress);
        Main_Layout->addLayout(Progress_Layout, Main_Layout->rowCount(),0);
        break;
    case 3:
        Progress_Layout->addWidget(filePL);
        Progress_Layout->addWidget(fileProgress);
        Main_Layout->addLayout(Progress_Layout , Main_Layout->rowCount(),0);
        Progress_Layout = new QHBoxLayout;
        Progress_Layout->addWidget(dbPL);
        Progress_Layout->addWidget(DBProgress);
        Main_Layout->addLayout(Progress_Layout, Main_Layout->rowCount(),0);
        break;
    case 4:
        Main_Layout->addWidget(NNWidget,Main_Layout->rowCount(),0);
    }
}











