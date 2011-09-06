#include "infoboxw.h"

infoBoxW::infoBoxW(QWidget *parent) :
    QWidget(parent)
{
    //info widget
    KRecord  = new KIWidget(trUtf8("Record:"),"0");
    KTime    = new KIWidget(trUtf8("Signal Time:"),"0");
    KAge     = new KIWidget(trUtf8("Patient Age:"),"0");
    KP       = new KIWidget(trUtf8("P Amplitude:"),"0");
    KQ       = new KIWidget(trUtf8("Q Amplitude:"),"0");
    KR       = new KIWidget(trUtf8("R Amplitude:"),"0");
    KS       = new KIWidget(trUtf8("S Amplitude:"),"0");
    KT       = new KIWidget(trUtf8("T Amplitude:"),"0");
    KQT      = new KIWidget(trUtf8("QT Duration:"),"0");
    KPR      = new KIWidget(trUtf8("PR Duration:"),"0");
    KQRS     = new KIWidget(trUtf8("QRS Width:"),"0");
    KPower   = new KIWidget(trUtf8("EKG Power:"),"0");
    KBeatRate = new KIWidget(trUtf8("Heart Beat:"),"0");
    KRR      = new KIWidget(trUtf8("RR Duration:"),"0");
    KDisease = new KIWidget(trUtf8("Disease:"),"");
    //Button Widget
    scanBTN  = new GButton(GButton::Red,trUtf8("Scan"));
    openBTN  = new GButton(GButton::Orenge,trUtf8("Open Record"));
    startBTN = new GButton(GButton::Green,trUtf8("Start"));
    //layout
    layout   = new QGridLayout;
    int i = 0;
    int k = 6;
    //add info widget
    layout->addWidget(KRecord,i/k,i%k);
    i++;
    layout->addWidget(KTime,i/k,i%k);
    i++;
    layout->addWidget(KAge,i/k,i%k);
    i++;
    layout->addWidget(KP,i/k,i%k);
    i++;
    layout->addWidget(KPR,i/k,i%k);
    i++;
    layout->addWidget(KQ,i/k,i%k);
    i++;
    layout->addWidget(KR,i/k,i%k);
    i++;
    layout->addWidget(KS,i/k,i%k);
    i++;
    layout->addWidget(KT,i/k,i%k);
    i++;
    layout->addWidget(KQT,i/k,i%k);
    i++;
    layout->addWidget(KQRS,i/k,i%k);
    i++;
    layout->addWidget(KPower,i/k,i%k);
    i++;
    layout->addWidget(KBeatRate,i/k,i%k);
    i++;
    layout->addWidget(KRR,i/k,i%k);
    i++;
    layout->addWidget(KDisease,i/k,i%k);
    i++;
    //add button
    layout->addWidget(scanBTN,i/k,i%k);
    i++;
    layout->addWidget(openBTN,i/k,i%k);
    i++;
    layout->addWidget(startBTN,i/k,i%k);
    i++;
    //final
    layout->setVerticalSpacing(5);
    setLayout(layout);
    //Connect Signal and Slots
    connect(scanBTN ,SIGNAL(click()),this,SLOT(p_scan_click()));
    connect(openBTN ,SIGNAL(click()),this,SLOT(p_open_click()));
    connect(startBTN,SIGNAL(click()),this,SLOT(p_start_click()));
}

QString infoBoxW::Record()
{
    return KRecord->getValue();
}

QString infoBoxW::Time()
{
    return KTime->getValue();
}

QString infoBoxW::Age()
{
    return KAge->getValue();
}

QString infoBoxW::P()
{
    return KP->getValue();
}

QString infoBoxW::PR()
{
    return KPR->getValue();
}

QString infoBoxW::Q()
{
    return KQ->getValue();
}

QString infoBoxW::R()
{
    return KR->getValue();
}

QString infoBoxW::S()
{
    return KS->getValue();
}

QString infoBoxW::T()
{
    return KT->getValue();
}

QString infoBoxW::QT()
{
    return KQT->getValue();
}

QString infoBoxW::QRS()
{
    return KQRS->getValue();
}

QString infoBoxW::Power()
{
    return KPower->getValue();
}

QString infoBoxW::BeatRate()
{
    return KBeatRate->getValue();
}

QString infoBoxW::RR()
{
    return KRR->getValue();
}

QString infoBoxW::Disease()
{
    return KDisease->getValue();
}

void infoBoxW::setRecord(QString value)
{
    KRecord->setValue(value);
}

void infoBoxW::setTime(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " s";
    KTime->setValue(buffer);
}

void infoBoxW::setAge(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += trUtf8(" سال");
    KAge->setValue(buffer);
}

void infoBoxW::setP(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " mV";
    KP->setValue(buffer);
}

void infoBoxW::setPR(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " ms";
    KPR->setValue(buffer);
}

void infoBoxW::setQ(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " mV";
    KQ->setValue(buffer);
}

void infoBoxW::setR(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " mV";
    KR->setValue(buffer);
}

void infoBoxW::setS(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " mV";
    KS->setValue(buffer);
}

void infoBoxW::setT(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " mV";
    KT->setValue(buffer);
}

void infoBoxW::setQT(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " ms";
    KQT->setValue(buffer);
}

void infoBoxW::setQRS(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " ms";
    KQRS->setValue(buffer);
}

void infoBoxW::setPower(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " mV";
    KPower->setValue(buffer);
}

void infoBoxW::setBeatRate(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " bpm";
    KBeatRate->setValue(buffer);
}

void infoBoxW::setRR(int value)
{
    QString buffer;
    buffer.setNum(value);
    buffer += " ms";
    KRR->setValue(buffer);
}

void infoBoxW::setDisease(QString value)
{
    KDisease->setValue(value);
}

void infoBoxW::p_scan_click()
{
    emit scan_click();
}

void infoBoxW::p_open_click()
{
    emit open_click();
}

void infoBoxW::p_start_click()
{
    emit start_click();
}



























