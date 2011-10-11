#ifndef EKGWINDOW_H
#define EKGWINDOW_H

#include "interceptw.h"

class EKGWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit EKGWindow(QWidget *parent = 0);
signals:

private slots:
    void A_Weka_Save();
    void A_Plot_change(bool val);
    void Detection_Click();
    void A_WekaMode_change();
    void A_TrainMode_change();
    void A_InterMode_change();
    void A_DiseaseMode_change();
    void openTrain();

private:
    int  getSigTime();
    void WekaDo();
    void createSandS();//Create Signal and Slot
    void createMenu();
    int  askComplex(int end);
    void Warning(QString text);
    void OpenError(char *recordNum);
    void CreateLayout();
    void update_layout();
    QStringList AddRecordFile(QString patch);
    QString askDiz(const char* recornNum);
    vector<QString> getFiles(char *patch);
    void plot(double *Signal,double *Detect,int size);
    vector<QString> getUnDiz(char *patch, vector<QString> values);
    //Layout
    QSpacerItem *space;
    QWidget *Main_Widget;
    QGridLayout *Main_Layout;
    QHBoxLayout *Progress_Layout;
    //Menu
    QMenuBar *menu;
    QAction  *A_Ekg_signal;
    QAction  *A_TrainMode;
    QAction  *A_Det_signal;
    QAction  *A_Fil_signal;
    QAction  *A_Plot_show;
    QAction  *A_Quit;
    QAction  *A_Start;
    QAction  *A_Demo;
    QAction  *A_DiseaseMode;
    QAction  *A_InterMode;
    QAction  *A_WekaMode;
    QAction  *A_Save;
    QAction  *A_Scan;
    QAction  *A_LoadImage;
    QAction  *A_clearlog;
    QAction  *A_SetTime;
    QAction  *A_SetDB;
    QAction  *A_SetRecord;
    QAction  *B_AskComplex;
    QAction  *A_addDiz;
    QAction  *A_setDiz;
    QAction  *A_setAge;
    QAction  *A_Content;
    QAction  *A_Support;
    QAction  *A_Update;
    QAction  *A_About;
    QMenu    *NNMenu;
    //EKG
    int              mode;
    EKG_atr          localFeature;
    wekaWriter       LWW;                     //Neural Network Train Data Constructor
    ListRW           disList;                //Disease List
    weka_data        localWeka;
    //Gui
    double           Percentage , FileMarhale;
    double           filePercentage;
    QPushButton     *numbers;
    QMessageBox     *message;
    QFileDialog     *filebrowser;
    QProgressBar    *fileProgress;
    QProgressBar    *DBProgress;
    QFrame          *jodaKonnande;
    QLabel          *filePL;//File Progress Label
    QLabel          *dbPL;//Database Progress Label
    //Scanner
    NNframwork      *NNWidget;
    //GUI
    InterceptW      *i_panel;//intercept panel

    EKGSignal        sigReader;
    //??
    int              WSigTime;
    QString          WSigRecord;
    vector<double>   WSignal;
};

#endif // EKGWINDOW_H
