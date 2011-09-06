#include "wekawriter.h"
wekaWriter::wekaWriter()
{
    buffer = vector<weka_data> (0);
}
void wekaWriter::setDiseaseList(vector <QString> diseaselist)
{
    disease_list = diseaselist ;
}
void wekaWriter::operator<< (weka_data input)
{
    buffer.push_back(input);
}
void wekaWriter::add(weka_data input)
{
    buffer.push_back(input);
}
void wekaWriter::save(QString filename)
{
    if(disease_list.size() < 1)
        return;
    //Filter Uncorrect Data
    for(int j = 0;j < buffer.size() ; j++)
        if (buffer[j].sqsdetected == 0 && buffer[j].pdetected == 0 && buffer[j].tdetected == 0 && buffer[j].Ramp == 0)
            buffer.erase(buffer.begin() + j);

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    //Header
    out << buffer.size() << " " << INPUT_NUMBER << " " << OUTPUT_NUMBER << endl;
    //Data
    for(int j = 0;j < buffer.size() ; j++)
    {
        if (j != 0)
            out << endl;
        out << buffer[j].sqsdetected << " " ;
        out << buffer[j].pdetected << " ";
        out << buffer[j].tdetected << " ";
        out << buffer[j].Ramp << " ";
        out << buffer[j].Qamp << endl;
        out << getDeisyID(buffer[j].disease);
    }
    file.close();
}
void wekaWriter::operator>> (QString filename)
{
    save(filename);
}

QString wekaWriter::getDeisyID(QString diseaName)
{
    int ID, i;
    QString DeisyID;
    for(int i = 0;i<disease_list.size();i++)
    {
        if(disease_list[i] == diseaName)
        {
            ID = i;
            break;
        }
    }
    for (i = 0 ; i < ID ; i++)
    {
        DeisyID +="0 ";
    }
    DeisyID += disease_list[ID];
    i++;
    for (i = 0 ; i < disease_list.size() ; i++)
    {
        DeisyID +=" 0";
    }

    return DeisyID;
}
