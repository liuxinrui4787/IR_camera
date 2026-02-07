<<<<<<< HEAD
#include "FileReader_Thread.h"
#include "linefinder.h"
#include "imgFeatureTool.h"

FileReader_Thread::FileReader_Thread(QObject *parent) : QThread(parent)
{
//    QDir *folder = new QDir;
//    //判断创建文件夹是否存在
//    bool exist = folder->exists("D:/RotateCorrection");
//    if(exist)
//    {

//    }
//    else //如果不存在，创建文件夹
//    {
//        //创建文件夹
//        bool ok = folder->mkpath("D:/RotateCorrection");
//    }
}
void FileReader_Thread::run()
{

}

void FileReader_Thread::setFileInfo(QString path, QStringList files, int start, int len)
{
    m_fileName.clear();
    m_fileStartId = start;
    for (int i = 0; i < len; i++)
    {
        if (files.length() <= start + i)
        {
            break;
        }
        m_fileName.push_back(path + files.at(start + i));
    }
}


void FileReader_Thread::readfile(std::string fileName)
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
    mWH = new QSize(cols, rows);
}

QStringList FileReader_Thread::readPath(QString dirpath)
{
    //设置要遍历的目录
    QDir dir(dirpath);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.csv";
//<<<<<<< Updated upstream
//    nameFilters /*<< "*.txt"*/ << "*.csv";
//=======
//    nameFilters << "*.csv";
//>>>>>>> Stashed changes0
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
QStringList FileReader_Thread::readPath(QString dirpath,QString filesType)
{
    //设置要遍历的目录
    QDir dir(dirpath);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << filesType;
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
cv::Mat FileReader_Thread::readFileToMat(std::string fileName)
{
    FILE *fp = fopen(fileName.c_str(), "rb");
    if (fp == nullptr)
    {
        qDebug() << " error" << errno;
        cv::Mat m;
        return m;
    }
    bool isEnd = false;
    int height = mWH->height();
    int width = mWH->width();
    cv::Mat m(height, width, CV_32FC1);

    int count = 0;
    int idx = 0; //用于标记在tmpbuf上的位置
    width = 0;
    height = 0;
    int max = 0;
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
                if(height > mWH->height()-1)
                {
                    isEnd = true;
                    break;
                }
                m.at<float>(height, width) = p;
//                qDebug()<<"before:"<<p;
//                qDebug()<<"after:"<<m.at<float>(height, width);
                //               (*m_pointArr)[height][width] = p;
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

=======
#include "FileReader_Thread.h"
#include "linefinder.h"
#include "imgFeatureTool.h"

FileReader_Thread::FileReader_Thread(QObject *parent) : QThread(parent)
{
//    QDir *folder = new QDir;
//    //判断创建文件夹是否存在
//    bool exist = folder->exists("D:/RotateCorrection");
//    if(exist)
//    {

//    }
//    else //如果不存在，创建文件夹
//    {
//        //创建文件夹
//        bool ok = folder->mkpath("D:/RotateCorrection");
//    }
}
void FileReader_Thread::run()
{

}

void FileReader_Thread::setFileInfo(QString path, QStringList files, int start, int len)
{
    m_fileName.clear();
    m_fileStartId = start;
    for (int i = 0; i < len; i++)
    {
        if (files.length() <= start + i)
        {
            break;
        }
        m_fileName.push_back(path + files.at(start + i));
    }
}


void FileReader_Thread::readfile(std::string fileName)
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
    mWH = new QSize(cols, rows);
}

QStringList FileReader_Thread::readPath(QString dirpath)
{
    //设置要遍历的目录
    QDir dir(dirpath);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << "*.csv";
//<<<<<<< Updated upstream
//    nameFilters /*<< "*.txt"*/ << "*.csv";
//=======
//    nameFilters << "*.csv";
//>>>>>>> Stashed changes0
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
QStringList FileReader_Thread::readPath(QString dirpath,QString filesType)
{
    //设置要遍历的目录
    QDir dir(dirpath);
    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    nameFilters << filesType;
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
cv::Mat FileReader_Thread::readFileToMat(std::string fileName)
{
    FILE *fp = fopen(fileName.c_str(), "rb");
    if (fp == nullptr)
    {
        qDebug() << " error" << errno;
        cv::Mat m;
        return m;
    }
    bool isEnd = false;
    int height = mWH->height();
    int width = mWH->width();
    cv::Mat m(height, width, CV_32FC1);

    int count = 0;
    int idx = 0; //用于标记在tmpbuf上的位置
    width = 0;
    height = 0;
    int max = 0;
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
                if(height > mWH->height()-1)
                {
                    isEnd = true;
                    break;
                }
                m.at<float>(height, width) = p;
//                qDebug()<<"before:"<<p;
//                qDebug()<<"after:"<<m.at<float>(height, width);
                //               (*m_pointArr)[height][width] = p;
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

>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
