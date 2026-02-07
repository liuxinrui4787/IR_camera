#ifndef CROPTHREAD_H
#define CROPTHREAD_H

#include "FileReader_Thread.h"
#include "linefinder.h"
#include "ImgCropConfig.h"
#include <QThread>


class CropThread : public QThread
{
    Q_OBJECT
public:
    virtual void run();
    explicit CropThread(QObject *parent = nullptr);
    void setFileInfo(QString path, QStringList files, int start, int len);
    void setRectList(QList<cv::Rect> &rl)
    {
        RectList = rl;
    };
    void setInfo(vector<struct ImgCropConfig> ICC);
    void setInfo(vector<int> *x, vector<int> *y, int &xDistance, int &yDistance);

    void setMode(int m)
    {
        mode = m;
    }
    void getPointList(vector<vector<Point>> &pl)
    {
        pl = pointList;
    }
    void setQSize(QSize qs)
    {
        csvReader->setSize(qs);
    }
    void setMat_Rect(Rect rect)
    {
        mat_rect = rect;
    }
    void setRotateAngle(double ca)
    {
        correctAngle = ca;
    }
    void setThreadId(int id)
    {
        ThreadId = id;
    }
    vector<Point> offsetList;
signals:
    void updateProgress(int,int);
private:
    FileReader_Thread *csvReader = new FileReader_Thread();
    LineFinder *lineFinderTool;
    QStringList m_fileName;
    QString outDirPath;
    Mat zeroMat;
    int m_fileStartId;
    Rect mat_rect;
    vector<vector<Point>> pointList;
    int ThreadId;

    int mode = 0;
    double correctAngle = 0;
    QList<cv::Rect> RectList;
    vector<int> xList;
    vector<int> yList;
    int mmx=0,mmy=0;


    void Rotate();
    void offsetCalculate();
    void offsetCalculate1();
    void crop();
    void crop1();
};

#endif // CROPTHREAD_H
