#include "ekgscan.h"

EkgScan::EkgScan()
{
    bSignal = vector<double> (0);
}
void EkgScan::scan()
{
    system("scanimage -x 1000 -y 500 --resolution 300 --format=tiff >scan.tiff");
    scanImage = QImage("scan.tiff");
    QTransform imageCorrector;
    imageCorrector = imageCorrector.rotate(-90);
    scanImage = scanImage.transformed(imageCorrector,Qt::SmoothTransformation);
    bSignal = proccessor();
}
void EkgScan::loadPic(QString path)
{
    scanImage = QImage(path);
    bSignal = proccessor();
}
QColor EkgScan::getRow(int row)
{
    int red = 0,blue = 0 ,green = 0 , width = 0;
    //Jam tamam rank haye yek satr
    for(int i = 0 ; i < scanImage.width() ; i++)
    {
        if(isColor(QColor(scanImage.pixel(i,row))) > 50)
        {
            red += QColor(scanImage.pixel(i,row)).red();
            blue += QColor(scanImage.pixel(i,row)).blue();
            green += QColor(scanImage.pixel(i,row)).green();
            width++;
        }
    }
    if (width == 0)
        width = 1;
    red /= width;
    blue /= width;
    green /= width;
    QColor returnBuffer = QColor(red,blue,green);
    return returnBuffer;
}
int EkgScan::isColor(QColor color)
{
    int miangin = color.red() + color.green() + color.blue();
    miangin /= 3;
    int returnData = abs(color.red() - miangin);
    returnData += abs(color.green() - miangin);
    returnData += abs(color.blue() - miangin);
    return returnData;
}
void EkgScan::destroyGrid(QColor color)
{
    for(int row = 0 ; row < scanImage.height() ; row++)
    {
        for(int i = 0 ; i < scanImage.width() ; i++)
        {
            //if(QColor(scanImage.pixel(i,row)).black() < 100)
            if(!isblack(QColor(scanImage.pixel(i,row))) && QColor(scanImage.pixel(i,row)).black() < 100)
            {
                scanImage.setPixel(i,row,color.rgb());
            }
        }
    }
}
bool EkgScan::isblack(QColor color)
{
    if(isColor(color) > 100)
        return false;
    if(color.red() > 150 || color.blue() > 150 || color.green() >150)
        return false;
    return true;
}
vector<double> EkgScan::proccessor()
{
    int i = 0;
    //Find Grid

    for (i = 0 ; i < scanImage.height();i++)
    {
        QColor Buffer = getRow(i);
        int jam = Buffer.red() + Buffer.blue() + Buffer.green();
        if (jam > 50)
            break;
    }
    scanImage = scanImage.copy(0,i,scanImage.width(),scanImage.height()-i);
    for (i = scanImage.height() - 1 ; i > 0 ; i--)
    {
        QColor Buffer = getRow(i);
        int jam = Buffer.red() + Buffer.blue() + Buffer.green();
        if (jam > 50)
            break;
    }
    scanImage = scanImage.copy(0,0,scanImage.width(),i);
    //End Find Grid

    //Destroy Grid
    QColor replace_color = QColor(255,255,255);
    destroyGrid(replace_color);
    //Grid Destroyed

    //Crop
    int red = 0,blue = 0 ,green = 0,realWidth = 0;
    vector<scanInfo> imageSum(scanImage.height());
    //fill picture info
    for(int row = 0 ; row < scanImage.height() ; row++)
    {
        red = 0,blue = 0 ,green = 0,realWidth = 0;
        for(int column = 0 ; column < scanImage.width() ; column++)
        {
            QColor color = QColor(scanImage.pixel(column,row));
            if(color.red() < 200 && color.blue() < 200 && color.blue() < 200)
            {
                red += color.red();
                blue += color.blue();
                green += color.green();
                realWidth++;
            }
        }
        if (realWidth == 0)
            realWidth = 1;
        red /= realWidth;
        blue /= realWidth;
        green /= realWidth;
        imageSum[row].count = realWidth; imageSum[row].red = red;
        imageSum[row].blue = blue; imageSum[row].green = green;
    }
    //End Fill info

    //Ghate ghate kardan Signal
    int start = 0,end = 0;
    vector<QImage> ghataat;
    vector<int> maxCount;
    bSignal = vector<double> (0);
    for(int row = 0 ; row < scanImage.height() ; row++)
    {
        if(imageSum[row].red != 0 && imageSum[row].blue != 0 && imageSum[row].green != 0)
        {
            start = row;
            int Mmax = 0;
            while(!(imageSum[row].red == 0 && imageSum[row].blue == 0 && imageSum[row].green == 0) && row < scanImage.height() )
            {
                if(imageSum[row].count > imageSum[Mmax].count)
                    Mmax = row;
                row++;
            }
            end = row - start;
            if (end > 100)
            {
                ghataat.push_back(scanImage.copy(0,start,scanImage.width(),end));
                maxCount.push_back(Mmax -start);
            }
        }
        end = 0;
    }
    //End Ghate ghate kardan Signal

    //Read Signal
    int j = maxCount.size()-1;
    int vBuffer = ghataat[j].height() - maxCount[j];
    int nBuffer = ghataat[j].height() - maxCount[j];
    for (int column = 0 ; column < ghataat[j].width() ; column++)
    {
        nBuffer = findNazdik(column,vBuffer,ghataat[j]);
        if(nBuffer != 0)
        {
            vBuffer = nBuffer;
            bSignal.push_back(vBuffer);
            //Chech End Signal
            nBuffer = ghataat[j].height() - vBuffer;
            //if (endSignal(ghataat[j].copy(column,nBuffer - (endSignalWidth /2), 1 , endSignalWidth)))
            if (column == 1990)
            {
                cout << column << endl;
                ghataat[j].copy(column,nBuffer - (endSignalWidth /2), 1 , endSignalWidth).save("hi.jpg");
                break;
             }
        }

    }
    //End Read Signal

    //Delete False
    long mMin = 0;int mMax = 0;
    for(int k = 0 ; k < bSignal.size();k++)
    {
        mMin += bSignal[k];
    }
    mMin /= bSignal.size();
    for(int k = 0 ; k < bSignal.size();k++)
    {
        bSignal[k] -= mMin;
    }
    int readSize = 200;
    if (readSize > bSignal.size())
        readSize = bSignal.size();

    mMax = 0 , mMin = 0;
    for(int i = 0 ; i < readSize ; i++)
    {
        if (bSignal[i] > bSignal[mMax])
            mMax = i;
        if(bSignal[i] < bSignal[mMin])
            mMin = i;
    }
    double nesbat = 0;
    if(abs(bSignal[mMin]) > abs(bSignal[mMax]))
    {
        nesbat = -300.0 / bSignal[mMin];
        for(int k = 0 ; k < bSignal.size();k++)
        {
            bSignal[k] *= nesbat;
        }
    }
    else
    {
        nesbat = 300.0 / bSignal[mMax];
        for(int k = 0 ; k < bSignal.size();k++)
        {
           bSignal[k] *= nesbat;
        }
    }

//    for (int k = 0 ; k < ghataat.size();k++)
//    {
//        QString filename = "scan";
//        QString sd;
//        sd.setNum(k+4);
//        filename += sd;
//        filename += ".png";
//        ghataat[k].save(filename);
//    }

//    scanImage.save("scan3.png");

    return bSignal;
}
int EkgScan::findNazdik(int column , int value , QImage image )
{
    int nazdik = 9999 , vNazdik = 0;
    for(int i = 0;i<image.height();i++)
    {
        QColor color = QColor(image.pixel(column,i));
        if(color.red() < 200 && color.blue() < 200 && color.blue() < 200)
        {
            int fasele = abs((image.height() - i) - value);
            if (fasele < nazdik)
            {
                nazdik = fasele;
                vNazdik = image.height() - i;
            }
        }
    }
    return vNazdik;
}
vector<double> EkgScan::getSignal()
{
    return bSignal;
}
bool EkgScan::endSignal(QImage image)
{
    int width = 0;
    for (int i = 0 ; i < image.height() ; i++)
    {
        QColor color = QColor(image.pixel(0,i));
        if(color.red() < 200 && color.blue() < 200 && color.blue() < 200)
            width++;
    }
    if (width > 73 && width < 77)
        return true;
    return false;
}






















