#include "imagecropthreadinfo.h"
#include <QtMath>
#include <QCollator>
#include <QDir>
#include "imgFeatureTool.h"
#include "imageformat.h"
ImageCropThreadInfo::ImageCropThreadInfo(QObject *parent)
    : QThread(parent)
{

}
void ImageCropThreadInfo::run()
{
    if(runMode == 1)
    {
        Computer();
    }else if (runMode == 2)
    {
        RotateCorrect();
    }else if (runMode == 3)
    {
        ImgCrop();
    }
}

void ImageCropThreadInfo::setRotateCorrect(QSize s,double ca,Rect mr,QString fdp)
{
    correctAngle =ca;
    size = s;
    mat_rect = mr;
    DirPath = fdp;
    runMode = 2;
}
void ImageCropThreadInfo::Computer()
{
    m_readList.clear();
    resultList.clear();
    threadprogressList.clear();
    files.clear();
    xList.clear();
    yList.clear();
    m_csvreader = new FileReader_Thread();
    //多线程
    files = m_csvreader->readPath(DirPath);
    if(files.size() < 1)
    {
        qDebug()<<"files only one";
        return;
    }
    fileCount = files.length();

    if(fileCount < 30)
    {
        threadCount = 1;
    }
    for (int i = 0; i < threadCount; i++)
    {
        CropThread *thread = new CropThread();
        m_readList.push_back(thread);
        int progress = 0;
        threadprogressList.push_back(progress);
    }
    threadReadFilesCount = qCeil(static_cast<double>(fileCount) / m_readList.length());
    int sIdx = 0;

    QDir dir;
    dir.mkdir(DirPath+"debug/");
    dir.mkdir(DirPath+"offsetResult/");
    dir.mkdir(DirPath+"result/");

    for (int i = 0; i < m_readList.length(); i++)
    {
        connect(m_readList.at(i), SIGNAL(updateProgress(int,int)), this, SLOT(updateProgress(int,int)));
        m_readList.at(i)->setThreadId(i);
        m_readList.at(i)->setQSize(size);
        m_readList.at(i)->setFileInfo(DirPath, files, sIdx, threadReadFilesCount);
        m_readList.at(i)->setRectList(RectList);
        m_readList.at(i)->setMode(2);
        sIdx += threadReadFilesCount;
    }

    for (int i = 0; i < m_readList.length(); i++)
    {
        this->m_readList.at(i)->start();
    }
    for (int i = 0; i < m_readList.length(); i++)
    {
        this->m_readList.at(i)->wait();
    }
    //整合所有图偏移量与方向
    vector<Point> pointList;
    int index = 0;
    for (int i = 0; i < m_readList.length(); i++)
    {
        vector<vector<Point>> pl;
        this->m_readList.at(i)->getPointList(pl);
        for(vector<Point> p : pl)
        {
            pointList.push_back(Point(p.at(0).x/*+offset_x*/,p.at(0).y/*+offset_y*/));
            if(p.size() == 2)
            {
                if(p.at(0).x == p.at(1).x && p.at(0).y == p.at(1).y)
                {
                    index++;
                }
            }else if(p.size() == 1)
            {
                if(p.at(0).x/*+offset_x*/ == 0 && p.at(0).y/*+offset_y*/ == 0)
                {
                    index++;
                }else
                {
                }
            }
        }
    }
    qDebug()<<"true point count:"<<index;
    vector<vector<int>> offsetList;

    int minx=99999,miny=99999,maxx=0,maxy=0;
    string s;
    for(Point p:pointList)
    {
        s += (QString::number(p.x)+","+QString::number(p.y)+"\n").toStdString();
        if(minx > p.x)
        {
            minx = p.x;
        }
        if(miny > p.y)
        {
            miny = p.y;
        }
        if(maxx < p.x)
        {
            maxx = p.x;
        }
        if(maxy < p.y)
        {
            maxy = p.y;
        }
    }
    tempx = maxx - minx;
    tempy = maxy - miny;
    for(Point p:pointList)
    {
        xList.push_back(tempx-p.x);
        yList.push_back(tempy-p.y);
    }
    imageFormat::outputCSV(DirPath.toStdString()+"offsetResult/offsetList.csv",s);
    resultList = pointList;
    computerResult();
}
void ImageCropThreadInfo::ImgCrop()
{
    int sIdx = 0;
    m_readList.clear();
    for (int i = 0; i < threadCount; i++)
    {
        CropThread *thread = new CropThread();
        m_readList.push_back(thread);
    }
    for (int i = 0; i < m_readList.length(); i++)
    {
        connect(m_readList.at(i), SIGNAL(updateProgress(int,int)), this, SLOT(updateProgress(int,int)));
        m_readList.at(i)->setThreadId(i);
        m_readList.at(i)->setQSize(size);
        m_readList.at(i)->setFileInfo(DirPath, files, sIdx, threadReadFilesCount);
        m_readList.at(i)->setInfo(&xList,&yList,tempx,tempy);
        m_readList.at(i)->setMode(3);
        sIdx += threadReadFilesCount;
    }

    for (int i = 0; i < m_readList.length(); i++)
    {
        this->m_readList.at(i)->start();
    }
    for (int i = 0; i < m_readList.length(); i++)
    {
        this->m_readList.at(i)->wait();
    }
    allOver();
}
void ImageCropThreadInfo::RotateCorrect()
{
    allOver();
}

void ImageCropThreadInfo::updateProgress(int progress, int id)
{
    for(int i = 0; i < threadprogressList.size(); i++)
    {
        if(i == id)
        {
            threadprogressList[i] = progress;
        }
    }
    int ceil = 0;
    for(int i = 0; i < threadprogressList.size(); i++)
    {
        ceil += threadprogressList[i];
    }
    ceil = qCeil(ceil*100/fileCount);
    SetProgress(ceil);
}
