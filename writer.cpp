#include"writer.h"
#include<iostream>
#include <vector>
using namespace std ;
writer::writer()
{
    buffer = vector<weka_data > (0);

}
void writer::setwekadata(weka_data input)
{
    buffer.push_back(input);
}
void writer::settofile()
{
    int minsize = buffer.size();

    //cout<<rate.size()<<endl<<size_rate;
    // cout<<minsize;
    ofstream file("train.arff");
    file<<"%it is created by reza"<<endl;
    file<<"@RELATION reza"<<endl;
    file<<"@ATTRIBUTE "<<"PR "<<"NUMERIC"<<endl;
    file<<"@ATTRIBUTE "<<"QRS "<<"NUMERIC"<<endl;
    file<<"@ATTRIBUTE "<<"rate "<<"NUMERIC "<<endl;
    file<<"@ATTRIBUTE "<< "rhythm "<<"NUMERIC"<<endl;
    file<<"@ATTRIBUTE "<<"disease"<<"NUMERIC"<<endl;
    file<<"@DATA \n";
    int i=0;
    cout <<buffer[0].PR;
    while( minsize > i )
    {
        file<<buffer[i].PR<<"," ;
        file<<buffer[i].QRS<<",";
        file<<buffer[i].rate<<",";
        file<<buffer[i].rhythm << ",";
        file<<buffer[i].disease;
        file<<endl;
        i++;
    }
    i=0;
}
