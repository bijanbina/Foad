#ifndef WEKAWRITER_H
#define WEKAWRITER_H
#include "sigdetect.h"
#define INPUT_NUMBER 5
#define OUTPUT_NUMBER 1
struct weka_data
{
    double Qamp;
    double Ramp ;
    double pdetected ;
    double sqsdetected ;
    double tdetected;
    QString disease ;
};
class wekaWriter
{
public :
    wekaWriter();
    void setDiseaseList(vector <QString> diseaselist);
    vector <QString> getDiseaseList();
    void operator<< (weka_data input);
    void operator>> (QString input);
    void add(weka_data input);
    void save(QString filename);
protected:
private:
    vector <QString> disease_list;
    vector<weka_data> buffer;
    QString getDeisyID(QString diseaName);
};

#endif // WEKAWRITER_H
