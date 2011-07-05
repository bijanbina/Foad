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
    scanImage.save("scan.tiff");
    bSignal = proccessor();
}
void EkgScan::loadPic(QString path)
{
    scanImage = QImage(path);
    bSignal = proccessor();
}
int EkgScan::getRow(int row)
{
    //Mohasbe Darsad pixel haye rangi dar satr i
    double darsad;
    double width = 0;
    for(int i = 0 ; i < scanImage.width() ; i++)
    {
        if(isColor(QColor(scanImage.pixel(i,row))) > 50)
        {
            width++;
        }
    }
    darsad = width / scanImage.width();
    darsad *= 100;
    return darsad;
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
        if (getRow(i) > 10)
            break;
    }
    scanImage = scanImage.copy(0,i,scanImage.width(),scanImage.height()-i);
    for (i = scanImage.height() - 1 ; i > 0 ; i--)
    {
        if (getRow(i) > 10)
            break;
    }
    scanImage = scanImage.copy(0,0,scanImage.width(),i);
    scanImage.save("bijan.tiff");
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
    int j = ghataat.size()-1;
    int vBuffer = ghataat[j].height() - maxCount[j];
    int nBuffer = vBuffer;
    for (int column = 0 ; column < ghataat[j].width() ; column++)
    {
        nBuffer = findNazdik(column,vBuffer,ghataat[j]);
        if(nBuffer != 0)
        {
            vBuffer = nBuffer;
            bSignal.push_back(vBuffer);
            //Chech End Signal
            nBuffer = ghataat[j].height() - vBuffer;
        }

    }
    //End Read Signal

    if (bSignal.size() < 300)
        return bSignal;
    //Delete False
    bSignal.erase(bSignal.begin(),bSignal.begin()+100);
    bSignal.erase(bSignal.end()-100,bSignal.end());
    //Find Base Line
    long mMin = 0;int mMax = 0,maingin = 0;
    for(int k = 0 ; k < bSignal.size();k++)
    {
        maingin += bSignal[k];
    }
    maingin /= bSignal.size();
    for(int k = 0 ; k < bSignal.size();k++)
    {
        bSignal[k] -= maingin;
    }
//    int readSize = 200;
//    if (readSize > bSignal.size())
//        readSize = bSignal.size();

//    mMax = 0 , mMin = 0;
//    for(int i = 0 ; i < readSize ; i++)
//    {
//        if (bSignal[i] > bSignal[mMax])
//            mMax = i;
//        if(bSignal[i] < bSignal[mMin])
//            mMin = i;
//    }
//    double nesbat = 0;
//    if(abs(bSignal[mMin]) > abs(bSignal[mMax]))
//    {
//        nesbat = -300.0 / bSignal[mMin];
//        for(int k = 0 ; k < bSignal.size();k++)
//        {
//            bSignal[k] *= nesbat;
//        }
//    }
//    else
//    {
//        nesbat = 300.0 / bSignal[mMax];
//        for(int k = 0 ; k < bSignal.size();k++)
//        {
//           bSignal[k] *= nesbat;
//        }
//    }

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

vector<QImage> EkgScan::VSegmentation(QImage input)
{
    //Crop
    int red = 0,blue = 0 ,green = 0,realWidth = 0;
    vector<scanInfo> imageSum(input.width());
    //fill picture info
    for(int column = 0 ; column < input.width() ; column++)
    {
        red = 0,blue = 0 ,green = 0,realWidth = 0;
        for(int row = 0 ; row < input.height() ; row++)
        {
            QColor color = QColor(input.pixel(column,row));
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
        imageSum[column].count = realWidth; imageSum[column].red = red;
        imageSum[column].blue = blue; imageSum[column].green = green;
    }
    //End Fill info

    //Ghate ghate kardan Signal
    int start = 0,end = 0;
    vector<QImage> segments;
    bSignal = vector<double> (0);
    for(int colomn = 0 ; colomn < input.width() ; colomn++)
    {
        if(imageSum[colomn].red != 0 && imageSum[colomn].blue != 0 && imageSum[colomn].green != 0)
        {
            start = colomn;
            while(!(imageSum[colomn].red == 0 && imageSum[colomn].blue == 0 && imageSum[colomn].green == 0) && colomn < input.width())
            {
                colomn++;
            }
            end = colomn - start;
            if (end > 2)
            {
                segments.push_back(input.copy(0,start,input.height(),end));
            }
        }
        end = 0;
    }
    return segments;
}

vector<QImage> EkgScan::HSegmentation(QImage input)
{
    //Crop
    int red = 0,blue = 0 ,green = 0,realWidth = 0;
    vector<scanInfo> imageSum(input.height());
    //fill picture info
    for(int row = 0 ; row < input.height() ; row++)
    {
        red = 0,blue = 0 ,green = 0,realWidth = 0;
        for(int column = 0 ; column < input.width() ; column++)
        {
            QColor color = QColor(input.pixel(column,row));
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
    bSignal = vector<double> (0);
    for(int row = 0 ; row < input.height() ; row++)
    {
        if(imageSum[row].red != 0 && imageSum[row].blue != 0 && imageSum[row].green != 0)
        {
            start = row;
            int Mmax = 0;
            while(!(imageSum[row].red == 0 && imageSum[row].blue == 0 && imageSum[row].green == 0) && row < input.height() )
            {
                if(imageSum[row].count > imageSum[Mmax].count)
                    Mmax = row;
                row++;
            }
            end = row - start;
            if (end > 100)
            {
                ghataat.push_back(input.copy(0,start,input.width(),end));
            }
        }
        end = 0;
    }
    //End Ghate ghate kardan Signal
    return ghataat;
}


















