#ifndef LISTRW_H
#define LISTRW_H
#include "r_feature.h"

class ListRW
{
public:
    ListRW();
    vector<QString> getList();
    void addItem(QString item);
    QString getItem(int i);
    void removeItem(int i);
    int getSize();
    bool isExist();
private:
    //Variable
    QFile *DiseseFile;
    bool exist;
};

#endif // LISTRW_H
