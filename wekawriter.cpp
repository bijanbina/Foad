#include "wekawriter.h"
wekaWriter::wekaWriter(vector <QString> diseaselist)
{
    buffer = vector<weka_data > (0);
    vector <QString> disease_list = diseaselist ;
}
void wekaWriter::setwekadata(weka_data input)
{
    buffer.push_back(input);
}


void wekaWriter::settofile(QString filename)
{
    int minsize = buffer.size();
        QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QString disease_string;
    disease_string += "@ATTRIBUTE";
    disease_string += " disease";
    disease_string += " {";
        QTextStream out(&file);
    for (int i=0 ; i<disease_list.size() ; i++)
    {
        disease_string += disease_list[i] ;
        if (i < disease_list.size() -1 )
        disease_string += ",";
    }
    disease_string += "}";
    out<<QString("%hello all")<<endl;
    out<<"@RELATION disease"<<endl;
    out<<"@ATTRIBUTE "<<"R amplitude"<<"NUMERIC"<<endl;
    out<<"@ATTRIBUTE "<<"Q amplitude "<<"NUMERIC"<<endl;
    out<<"@ATTRIBUTE "<<"P detection ratio"<<"NUMERIC "<<endl;
    out<<"@ATTRIBUTE "<<"S or QS detection ratio "<<"NUMERIC "<<endl;
    out<<"@ATTRIBUTE "<< "T detection ratio "<<"NUMERIC"<<endl;
    out<<disease_string<<endl;
    out<<"@DATA \n";
        int j=0;
    while( minsize > j )
    {
        out<<buffer[j].sqsdetected<<"," ;
        out<<buffer[j].pdetected<<",";
        out<<buffer[j].tdetected<<",";
        out<<buffer[j].Ramp << ",";
        out<<buffer[j].Qamp<<",";
        out<<buffer[j].disease;
        out<<endl;
        j++;
    }
    file.close();
}
