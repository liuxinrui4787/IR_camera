#include "imageformat.h"
#include <QDebug>
#include <fstream>
#include <algorithm>

QImage imageFormat::MatToQImage(const cv::Mat& mat)
{
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar* pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar* pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if (mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        //MessageInfo("ERROR: Mat could not be converted to QImage.", 1);
        //emit sig_RunInfo("ERROR: Mat could not be converted to QImage.", 1);
        //if (!globalPara.IsInlineRun) Runstateinfo("ERROR: Mat could not be converted to QImage.", 1);
        return QImage();
    }
}
void imageFormat::matMinMaxNoise(Mat &m1)
{
    if(m1.type() == CV_8UC3)
    {
        cvtColor(m1,m1,COLOR_BGR2GRAY);
    }
    //归一化到0~255
    double minValue, maxValue;
    cv::Point  minIdx, maxIdx;
    minMaxLoc(m1,&minValue, &maxValue, &minIdx, &maxIdx);
    //****加降噪
    double minMeanValue=0,maxMeanValue=0;
    for(int y = minIdx.y - 1; y <= minIdx.y + 1;y++)
    {
        for(int x = minIdx.x - 1; x <= minIdx.x + 1;x++)
        {
            if(x == minIdx.x && y == minIdx.y)
            {

            }else
            {
                minMeanValue += m1.at<float>(y, x);
            }
        }
    }
    minMeanValue = minMeanValue/8;
    m1.at<float>(minIdx.y, minIdx.x) = minMeanValue;
    for(int y = maxIdx.y - 1; y <= maxIdx.y + 1;y++)
    {
        for(int x = maxIdx.x - 1; x <= maxIdx.x + 1;x++)
        {
            if(x == maxIdx.x && y == maxIdx.y)
            {

            }else
            {
                maxMeanValue += m1.at<float>(y, x);
            }
        }
    }
    maxMeanValue = maxMeanValue/8;
    m1.at<float>(maxIdx.y, maxIdx.x) = maxMeanValue;
}
void imageFormat::mat2gray(Mat &m1)
{
    if(m1.type() == CV_8UC3)
    {
        cvtColor(m1,m1,COLOR_BGR2GRAY);
    }
    //归一化到0~255
    double minValue, maxValue;
    cv::Point  minIdx, maxIdx;
    minMaxLoc(m1,&minValue, &maxValue, &minIdx, &maxIdx);

    if(minValue >= 0 && maxValue <= 255)
    {
        if(maxValue - minValue < 255 && maxValue > 100)
        {
            convertScaleAbs(m1,m1);
        }else
        {
            normalize(m1, m1, 0, 255, NORM_MINMAX);
            convertScaleAbs(m1,m1);
        }
    }else if(minValue == 0 && maxValue > 255)
    {
//        normalize(m1, m1, 0, 255, NORM_L1);
//        convertScaleAbs(m1,m1);
        normalize(m1, m1, 0, 255, NORM_MINMAX);
        convertScaleAbs(m1,m1);
    }else
    {
        normalize(m1, m1, 0, 255, NORM_MINMAX);
        convertScaleAbs(m1,m1);
    }
    m1.convertTo(m1,CV_8UC1);
}
Mat imageFormat::vector2mat(vector<vector<double>> vec)
{
    int rows = vec.size();
    int cols = vec[0].size();
    Mat m(rows,cols,CV_64FC1);
    for(int i = 0; i< rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            m.at<double>(i,j) = vec[i][j];
        }
    }
    return m;
}
void imageFormat::writeCSV(std::string filename, Mat m)
{
    ofstream writefile;
    writefile.open(filename.c_str());
    writefile<< format(m,Formatter::FMT_CSV);
    writefile.close();
}
void imageFormat::outputCSV(std::string filename, string s)
{
    remove(filename.c_str());
    ofstream writefile;
    writefile.open(filename.c_str());
    writefile<< s;
    writefile.close();
}
