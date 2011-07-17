#include "ekgimagep.h"

EKGImageP::EKGImageP()
{
    bSignal = vector<double> (0);
}
void EKGImageP::loadPic(QString path)
{
    scanImage = QImage(path);
    mm2Pixel  = scanImage.height() / 210.0; // 1 millimeter is mm2Pixel pixel
}
int EKGImageP::getRow(int row)
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
int EKGImageP::isColor(QColor color)
{
    int miangin = color.red() + color.green() + color.blue();
    miangin /= 3;
    int returnData = abs(color.red() - miangin);
    returnData += abs(color.green() - miangin);
    returnData += abs(color.blue() - miangin);
    return returnData;
}
void EKGImageP::destroyGrid(QColor color)
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
bool EKGImageP::isblack(QColor color)
{
    if(isColor(color) > 100)
        return false;
    if(color.red() > 150 || color.blue() > 150 || color.green() >150)
        return false;
    return true;
}
vector<double> EKGImageP::proccessor()
{
    if (scanImage.height() < 1)
        return vector<double>();
    //Find Grid
    scanImage = crop2grid(scanImage);
    //Destroy Grid
    QColor replace_color = QColor(255,255,255);
    destroyGrid(replace_color);
    //Crop
    //Fill info
    vector<ImageInfo> imageSum = getPicInfo(scanImage);

    //Ghate ghate kardan Signal
    int start = 0,end = 0;
    vector<QImage> ghataat;
    vector<int> maxCount;
    bSignal = vector<double> (0);
    for(int row = 0 ; row < scanImage.height() ; row++)
    {
        if(BlackPixel(imageSum[row]))
        {
            start = row;
            int Mmax = 0;
            while(BlackPixel(imageSum[row]) && row < scanImage.height() )
            {
                if(imageSum[row].count > imageSum[Mmax].count)
                    Mmax = row;
                row++;
            }
            end = row - start;
            ghataat.push_back(scanImage.copy(0,start,scanImage.width(),end));
            maxCount.push_back(Mmax - start);
            //Check Vertical Count
            QImage input = ghataat[ghataat.size()-1];
            vector<QImage> vseg = VSegmentation(input);
            if (!HavaWidth(vseg ,input.width() / 2))
            {
                ghataat.pop_back();
                maxCount.pop_back();
            }
        }
        end = 0;
    }
    //End Ghate ghate kardan Signal

//    for (int k = 0 ; k < ghataat.size();k++)
//    {
//        QString filename = "scan";
//        QString sd;
//        sd.setNum(k+4);
//        filename += sd;
//        filename += ".png";
//        ghataat[k].save(filename);
//    }

    if (ghataat.size() < 1)
        return bSignal;
    //Read Signal
    int j = ghataat.size()-1;
    int vBuffer = ghataat[j].height() - maxCount[j];
    int nBuffer = vBuffer;
    ghataat[j] = bijooFilter(ghataat[j]);
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
    int maingin = 0;
    for(int k = 0 ; k < bSignal.size();k++)
    {
        maingin += bSignal[k];
    }
    maingin /= bSignal.size();
    for(int k = 0 ; k < bSignal.size();k++)
    {
        bSignal[k] -= maingin;
    }

    return bSignal;
}
/*!
  \sa find nearest pixel to value param
  \param input imageInfo it can create manual!
  \return return 0 if no black pixel found and return distance of nearest pixel to value
*/
int EKGImageP::findNazdik(int column , int value , QImage image )
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
vector<double> EKGImageP::getSignal()
{
    proccessor();
    return bSignal;
}
vector<QImage> EKGImageP::VSegmentation(QImage input)
{
    vector<ImageInfo> imageSum = getVInfo(input);
    input.save("input.png");
    //Ghate ghate kardan Signal
    int start = 0,end = 0,tool = 0,colomn = 0;
    vector<QImage> ghataat;
    while(!BlackPixel(imageSum[colomn]))
        colomn++;
    start = colomn;
    for(; colomn < input.width() ; colomn++)
    {
        if(!BlackPixel(imageSum[colomn]))
        {
            end = colomn;
            while(!BlackPixel(imageSum[colomn]) && colomn < input.width())
                colomn++;
            tool = colomn - end;
            if (tool > mm2Pixel)
            {
                ghataat.push_back(input.copy(start,0,end-start,input.height()));
                colomn++;
                start = colomn;
            }
        }
    }

    return ghataat;
}

vector<QImage> EKGImageP::HSegmentation(QImage input)
{
    int tedad = 0;
    vector<ImageInfo> imageSum = getPicInfo(input);
    //End Fill info

    //Ghate ghate kardan Signal
    int start = 0,end = 0;
    vector<QImage> ghataat;
    bSignal = vector<double> (0);
    for(int row = 0 ; row < input.height() ; row++)
    {
        if(BlackPixel(imageSum[row]))
        {
            start = row;
            int Mmax = 0;
            while(BlackPixel(imageSum[row]) && row < input.height() )
            {
                if(imageSum[row].count > imageSum[Mmax].count)
                    Mmax = row;
                row++;
            }
            end = row - start;
            if (end > 100)
            {
                tedad++;
                ghataat.push_back(input.copy(0,start,input.width(),end));
            }
        }
        end = 0;
    }
    //End Ghate ghate kardan Signal
    return ghataat;
}

QImage EKGImageP::bijooFilter(QImage input)
{
    QColor replace_color = QColor(255,255,255);
    QColor fill_color = QColor(0,0,0);
    for(int column = 0;column<input.width();column++)
    {
        for(int row = 0; row < input.height();row++)
            if(isblack(input.pixel(column,row)))
            {
                int start = row;
                while(isblack(input.pixel(column,row)))
                {
                    input.setPixel(column,row,replace_color.rgb());
                    row++;
                    if (!(row < input.height()))
                        break;
                }
                if (row >= input.height())
                    row = input.height() - 1;
                int Mreplace = start;
                Mreplace = (row + start) / 2;
                input.setPixel(column,Mreplace,fill_color.rgb());
            }
    }
    return input;
}

QImage EKGImageP::crop2grid(QImage input)
{
    int i= 0;
    for (i = 0 ; i < input.height();i++)
    {
        if (getRow(i) > 10)
            break;
    }
    input = input.copy(0,i,input.width(),input.height()-i);
    for (i = input.height() - 1 ; i > 0 ; i--)
    {
        if (getRow(i) > 10)
            break;
    }
    input = input.copy(0,0,input.width(),i);
    return input;
}

vector<ImageInfo> EKGImageP::getPicInfo(QImage input)
{
    int red = 0,blue = 0 ,green = 0,realWidth = 0;
    vector<ImageInfo> imageSum(input.height());
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
    return imageSum;
}

vector<ImageInfo> EKGImageP::getVInfo(QImage input)
{
    int red = 0,blue = 0 ,green = 0,realWidth = 0;
    vector<ImageInfo> imageSum(input.width());
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

    return imageSum;
}

/*!
  \sa if have Black pixel return true
  \param input imageInfo it can create manual!
*/
bool EKGImageP::BlackPixel(ImageInfo input)
{
    if(input.red != 0 && input.blue != 0 && input.green != 0)
        return true;
    return false;
}


/*!
  \sa find out have any image that bigger than size value
  \param input imageInfo it can create manual!
*/
bool EKGImageP::HavaWidth(vector<QImage> input,int width)
{
    for(int i = 0;i<input.size();i++)
        if(input[i].width() >= width)
            return true;
    return false;
}

















