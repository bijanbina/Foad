#include "sigdemo.h"

SigDemo::SigDemo(vector<double> signal , bool getPlot , QwtPlot *plotwidget ,  int id , int EKG_Age, int start , QObject *parent ) : QObject(parent)
{
//Define variable
    MainSig = signal;
    buffer = signal;

    QFeuture InitalizeData;
    InitalizeData.detect  = NOTDETECTED;
    InitalizeData.end     = NOTDETECTED;
    InitalizeData.height  = NOTDETECTED;
    InitalizeData.start   = NOTDETECTED;
    InitalizeData.voltage = NOTDETECTED;
    InitalizeData.width   = NOTDETECTED;

    sigInfo.qs    = InitalizeData;
    sigInfo.p     = vector<QFeuture> (0);
    sigInfo.q     = InitalizeData;
    sigInfo.r     = InitalizeData;
    sigInfo.s     = InitalizeData;
    sigInfo.t     = InitalizeData;
    sigInfo.u     = InitalizeData;
    sigInfo.delta = InitalizeData;
    sigInfo.age   = EKG_Age;
    sigInfo.Pcount = 0;
    sigInfo.RR     = MainSig.size();
    sigInfo.start  = start;

    if(buffer.size() < 60)
        return ;
//Make On base
    ZeroLine();
    ZeroShib();
    MainSig = buffer;
//Find R;
    bool HaveR = findR(),HaveP=true;
    if (HaveR)
    {
        findBeat();
        findQ();
        findS();
    }
    else
    {
        findQS();
    }

    MK_QRS_Zero(DETECTED);
    ZeroLine();
    //MK_First_Zero();
    double PT = 3.0 / 8.0;
    //Smooth Filter
    vector<double> BDetected = getDetect();
    NODetect(0);
    gussian(5);
    ZeroLine();
    setDetect(BDetected);

    if (MAX() > PT * buffer.size())
    {
        //while(HaveP)
        //{
            HaveP = findP();
            ZeroLine();
        //}
        if (MAX() < PT * buffer.size() && buffer[MAX()]> 5)
        {
            findT();
        }
    }
    else
    {
        findT();
        ZeroLine();
        if (MAX() > PT * buffer.size())
        {
            int Pcount = 0;
            findP();
            while(!AllDetected() && Pcount < 5)
            {
                Pcount++;
                //findP();
            }
        }
    }

    MK_QRS_Zero(0);
    //-----------is beat NSR ---------------
    if(sigInfo.p.size() == 1)
    {
        if(sigInfo.q.detect == DETECTED && sigInfo.r.detect == DETECTED && sigInfo.s.detect ==DETECTED)
            sigInfo.NSR = true;
    }
     else
        sigInfo.NSR = false;

//Copy to double array
    if(getPlot)
    {
        myPlot = plotwidget;
        EKG_Grid = new QwtPlotGrid();
        QString strID ;
        strID.setNum(id);
        myPlot->setTitle(strID);
        myPlot->canvas()->setFrameStyle(0);
        myPlot->setWindowIcon(QIcon(":/icon"));
        zoomer = new QwtPlotZoomer(myPlot->canvas());
        zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);
        plot(MainSig);
    }
}
//! Create plot for signal first convert it to double array
void SigDemo::plot(vector<double> Signal)
{
    double PSignal[Signal.size()];//Private Signal
    for (int i = 0 ; i < Signal.size(); i++)
    {
        PSignal[i] = Signal[i];
    }
    plot(PSignal,Signal.size());
}

