#ifndef EKGFRAMWORK_H
#define EKGFRAMWORK_H
#include "header.h"
class EKGframeWork
{
public:
    EKGframeWork();
    void operator<<(QString text);
private:
    void info(QString text);

};
#endif // EKGFRAMWORK_H
