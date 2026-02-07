#ifndef FILEREADER_THREAD_H
#define FILEREADER_THREAD_H
#include <QThread>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/ml.hpp>
#include <opencv2/ml/ml.hpp>
#include <QDebug>
#include <QDir>
#include <QSize>

using namespace std;
using namespace cv;
class FileReader_Thread : public QThread
{
    Q_OBJECT
public:
    explicit FileReader_Thread(QObject *parent = nullptr);
    virtual void run();
    cv::Mat readFileToMat(string fileName);
    void setFileInfo(QString path, QStringList files, int start, int len);

    void setPoinstArr(QVector<QVector<double>> *p, int readCount = 0)
    {
        m_pointArr = p;
        m_readCount = readCount;
        if (readCount <= 0)
        {
            m_readCount = INT_MAX;
        }
    }
    void setMat_Rect(Rect rect)
    {
        mat_rect = rect;
    }
    void setRotateAngle(double ca)
    {
        correctAngle = ca;
    }
    void writeCSV(std::string filename, cv::Mat m)
    {
       std::ofstream myfile;
       myfile.open(filename.c_str());
       myfile<< cv::format(m, cv::Formatter::FMT_CSV) << std::endl;
       myfile.close();
    }
    void readfile(std::string fileName);
    QStringList readPath(QString dirpath);
    QSize getSize() const {
        return *mWH;
    }
    void setSize(QSize qs){
        mWH = new QSize(qs.width(),qs.height());
    }
    QStringList readPath(QString dirpath, QString filesType);
private:
    QStringList m_fileName;
    int m_fileStartId;
    cv::Mat result;
    string str;
    Rect mat_rect;
    QVector<QVector<double>> *m_pointArr;
    int m_readCount; //单次任务读取的数量
    QList<long> m_filePathPos; //对应每个文件读取结束的位置
    char buf[4001]; //读取数据
    char tmpBuf[20]; //上次数据
    double correctAngle;
    QSize* mWH = new QSize();

    void SplitString(const string& s, vector<string>& v, const string& c);

};

#endif // FILEREADER_THREAD_H