void SigDemo::plot(double *Signal ,int size)
{
    myPlot->clear();
    myPlot->plotLayout()->setAlignCanvasToScales(true);
    double x[size];
    double s = 0,MaxVal = -999999, MinVal = 999999;
    for(int i = 0 ; i < size ; i++)
    {
        x[i] = s;
        s +=0.005;
        if (MaxVal < Signal[i])
            MaxVal = Signal[i];
        if (MinVal > Signal[i])
            MinVal = Signal[i];
    }
    //--------------EKG Grid Line-----------------
    EKG_Grid->enableXMin(true);
    EKG_Grid->enableYMin(true);
    myPlot->setAxisMaxMinor(QwtPlot::xBottom, 5);
    myPlot->setAxisMaxMajor(QwtPlot::xBottom, s * 10);
    myPlot->setAxisMaxMinor(QwtPlot::yLeft, 5);
    myPlot->setAxisMaxMajor(QwtPlot::yLeft, 10);
    EKG_Grid->setMajPen(QPen(QColor(125,0,0,200), 1, Qt::SolidLine));
    EKG_Grid->setMinPen(QPen(QColor(125,0,0,60), 0.2, Qt::SolidLine));
    EKG_Grid->attach(myPlot);

    //--------------- Add Curves ----------------
    Signal_curves = new QwtPlotCurve(trUtf8("Signal"));

    //--------------- Preparing -----------------
    zoomer->zoom(0);
    myPlot->setAxisTitle(myPlot->xBottom, trUtf8("Time (s)"));
    myPlot->setAxisTitle(myPlot->yLeft, trUtf8("Voltage (mV)"));
    Signal_curves->setRenderHint(QwtPlotItem::RenderAntialiased);

    // ------copy the data into the curves-----------
    Signal_curves->setData(x,Signal,size);
    //--------------- Attach Curves ----------------
    Signal_curves->attach(myPlot);
    // ADD Finded Feutures
    qCurve();
    rCurve();
    sCurve();
    pCurve();
    tCurve();
    //-----------------Set pen-----------------------
    QPen *ekgPen = new QPen(Qt::blue);
    ekgPen->setWidthF(0.5);
    ekgPen->setJoinStyle(Qt::RoundJoin);
    ekgPen->setCapStyle(Qt::RoundCap);
    Signal_curves->setPen(*ekgPen);
    //------------- Set Axis Scale-------------------
    int yStart = -600,yEnd = 600;
    if (MaxVal > yEnd)
        yEnd = MaxVal + 100;
    if (MinVal < yStart)
        yStart = MinVal - 100;
    myPlot->setAxisScale(QwtPlot::xBottom, 0, s/1);
    myPlot->setAxisScale(QwtPlot::yLeft, yStart, yEnd);
    //zoomer->zoom(0);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect4,Qt::RightButton);
    zoomer->setZoomBase();
    zoomer->setRubberBandPen(QPen(QColor(0, 155, 0), 2, Qt::SolidLine));
    zoomer->setTrackerPen(QPen(Qt::red));

    myPlot->replot();

}
void SigDemo::ZeroShib()
{
    int BestShib = -500;
    int sMin = 999999;
    int s = 0 , b = 0 ,y = 0, tedad = 0;
    int minDot = 0 , disDot = 999999;
    //Mohasebe Miangin
    for (int j = 10 ; j < buffer.size() - 30; j++)
    {
        if (buffer[j] != DETECTED)
        {
            tedad++;
            s += buffer[j];
        }
    }
    s /= tedad;
    //Find nazdiktarin noghte be miangin
    for (int j = 10 ; j < buffer.size() - 30; j++)
    {
        if (buffer[j] != DETECTED)
        {
            if (abs(buffer[j]-s) < disDot)
            {
                minDot = j;
                disDot = abs(buffer[j]-s);
            }
        }
    }
    for (int i = -100 ; i < 100 ; i++)
    {
        double a = i / 100.0;
        s = 0;
        //Mohasebe Arz Az mabda
        b = -1 * ((a * minDot) - buffer[minDot]);
        for (int x = 20 ; x < buffer.size() - 20; x++)
        {
            if (buffer[x] != DETECTED)
            {
                y = (a * x) + b;
                s += abs(buffer[x] - y);
            }
        }
        if (s < sMin)
        {
            sMin = s;
            BestShib = a;
        }
    }
    b = -1 * ((BestShib * minDot) - buffer[minDot]);
    for (int x = 0 ; x < buffer.size(); x++)
    {
        if (buffer[x] != DETECTED)
        {
            y = (BestShib * x) + b;
            buffer[x] = buffer[x] - y;
        }
    }

}
void SigDemo::ZeroLine()
{
    int BestLine = -500;
    int sMin = 999999;
    int s;
    for (int i = -500 ; i < 500 ; i++)
    {
        s = 0;
        for (int j = 15 ; j < buffer.size() - 40; j++)
        {
            if (buffer[j] != DETECTED)
                s += abs(buffer[j] - i);
        }
        if (s < sMin)
        {
            sMin = s;
            BestLine = i;
        }
    }
    for (int j = 0 ; j < buffer.size(); j++)
    {
        if (buffer[j] != DETECTED)
        buffer[j] = buffer[j] - BestLine;
    }
}
void SigDemo::findBeat()
{
    vector<double> Bbuffer = buffer;
    Pow2();
    //sigBeat = vector<double> (0);
    for(int k = 0 ; k < 7; k++)
    {
        int mMax = MAX();
        //DEBUG!!
        if (mMax > buffer.size() - 3)
            mMax = buffer.size() - 3;
        //Debug!!
        if (mMax < 2)
            mMax = 2;
        int i = mMax -1 ;
        while(i < buffer.size() -1)
        {
            i++;
            if (buffer[i] > buffer[i+1] && buffer [i] > 0)
            {
                buffer[i] = 0;
            }
            else
            {
                break;
            }
        }
        i = mMax;
        while(0 < i)
        {
            i--;
            if (buffer[i] > buffer[i-1] && buffer [i] > 0)
            {
                buffer[i] = 0;
            }
            else
            {
                break;
            }
        }
        sigBeat.push_back(mMax);
    }
    buffer = Bbuffer;
}
void SigDemo::Pow2()
{
    for (int i = 0 ; i < buffer.size() ; i++)
    {
        if (buffer[i] != DETECTED)
            buffer[i] = pow(buffer[i], 2);
    }
}
void SigDemo::ABS()
{
    for (int i = 0 ; i < MainSig.size() ; i++)
    {
        buffer[i] = abs(buffer[i]);
    }
}
void SigDemo::SQRT()
{
    for (int i = 0 ; i < buffer.size() ; i++)
    {
        if (buffer[i] != DETECTED)
        {
            buffer[i] = sqrt(buffer[i]);
            if(MainSig[i] < 0)
                buffer[i] = -buffer[i];
        }
    }
}
int SigDemo::MAX()
{
    int Makan = -1 , max = -1000;
    for (int i = 0 ; i < buffer.size(); i++)
    {
        if (buffer[i] != DETECTED)
        {
            if ( max < buffer[i])
            {
                max = buffer[i];
                Makan = i;
            }
        }
    }
    return Makan;
}
double SigDemo::Miangin(vector<double> fbuffer)
{
    int s = 0, tedad = 0;
    for (int i = 0 ; i < fbuffer.size(); i++)
    {
        if (fbuffer[i] != DETECTED)
        {
            tedad++;
            s += fbuffer[i];
        }
    }
    if (tedad == 0)
        return 0;
    s /= tedad;
    return s;
}
bool SigDemo::findR()
{
    Pow2();
    int mMax = MAX();
    SQRT();
    //DEBUG!!
    if (mMax > buffer.size() - 3)
        mMax = buffer.size() - 3;
    //Debug!!
    if (mMax < 2)
        mMax = 2;
    int i = mMax -1 ;
    bool haveDelta;
    //If have false QS detection
    if(buffer[mMax] < 0)
    {
        int qs = mMax;
        int rGuess = MAX();
        int tafavot = buffer[rGuess] - buffer[qs];
        if (tafavot > 90 && rGuess > buffer.size()/2 && buffer[rGuess] > 90)
        {
            mMax = rGuess;
        }
    }
    //END false QS detection
    if(MainSig[mMax]>=MainSig[mMax-1] && MainSig[mMax] >= MainSig[mMax-2] && MainSig[mMax] >= MainSig[mMax+1] && MainSig[mMax] >= MainSig[mMax+2])
    {
        while(i < buffer.size() -1)
        {
            i++;
            if (buffer[i] > buffer[i+1] && buffer [i] > 0)
            {
                buffer[i] = DETECTED;
            }
            else
            {
                if (buffer [i] > 0)
                {
                    haveDelta = true;
                }
                break;
            }
        }
        i = mMax;
        sigInfo.r.detect = mMax;
        while(0 < i)
        {
            i--;
            if (buffer[i] > buffer[i-1] && buffer [i] > 0)
            {
                buffer[i] = DETECTED;
            }
            else
            {
                if (buffer[i] > 0)
                {
                    haveDelta = true;
                }
                break;
            }
        }
        if (haveDelta)
        {
            //Find Delta
            //findDelta();
        }
        sigInfo.r.voltage = MainSig[mMax];
        return true;
    }
    else
    {
        return false;
    }

}
void SigDemo::findDelta()
{
    int mMax = MAX();
    int i = mMax -1 ;
    while(i < buffer.size() -1)
    {
        i++;
        if (buffer[i] > buffer[i+1] && buffer [i] > 0)
        {
            buffer[i] = DETECTED;
        }
        else
        {
            break;
        }
    }
    i = mMax;
    sigInfo.delta.detect = mMax;
    while(0 < i)
    {
        i--;
        if (buffer[i] > buffer[i-1] && buffer [i] > 0)
        {
            buffer[i] = DETECTED;
        }
        else
        {
            break;
        }
    }
    return;
}
void SigDemo::findQS()
{
    Pow2();
    int mMax = MAX();
    //DEBUG!!
    if (mMax > buffer.size() - 3)
        mMax = buffer.size() - 3;
    //Debug!!
    if (mMax < 2)
        mMax = 2;
    int i = mMax -1 ;
    while(i < buffer.size() - 1)
    {
        i++;
        if (buffer[i] >= buffer[i+1] && buffer [i] < 0)
        {
            buffer[i] = DETECTED;
        }
        else
        {
            sigInfo.qs.end = i;
            break;
        }
    }
    i = mMax;
    sigInfo.qs.detect = mMax;
    while(0 < i)
    {
        i--;
        if (buffer[i] <= buffer[i-1] && buffer [i] < 0)
        {
            buffer[i] = DETECTED;
        }
        else
        {
            sigInfo.qs.start = i;
            break;
        }
    }
    sigInfo.qs.voltage = MainSig[mMax];
    SQRT();
}
void SigDemo::findQ()
{
    int k = 0 , mMax = 0, Qstart = 0;
    for(k = sigInfo.r.detect; 2 < k; k--)
    {
        if (MainSig[k] < MainSig[k-1] && MainSig[k] < MainSig[k-2])
        {
            mMax = k;
            break;
        }
    }
    int i = mMax -1 ;
    sigInfo.q.detect = mMax;
    while(i < buffer.size() -1)
    {
        i++;
        if (buffer[i] < buffer[i+1] && buffer [i] < 0)
        {
            buffer[i] = DETECTED;
        }
        else
        {
            break;
        }
    }
    i = mMax;
    while(0 < i)
    {
        i--;
        if (buffer[i] < buffer[i-1] && buffer [i] < 0)
        {
            buffer[i] = DETECTED;
        }
        else
        {
            Qstart = i;
            break;
        }
    }
    sigInfo.q.start = Qstart;
    sigInfo.q.voltage = MainSig[mMax];
}
void SigDemo::findS()
{
    int k = 0 , mMax = -1;
    for(k = sigInfo.r.detect ; k < MainSig.size() - 2; k++)
    {
        if(MainSig[k] < MainSig[k+1] && MainSig[k] < MainSig[k+2] &&  MainSig[k] < 0)
        {
            mMax = k;
            break;
        }
    }
    if (mMax == -1)
        mMax = MainSig.size()-1;
    Pow2();
    sigInfo.s.detect = mMax;
    int i = mMax -1 ;
    while(i < buffer.size() -1)
    {
        i++;
        if (buffer[i] > buffer[i+1] && buffer [i] > 0)
        {
            buffer[i] = DETECTED;
        }
        else
        {
            sigInfo.s.end = i;
            break;
        }
    }
    i = mMax;
    while(0 < i)
    {
        i--;
        if (buffer[i] > buffer[i-1] && buffer [i] > 0)
        {
            buffer[i] = DETECTED;
        }
        else
        {
            break;
        }
    }
    sigInfo.s.voltage = MainSig[mMax];
    SQRT();

}
bool SigDemo::findP()
{
    vector<double> bufferList(8);
    int mMax = MAX();
    //DEBUG
    if(mMax + bufferList.size() > buffer.size())
        mMax = buffer.size() - bufferList.size();
    if(mMax - bufferList.size() < 0)
        mMax = bufferList.size();
    //END Bug
    int nMax = mMax , minP = 0;
    int i = mMax -1 , Pwidth = 0 , PStart = 0 , Pend = 0, Pheight = 0;
    for (int k = 1 ; k < bufferList.size() ; k++)
        bufferList[k] = buffer[mMax + k];
    while(i < buffer.size() - bufferList.size())
    {
        bufferList.push_back(buffer[i+bufferList.size()]);
        bufferList.erase(bufferList.begin());
        Pwidth++;
        i++;
        if ((getLine(bufferList) > 10 || i - mMax < 10) && buffer[i] != DETECTED)
        {
            if (MainSig[i] > MainSig[nMax])
            {
                nMax = i;
            }
            buffer[i] = DETECTED;
        }
        else
        {
            Pend = i;
            break;
        }
    }
    i = mMax;
    for (int k = 0 ; k < bufferList.size() -1 ; k++)
        bufferList[k] = buffer[mMax - bufferList.size() + k];
    while(bufferList.size() < i)
    {
        Pwidth++;
        i--;
        bufferList.insert(bufferList.begin(),buffer[i - bufferList.size()]);
        bufferList.erase(bufferList.end()-1);
        if ((getLine(bufferList) > 10 || mMax - i < 10) && buffer[i] != DETECTED)
        {
            if (MainSig[i] > MainSig[nMax])
            {
                nMax = i;
            }
            buffer[i] = DETECTED;
        }
        else
        {
            PStart = i;
            break;
        }
    }
    for(i = PStart;i < PStart + Pwidth ; i++)
        buffer[i] = DETECTED;
    if (MainSig[PStart] < MainSig[Pend])
        minP = PStart;
    else
        minP = Pend;
    Pheight = abs(MainSig[nMax] - MainSig[minP]);
    //if (Pheight >= 10 && Pwidth > 10)
    if (Pheight >= 10 && Pwidth > 0)
    {
        sigInfo.Pcount++;
        QFeuture Pbuffer;
        Pbuffer.detect = PStart + (Pwidth / 2);
        Pbuffer.voltage = MainSig[Pbuffer.detect];
        Pbuffer.start = PStart;
        Pbuffer.end = Pend;
        sigInfo.p.push_back(Pbuffer);
        //sigInfo.p = nMax;
        int sum = 0;
        for(int start = PStart ; start < PStart + Pwidth ; start++ )
            sum += buffer[start];
        cout << sum<<endl;
        sigInfo.p_area=sum;
        return true;
    }
    return false;
}
bool SigDemo::findT()
{
    int PMax = MAX();
    vector<double> bufferList(8,0);
    if(PMax + bufferList.size() > buffer.size())
        PMax = buffer.size() - bufferList.size();
    if(PMax < 8)
        PMax = bufferList.size();
    int nMax = PMax , minT = 0 ,i = PMax - 1 , Twidth = 0 , TStart = 0 , Tend = 0, Theight = 0;
    for (int k = 1 ; k < bufferList.size() ; k++)
        bufferList[k] = buffer[PMax + k];
    //-----------Find Width------------
    while(i < buffer.size() - bufferList.size())
    {
        bufferList.push_back(buffer[i+bufferList.size()]);
        bufferList.erase(bufferList.begin());
        i++;
        Twidth++;
        if ((getLine(bufferList) > 10 || i - PMax < 10) &&
                (getShib(bufferList) <= 0 || i - PMax < 10) && buffer[i] != DETECTED)
        {
            if (MainSig[i] > MainSig[nMax])
            {
                nMax = i;
            }
            buffer[i] = DETECTED;
        }
        else
        {
            Tend = i;
            break;
        }
    }
    i = PMax;
    //Add 7 sample before i to bufferList
    for (int k = 0 ; k < bufferList.size() -1; k++)
        bufferList[k] = buffer[PMax - bufferList.size()+ k];
    while(bufferList.size() < i)
    {
        i--;
        Twidth++;
        bufferList.insert(bufferList.begin(),buffer[i - bufferList.size()]);
        bufferList.erase(bufferList.end()-1);
        if ((getLine(bufferList) > 10 || i - PMax < 10) &&
                (getShib(bufferList) >= 0 || PMax -i < 10) && buffer[i] != DETECTED)
        {
            buffer[i] = DETECTED;
            if (MainSig[i] > MainSig[nMax])
            {
                nMax = i;
            }
        }
        else
        {
            TStart = i;
            break;
        }
    }
    //Find Min of T
    if (MainSig[TStart] < MainSig[Tend])
        minT = TStart;
    else
        minT = Tend;
    //Find T Height
    Theight = abs(MainSig[nMax] - MainSig[minT]);
    //Check that the dected T is Real?
    double PT = 3.0/8.0;
    if (Theight >= 10 && Twidth > 10)
    {
        if (Tend > PT)
        sigInfo.t.detect = nMax;
        sigInfo.t.voltage = MainSig[sigInfo.t.detect];
        sigInfo.t.start = TStart;
        sigInfo.t.end = Tend;
        return true;
    }
    //If Detected T is false this code run!
    return false;
}
void SigDemo::MK_QRS_Zero(int replace)
{
    int M = sigInfo.q.start;
    if (M == NOTDETECTED)
    {
        M = sigInfo.qs.start;
    }
    for (int i = M;  i < buffer.size() ; i++ )
    {
        buffer[i] = replace;
    }
}
void SigDemo::MK_First_Zero()
{
    for (int i = 0;  i < 20 ; i++ )
    {
        buffer[i] = DETECTED;
    }
}
void SigDemo::TPrepare()
{
    int k = MainSig[0];
    double PT = 3.0/8.0 * MainSig.size();
    for (int i = 0;  i < PT ; i++ )
    {
       if (buffer[i] != DETECTED)
           buffer[i] -= k;
    }
}
Ekg_Data SigDemo::getInfo()
{
    return sigInfo;
}
SigDemo::SigDemo(QObject *parent) : QObject(parent)
{
    ;
}
bool SigDemo::AllDetected()
{
    for(int i = 0 ; i < buffer.size() ; i++)
        if(buffer[i] != DETECTED)
            return false;
    return true;
}
double SigDemo::getLine(vector<double> fbuffer)
{
    double sMin = 0;
    double s = 0;
    for ( int i = 0 ; i < fbuffer.size();i++)
    {
        s += fbuffer[i];
    }
    s /= fbuffer.size();
    for (int j = 0 ; j < fbuffer.size(); j++)
    {
        if (fbuffer[j] != DETECTED)
            sMin += abs(fbuffer[j] - s);
    }
    return sMin;
}
double SigDemo::getVariance(vector<double> fbuffer)
{
    double s = 0;
    double variance = 0;
    for (int i = 0 ; i < fbuffer.size() ; i++)
    {
        s += fbuffer[i];
    }
    s /= fbuffer.size();
    for (int i = 0 ; i < fbuffer.size() ; i++)
    {
        variance += pow(fbuffer[i] - s,2);
    }
    return variance;
}
//---------------------------------------PLOT--------------------------------------------
void SigDemo::qCurve()
{
    int M = sigInfo.q.detect;
    if (M == -1)
    {
        M = sigInfo.qs.detect;
    }
   q_curves = new QwtPlotCurve;
   //Calculate Q place
   double x[1];
   x[0] = M / 200.0;
   //x[0] = M;
   double y[1];
   y[0] = MainSig[M];
   //-------------- Add Symbol -------------------
   QwtSymbol sym;
   sym.setStyle(QwtSymbol::Ellipse);
   sym.setPen(QColor(Qt::red));
   sym.setBrush(QColor(Qt::yellow));
   sym.setSize(8);
   q_curves->setSymbol(sym);
   q_curves->setStyle(QwtPlotCurve::NoCurve);
   // ------copy the data into the curves-----------
   q_curves->setData(x,y,1);
   //--------------- Attach Curves ----------------
   q_curves->attach(myPlot);
}
void SigDemo::rCurve()
{
    if(sigInfo.r.detect!=-1)
    {
        r_curves = new QwtPlotCurve;
        int M = sigInfo.r.detect;
        //Calculate R place
        double x[1];
        x[0] = M / 200.0;
        //!x[0] = M;
        double y[1];
        y[0] = MainSig[sigInfo.r.detect];
        //-------------- Add Symbol -------------------
        QwtSymbol sym;
        sym.setStyle(QwtSymbol::Ellipse);
        sym.setPen(QColor(Qt::red));
        sym.setBrush(QColor(255,100,20));
        sym.setSize(8);
        r_curves->setSymbol(sym);
        r_curves->setStyle(QwtPlotCurve::NoCurve);
        // ------copy the data into the curves-----------
        r_curves->setData(x,y,1);
        //--------------- Attach Curves ----------------
        r_curves->attach(myPlot);
    }
}
void SigDemo::sCurve()
{
    s_curves = new QwtPlotCurve;
    //Calculate S place
    int M = sigInfo.s.detect;
    if (M == -1)
    {
        M = sigInfo.qs.detect;
    }
    double x[1];
    x[0] = M / 200.0;
    //x[0] = M;
    double y[1];
    y[0] = MainSig[M];
    //-------------- Add Symbol -------------------
    QwtSymbol sym;
    sym.setStyle(QwtSymbol::Ellipse);
    sym.setPen(QColor(Qt::red));
    sym.setBrush(QColor(Qt::green));
    sym.setSize(8);
    s_curves->setSymbol(sym);
    s_curves->setStyle(QwtPlotCurve::NoCurve);
    // ------copy the data into the curves-----------
    s_curves->setData(x,y,1);
    //--------------- Attach Curves ----------------
    s_curves->attach(myPlot);
}
void SigDemo::pCurve()
{
    pBold_curves = new QwtPlotCurve;
    p_curves = new QwtPlotCurve;
    //Calculate S place
    if (sigInfo.Pcount > 0)
    {
        double x[sigInfo.Pcount];
        double y[sigInfo.Pcount];
        //!x[0] = M / 200.0;
        for (int k = 0 ; k < sigInfo.Pcount ; k++)
        {
            x[k] = sigInfo.p[k].detect / 200.0;
            y[k] = MainSig[sigInfo.p[k].detect];
        }

        //-------------- Add Symbol -------------------
        QwtSymbol sym;
        sym.setStyle(QwtSymbol::Ellipse);
        sym.setPen(QColor(Qt::red));
        sym.setBrush(QColor(Qt::magenta));
        sym.setSize(8);
        p_curves->setSymbol(sym);
        p_curves->setStyle(QwtPlotCurve::NoCurve);
        //------------ Bold P Duration ----------------
        int size = sigInfo.p[0].end - sigInfo.p[0].start + 1;
        double Xbold[size];
        double Ybold[size];
        for (int i = sigInfo.p[0].start; i <= sigInfo.p[0].end;i++)
        {
            Xbold[i - sigInfo.p[0].start] = i / 200.0;
            Ybold[i - sigInfo.p[0].start] = MainSig[i];
        }
        //-----------------Set pen-----------------------
        pBold_curves->setRenderHint(QwtPlotItem::RenderAntialiased);
        QPen ekgPen = QPen(Qt::red);
        ekgPen.setWidthF(2);
        ekgPen.setJoinStyle(Qt::RoundJoin);
        ekgPen.setCapStyle(Qt::RoundCap);
        pBold_curves->setPen(ekgPen);
        // ------copy the data into the curves-----------
        p_curves->setData(x,y,1);
        pBold_curves->setData(Xbold,Ybold,size);
        //--------------- Attach Curves ----------------
        p_curves->attach(myPlot);
        pBold_curves->attach(myPlot);
    }

}
void SigDemo::tCurve()
{
    t_curves = new QwtPlotCurve;
    tBold_curves = new QwtPlotCurve;
    //Calculate T place
    int M = sigInfo.t.detect;
    if (M != -1)
    {
        double x[1];
        x[0] = M / 200.0;
        //x[0] = M;
        double y[1];
        y[0] = MainSig[M];
        //-------------- Add Symbol -------------------
        QwtSymbol sym;
        sym.setStyle(QwtSymbol::Ellipse);
        sym.setPen(QColor(Qt::red));
        sym.setBrush(QColor(Qt::gray));
        sym.setSize(8);
        t_curves->setSymbol(sym);
        t_curves->setStyle(QwtPlotCurve::NoCurve);
        //------------ Bold T Duration ----------------
        int size = sigInfo.t.end - sigInfo.t.start + 1;
        double Xbold[size];
        double Ybold[size];
        for (int i = sigInfo.t.start; i <= sigInfo.t.end;i++)
        {
            Xbold[i - sigInfo.t.start] = i / 200.0;
            Ybold[i - sigInfo.t.start] = MainSig[i];
        }
        //-----------------Set pen-----------------------
        tBold_curves->setRenderHint(QwtPlotItem::RenderAntialiased);
        QPen ekgPen = QPen(Qt::black);
        ekgPen.setWidthF(2);
        ekgPen.setJoinStyle(Qt::RoundJoin);
        ekgPen.setCapStyle(Qt::RoundCap);
        tBold_curves->setPen(ekgPen);
        // ------copy the data into the curves-----------
        t_curves->setData(x,y,1);
        tBold_curves->setData(Xbold,Ybold,size);
        //--------------- Attach Curves ----------------
        t_curves->attach(myPlot);
        tBold_curves->attach(myPlot);
    }
}
//----------------------------------DETECTED Functions-----------------------------------
void SigDemo::NODetect(double replace)
{
    for (int i = 0 ; i < buffer.size();i++)
        if (buffer[i] == DETECTED)
            buffer[i] = replace;
}
void SigDemo::setDetect(vector<double> input)
{
    if (input.size() > buffer.size())
        return;
    for (int i = 0 ; i < input.size();i++)
        if (input[i] == DETECTED)
            buffer[i] = DETECTED;
}
vector<double> SigDemo::getDetect()
{
    vector<double> returnB(buffer.size());
    for (int i = 0 ; i < buffer.size();i++)
        if (buffer[i] == DETECTED)
            returnB[i] = DETECTED;
    return returnB;
}
//------------------------------------------Filter---------------------------------------
void SigDemo::gussian(int reduce)
{
      buffer = fastSmooth(buffer,reduce);
      buffer = fastSmooth(buffer,reduce);
      buffer = fastSmooth(buffer,reduce);
}
vector<double> SigDemo::fastSmooth(vector<double> input , int width)
{
    int SumPoints = width * (width +1);
    vector<double> s (input.size(),0);
    int halfw = width/2;
    int L= input.size();
    for (int k = 1 ; k <= L- width;k++)
    {
        s[k+halfw-1] = SumPoints;
        SumPoints = SumPoints - input[k];
        SumPoints = SumPoints + input[k+width];
    }
    int sum = 0;
    for (int i = L - width+1 ;i < L ;i++ )
    {
        sum += input[i];
    }
    s[L- width + halfw]= sum;

    for (int i = 0 ;i < s.size() ;i++)
    {
        s[i] /= width;
    }
    return s;
}
//---------------------------------Geometry Functions----------------------------------
double SigDemo::getShib(double point1,double point2)
{
    return atan(point2 - point1);
}
double SigDemo::getShib(vector<double> fbuffer)
{
    double s = 0;
    for(int i = 1;i<fbuffer.size();i++)
    {
        s += getShib(fbuffer[i-1],fbuffer[i]);
    }
    s /= fbuffer.size() -1;
    return s;
}










