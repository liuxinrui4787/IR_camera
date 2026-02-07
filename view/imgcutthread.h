#ifndef IMGCUTTHREAD_H
#define IMGCUTTHREAD_H
#include <QThread>
#include <opencv2/core.hpp>



class ImgCutThread : public QThread
{
    Q_OBJECT
public:
    explicit ImgCutThread(QObject *parent = nullptr);
    virtual void run();
    QStringList readPath(QString dirpath);
    cv::Mat readFileToMat(std::string fileName);
    
    void setfileList(QStringList& List)
    {
        fileList = List;
    }
    void setRange(int xs, int xe,int ys, int ye)
    {
        x_start = xs;
        x_end = xe;
        y_start = ys;
        y_end = ye;
    };
signals:
    void signalSetLog(int,QString);
private:
    QStringList fileList;
    int x_start,x_end,y_start,y_end;
};

#endif // IMGCUTTHREAD_H
