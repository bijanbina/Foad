#include "listrw.h"

ListRW::ListRW()
{
    DiseseFile = new QFile("disease.conf");
    exist = DiseseFile->open(QIODevice::ReadOnly | QIODevice::Text);
    if (exist)
        DiseseFile->close();
}
vector<QString> ListRW::getList()
{
    vector<QString> Dizlist(0);
    if(exist)
    {
        DiseseFile->open(QIODevice::ReadOnly | QIODevice::Text);
        QString data = DiseseFile->readAll();
        int start = 0 , end;
        for(int i = 0 ; i < data.size();i++)
        {
            if(data[i].toAscii() == ';')
            {
                end = i;
                QString buffer;
                for (int j = start ; j < end ; j++)
                {
                    buffer += data[j];
                }
                Dizlist.push_back(buffer);
                start = end + 1;
            }
        }
        DiseseFile->close();
    }
    return Dizlist;
}
void ListRW::addItem(QString item)
{
    QString buffer;
    if(isExist())
    {
        DiseseFile->open(QIODevice::ReadOnly | QIODevice::Text);
        buffer = DiseseFile->readAll();
        DiseseFile->close();
    }
    DiseseFile->open(QIODevice::WriteOnly | QIODevice::Text);
    //Write Last List
    if (exist)
        DiseseFile->write(buffer.toStdString().c_str());
    //Add New Item
    DiseseFile->write(item.toStdString().c_str());
    DiseseFile->write(";");
    DiseseFile->close();
    exist = true;
}
QString ListRW::getItem(int i)
{
    vector<QString> buffer = getList();
    return buffer[i];
}
void ListRW::removeItem(int i)
{
    if (exist)
    {
        vector<QString> buffer = getList();
        //Remove Item From List
        buffer.erase(buffer.begin()+i);
        //Delete File And Create A New List
        DiseseFile->open(QIODevice::WriteOnly | QIODevice::Text);
        //Write List Again
        for (int i = 0 ; i < buffer.size();i++)
        {
            DiseseFile->write(buffer[i].toStdString().c_str());
            DiseseFile->write(";");
        }
        DiseseFile->close();
    }
}
int ListRW::getSize()
{
    vector<QString> buffer = getList();
    return buffer.size();
}
bool ListRW::isExist()
{
    return exist;
}
