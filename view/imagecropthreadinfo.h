#ifndef IMAGECROPTHREADINFO_H
#define IMAGECROPTHREADINFO_H

#include <QThread>
#include "cropthread.h"



class ImageCropThreadInfo: public QThread
{
    Q_OBJECT
public:
    virtual void run();
    explicit ImageCropThreadInfo(QObject *parent = nullptr);

    void setRedress(QSize qs,QList<cv::Rect> &list,QString dir)
    {
        RectList = list;
        DirPath = dir;
        size = qs;
        runMode = 1;
    };
    void DoCrop()
    {
        runMode = 3;
    };
    void setRotateCorrect(QSize s, double ca, Rect mr, QString fdp);

private slots:
    void updateProgress(int progress,int id);
signals:
    void SetProgress(int);
    void allOver();
    void computerResult();
public:
    vector<Point> getResultList()
    {
        resultList.push_back(Point(0,0));
        return resultList;
    }
private:
    void Computer();
    void ImgCrop();
    void RotateCorrect();
    vector<Point> resultList;

    QList<CropThread *> m_readList;
    int fileCount;
    FileReader_Thread *m_csvreader;
    QSize size;
    QList<cv::Rect> RectList;
    QString DirPath;
    double correctAngle;
    Rect mat_rect;
    int runMode=1;
    QVector<int> threadprogressList;
    vector<int> xList,yList;
    int tempx,tempy;
    int threadReadFilesCount;
    QStringList files;
    int threadCount = 10;
};

#endif // IMAGECROPTHREADINFO_H
