#ifndef RANDOMDELETE_H
#define RANDOMDELETE_H

#include "FileReader_Thread.h"
#include "imageformat.h"
#include "imgFeatureTool.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/types_c.h>
#include <QWidget>

using namespace cv;
namespace Ui {
class randomDelete;
}

class randomDelete : public QWidget
{
    Q_OBJECT

public:
    explicit randomDelete(QWidget *parent = nullptr);
    ~randomDelete();

    bool checkDirExists(const QString &filePath);
    void SetDirPath(const QString &file);
    void readDirPath();
    void fileOpenInit(QStringList &pathList);
signals:
    void signalRDImg(QPixmap);
    void signalSetLog(int,QString);
private slots:
    void on_btnOpen_clicked();

    void on_viewButton_clicked();

    void on_startButton_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::randomDelete *ui;
    QString fileDir;
    QStringList tempFiles;
    int x=0,y=0;
    Mat temp;
    Mat initial;
    QList<cv::Point> randomList;
    cv::Point makeRandomPoint(vector<vector<Point> > &contour);
    bool isRepeatPoint(Mat &fileMat, vector<vector<Point> > &contour);
    int deletetimes = 10;
};

#endif // RANDOMDELETE_H
