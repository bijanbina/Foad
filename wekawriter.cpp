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
    out<<"@ATTRIBUTE "<<"PR "<<"NUMERIC"<<endl;
    out<<"@ATTRIBUTE "<<"QRS "<<"NUMERIC"<<endl;
    out<<"@ATTRIBUTE "<<"rate "<<"NUMERIC "<<endl;
    out<<"@ATTRIBUTE "<<"Ramp "<<"NUMERIC "<<endl;
    out<<"@ATTRIBUTE "<< "Q2T "<<"NUMERIC"<<endl;
    out<<"@ATTRIBUTE "<<"EKGpower "<<"NUMERIC"<<endl;
    out<<disease_string<<endl;
    out<<"@DATA \n";
        int j=0;
    while( minsize > j )
    {
        out<<buffer[j].PR<<"," ;
        out<<buffer[j].QRS<<",";
        out<<buffer[j].rate<<",";
        out<<buffer[j].Ramp << ",";
        out<<buffer[j].Q2T<<",";
        out<<buffer[j].EKGpower<<",";
        out<<buffer[j].disease;
        out<<endl;
        j++;
    }
    file.close();
}
