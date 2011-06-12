#ifndef WEKAWRITER_H
#define WEKAWRITER_H
#include "sigdetect.h"
struct weka_data
{
    double Qamp ;
    double Ramp ;
    double pdetected;
    double sqsdetected ;
    double tdetected;
    QString disease ;
};
class wekaWriter
{
public :
    wekaWriter(vector <QString> diseaselist);
    void setwekadata(weka_data input);
    void settofile(QString filename);
protected:
private:
    vector <QString> disease_list;
    vector<weka_data> buffer;

};

#endif // WEKAWRITER_H
