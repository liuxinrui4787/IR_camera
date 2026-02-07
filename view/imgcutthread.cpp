#include "imageformat.h"
#include "imgcutthread.h"
#include <QDebug>
#include <QDir>
#include <QSize>

using namespace cv;
ImgCutThread::ImgCutThread(QObject *parent)
    : QThread(parent)
{

}

void ImgCutThread::run()
{
    for(QString path:fileList)
    {
        signalSetLog(2,u8"参考图像裁剪路径："+path);
        Mat tempMat;
        tempMat = readFileToMat((path).toStdString());
        tempMat = tempMat(Range(y_start,y_end), Range(x_start,x_end));

        path.chop(4);//
        path += "_cut.csv";//
        imageFormat::writeCSV((path).toStdString(),tempMat);
    }
}
QStringList ImgCutThread::readPath(QString dirpath)
{
    //设置要遍历的目录
    QDir dir(dirpath);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.csv";
//<<<<<<< Updated upstream
//    nameFilters /*<< "*.txt"*/ << "*.csv";

    //将过滤后的文件名称存入到files列表中
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable);
    //数组按int值大小排列
    std::sort(files.begin(), files.end(), [](const QString& s1, const QString& s2)
    {
        QString tmp_1, tmp_2;
        for (int j = 0; j < s1.length(); j++)
        {
            if (s1[j] >= '0' && s1[j] <= '9')
            {
                tmp_1.append(s1[j]);
            }
        }
        for (int j = 0; j < s2.length(); j++)
        {
            if (s2[j] >= '0' && s2[j] <= '9')
            {
                tmp_2.append(s2[j]);
            }
        }
        return tmp_1.toInt() < tmp_2.toInt();
    });
    return files;
}
cv::Mat ImgCutThread::readFileToMat(std::string fileName)
{
    QFile readfile(QString::fromStdString(fileName));
    QString strall;
    if (readfile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&readfile);
        strall = stream.readAll();
    }
    readfile.close();

    QStringList list_1 = strall.split("\r\n");
    if(list_1.size() < 2)
    {
        list_1 = strall.split("\n");
    }
    QStringList list_2 = list_1.at(0).split(",");
    if(list_2.size() < 2)
    {
        list_2 = list_1.at(0).split(";");
    }
    list_1.removeAll("");
    list_2.removeAll("");
    int rows = list_1.length();
    int cols = list_2.length();
    QSize mWH(cols, rows);


    FILE *fp = fopen(fileName.c_str(), "rb");
    if (fp == nullptr)
    {
        qDebug() << " error" << errno;
        cv::Mat m;
        return m;
    }
    bool isEnd = false;
    int height = mWH.height();
    int width = mWH.width();
    cv::Mat m(height, width, CV_32FC1);

    int count = 0;
    int idx = 0; //用于标记在tmpbuf上的位置
    width = 0;
    height = 0;
    char buf[4001]; //读取数据
    char tmpBuf[20]; //上次数据
    while (!isEnd)
    {
        memset(buf, 0, sizeof(char) * (4001));
        size_t pos = fread(buf, sizeof(char), 4000, fp); // fread返回的是读取的长度，遇到结尾或者错误返回0
        buf[pos] = '\0';
        if (pos == 0)
        {
            isEnd = true;
            break;
        }
        for (size_t i = 0; i < pos; i++)
        {
            tmpBuf[idx++] = buf[i];
            if (buf[i] == ',' || buf[i] == ';' || buf[i] == '\n')
            {

                tmpBuf[idx - 1] = 0;
                idx = 0;
                float p = atof(tmpBuf);
                if(height > mWH.height()-1)
                {
                    isEnd = true;
                    break;
                }
                m.at<float>(height, width) = p;
                width++;
                if (buf[i] == '\n') {
                    width = 0;
                    height++;
                }
                count++;
            }
        }
    }
    fclose(fp);
    fp = nullptr;
    return m;
}
