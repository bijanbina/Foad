#include "EKGFramWork.h"

EKGframeWork::EKGframeWork()
{

}

void EKGframeWork::operator<<(QString text)
{
    info(text);
}

void EKGframeWork::info(QString text)
{
    QString title = "Warning";
    QMessageBox *warnWindow = new QMessageBox(QMessageBox::Warning,title,text);
    warnWindow->setIconPixmap(QPixmap(":/warning"));
    warnWindow->setWindowIcon(QIcon(":/warning"));
    warnWindow->show();
}
