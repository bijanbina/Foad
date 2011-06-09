#include "diseawriter.h"

diseaWriter::diseaWriter(char *DB ,const char *recordNumber)
{
    record = QString(DB);
    record += "/";
    record += recordNumber;
    record += ".diz";
    DiseseFile = new QFile(record);
    open = DiseseFile->open(QIODevice::ReadOnly | QIODevice::Text);
    if (open)
        DiseseFile->close();
}
bool diseaWriter::isExist()
{
     return open;
}
QString diseaWriter::getDisease()
{
    QString buffer;
    if(isExist())
    {
        DiseseFile->open(QIODevice::ReadOnly | QIODevice::Text);
        buffer = DiseseFile->readLine();
        DiseseFile->close();
        return buffer;
    }
    return buffer;
}
void diseaWriter::setDisease(QString disease)
{
    open = DiseseFile->open(QIODevice::WriteOnly | QIODevice::Text);
    if (!open)
        return;
    DiseseFile->write(disease.toStdString().c_str());
    DiseseFile->close();
}
