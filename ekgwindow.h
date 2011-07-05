#ifndef EKGWINDOW_H
#define EKGWINDOW_H
#include "qrsdet.h"
#define TIMEREPEAT 60
#define TRAiNTIME 8 * SAMPLE_RATE
#define WIDTH 800
#define HEIGHT 720
#define ASKCOMPLEX false
class EKGWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit EKGWindow(QWidget *parent = 0);
    weka_data Intercept(vector<double> sig , bool getPlot = false, bool getIntercept = false);
signals:

private slots:
    void setAge();
    void setDB_Path();
    void setSigTime();
    void addDisease();
    void setDisease();
    void A_Weka_Save();
    void setRecordNum();
    void A_Sig_change();
    void A_Det_change();
    void A_Plot_change();
    void A_Fil_change();
    void Detection_Click();
    void A_WekaMode_change();
    void A_InterMode_change();
    void A_DiseaseMode_change();
    void openTrain();
    void scan();
    void openImage();
    void openRecord();

private:
    void WekaDo();
    void Info_Box();
    int askComplex(int end);
    void Wraning(QString text);
    void OpenError(char *recordNum);
    void CreateLayout(bool newplot);
    void CreateWeka(char* recordNum);
    QStringList AddRecordFile(QString patch);
    QString askDiz(const char* recornNum);
    vector<QString> getFiles(char *patch);

    void createInfo(QString name , int value);
    void createInfo(QString name , QString value);

    void plot(double *Signal,double *Detect,int size);
    vector<QString> getUnDiz(char *patch, vector<QString> values);
    //Layout
    QSpacerItem *space;
    QWidget *Main_Widget;
    QVBoxLayout *Main_Layout;
    QGridLayout *Grid_Layout;
    QHBoxLayout *Info_Layout;
    QHBoxLayout *Feuture_Layout;
    QHBoxLayout *Progress_Layout;
    //Menu
    QMenuBar *menu;
    QAction *A_Ekg_signal;
    QAction *A_Start_Train;
    QAction *A_Det_signal;
    QAction *A_Fil_signal;
    QAction *A_Plot_show;
    QAction *A_Quit;
    QAction *A_Start;
    QAction *A_DiseaseMode;
    QAction *A_InterMode;
    QAction *A_WekaMode;
    QAction *A_Save;
    QAction *A_Scan;
    QAction *A_LoadImage;
    QAction *A_clearlog;
    QAction *A_SetTime;
    QAction *A_SetDB;
    QAction *A_SetRecord;
    QAction *B_AskComplex;
    //Plot
    QwtPlot *myPlot;
    QwtPlot *interceptPlot;
    QwtPlotZoomer *zoomer;
    QwtPlotCurve *Signal_curves;
    QwtPlotCurve *Detect_curves;
    QwtPlotGrid *EKG_Grid;
    QwtPlotCurve *q_curves;
    QwtPlotCurve *r_curves;
    QwtPlotCurve *s_curves;
    QwtPlotCurve *p_curves;
    QwtPlotCurve *t_curves;
    QwtPlotCurve *u_curves;
    //Functions for Plot
    void qCurve();
    void rCurve();
    void sCurve();
    void pCurve();
    void tCurve();
    //EKG
    int mode;
    EKG_atr localFeature;
    wekaWriter LWW;                //Neural Network Train Data Constructor
    ListRW disList;                //Disease List
    weka_data localWeka;
    vector<double> Signal;
    EKGSignal sigReader;
    int ventricularrate();
    SigDetect InterCeptWindow;
    vector<Ekg_Data> localInfo;
    int InputFileSampleFrequency;
    int EKG_age ,ADCZero ,ADCUnit;
    //Gui
    double Percentage , FileMarhale;
    double filePercentage;
    QPushButton *numbers;
    QMessageBox *message;
    QFileDialog *filebrowser;
    QProgressBar *fileProgress;
    QProgressBar *DBProgress;
    QFrame *jodaKonnande;
    QLabel *filePL;//File Progress Label
    QLabel *dbPL;//Database Progress Label
    QLabel *fLabel;//EKG Feuture Label
    QGroupBox *fWidget;//EKG Feuture Widget
    QWidget *BoxWidget;
    GButton *startButton;
    GButton *TimeButton;
    GButton *RecordButton;
    //Signal Info
    int SigTime;
    QString SigRecord;
    //Scanner
    EkgScan ekgScanner;
};

#endif // EKGWINDOW_H
