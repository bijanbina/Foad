#ifndef DISEAWRITER_H
#define DISEAWRITER_H
#include "feutureextraction.h"
class diseaWriter
{
public:
    diseaWriter(char *DB ,const char *recordNumber);
    bool isExist();
    QString getDisease();
    void setDisease(QString disease);
private:
    QString record;
    QFile *DiseseFile;
    bool open;
};

#endif // DISEAWRITER_H
