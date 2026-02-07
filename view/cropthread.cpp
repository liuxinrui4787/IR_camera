<<<<<<< HEAD
﻿#include "cropthread.h"
#include "imageformat.h"
#include "imgFeatureTool.h"
#include <QRandomGenerator>

CropThread::CropThread(QObject *parent)
    : QThread(parent)
{

}

void CropThread::run()
{
    if(mode == 1)
    {
        lineFinderTool = new LineFinder();
        Rotate();
    }else if(mode == 2)
    {
        offsetCalculate1();
    }else if(mode == 3)
    {
        crop1();
    }
}
void CropThread::setFileInfo(QString path, QStringList files, int start, int len)
{
    m_fileName.clear();
    outDirPath = path;
    m_fileStartId = start;

    for (int i = 0; i < len; i++)
    {
        if (files.length() <= start + i)
        {
            break;
        }
        m_fileName.push_back(path + files.at(start + i));
    }
    //初始化基准图像
    zeroMat = csvReader->readFileToMat((path + files.at(0)).toStdString());
}

void CropThread::setInfo(vector<int>* x, vector<int>* y,int &xDistance,int &yDistance)
{
    xList = *x;
    yList = *y;
    mmx = xDistance;
    mmy = yDistance;
}

void CropThread::Rotate()
{

}
void CropThread::offsetCalculate1()
{
    pointList.clear();
    for (int i = 0; i < m_fileName.length(); i++)
    {
        csvReader->readfile(m_fileName.at(i).toStdString());
        Mat m = csvReader->readFileToMat(m_fileName.at(i).toStdString());
        vector<Point> offsetPoint;
        vector<vector<Mat>> rectMatList;

        for(int c = 0; c < RectList.size(); c++)
        {
            Mat mat,zero;
            if(RectList.at(c).width != 0 && RectList.at(c).height != 0)
            {
                mat = m(RectList.at(c));
                zero = zeroMat(RectList.at(c));
            }else
            {
                qDebug()<<"error : rect is null";
                break;
            }
            vector<Mat> newRect;
            newRect.push_back(zero);
            newRect.push_back(mat);
            rectMatList.push_back(newRect);
        }
        Point result;
        result = imgFeatureTool::matDiffer1(rectMatList);
        offsetPoint.push_back(result);
        pointList.push_back(offsetPoint);
        updateProgress(i+1,ThreadId);
        msleep(1);
    }
    msleep(1);
}

void CropThread::crop1()
{
    for (int i = 0; i < m_fileName.length(); i++)
    {
        int offset_x=0;
        int offset_y=0;
        offset_x = xList.at(i+m_fileStartId);
        offset_y = yList.at(i+m_fileStartId);
        Mat translation;
        Mat m = csvReader->readFileToMat(m_fileName.at(i).toStdString());
        translation = m.clone();
        imageFormat::writeCSV(outDirPath.toStdString()+"result/"+QString::number(m_fileStartId+i+1).toStdString()+".csv",translation);

        updateProgress(i+1,ThreadId);
        msleep(1);
    }
}
=======
﻿#include "cropthread.h"
#include "imageformat.h"
#include "imgFeatureTool.h"
#include <QRandomGenerator>

CropThread::CropThread(QObject *parent)
    : QThread(parent)
{

}

void CropThread::run()
{
    if(mode == 1)
    {
        lineFinderTool = new LineFinder();
        Rotate();
    }else if(mode == 2)
    {
        offsetCalculate1();
    }else if(mode == 3)
    {
        crop1();
    }
}
void CropThread::setFileInfo(QString path, QStringList files, int start, int len)
{
    m_fileName.clear();
    outDirPath = path;
    m_fileStartId = start;

    for (int i = 0; i < len; i++)
    {
        if (files.length() <= start + i)
        {
            break;
        }
        m_fileName.push_back(path + files.at(start + i));
    }
    //初始化基准图像
    zeroMat = csvReader->readFileToMat((path + files.at(0)).toStdString());
}

void CropThread::setInfo(vector<int>* x, vector<int>* y,int &xDistance,int &yDistance)
{
    xList = *x;
    yList = *y;
    mmx = xDistance;
    mmy = yDistance;
}

void CropThread::Rotate()
{

}
void CropThread::offsetCalculate1()
{
    pointList.clear();
    for (int i = 0; i < m_fileName.length(); i++)
    {
        csvReader->readfile(m_fileName.at(i).toStdString());
        Mat m = csvReader->readFileToMat(m_fileName.at(i).toStdString());
        vector<Point> offsetPoint;
        vector<vector<Mat>> rectMatList;

        for(int c = 0; c < RectList.size(); c++)
        {
            Mat mat,zero;
            if(RectList.at(c).width != 0 && RectList.at(c).height != 0)
            {
                mat = m(RectList.at(c));
                zero = zeroMat(RectList.at(c));
            }else
            {
                qDebug()<<"error : rect is null";
                break;
            }
            vector<Mat> newRect;
            newRect.push_back(zero);
            newRect.push_back(mat);
            rectMatList.push_back(newRect);
        }
        Point result;
        result = imgFeatureTool::matDiffer1(rectMatList);
        offsetPoint.push_back(result);
        pointList.push_back(offsetPoint);
        updateProgress(i+1,ThreadId);
        msleep(1);
    }
    msleep(1);
}

void CropThread::crop1()
{
    for (int i = 0; i < m_fileName.length(); i++)
    {
        int offset_x=0;
        int offset_y=0;
        offset_x = xList.at(i+m_fileStartId);
        offset_y = yList.at(i+m_fileStartId);
        Mat translation;
        Mat m = csvReader->readFileToMat(m_fileName.at(i).toStdString());
        translation = m.clone();
        imageFormat::writeCSV(outDirPath.toStdString()+"result/"+QString::number(m_fileStartId+i+1).toStdString()+".csv",translation);

        updateProgress(i+1,ThreadId);
        msleep(1);
    }
}
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
