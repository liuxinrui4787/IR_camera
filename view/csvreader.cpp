#include "csvreader.h"
#include <QCollator>
#include <QDir>
#include <QFile>
#include <QPoint>
#include <qdebug.h>
#include <string>

#define MOVESTEP 10

CSVReader::CSVReader(QObject *parent)
    : QObject(parent)
{
}

void CSVReader::start()
{
    m_pointArr.clear();
    m_pointVec.clear();
    for (int i = 0; i < m_pointCount; i++)
    {
        QVector<int> vPos(m_fileCount);
        m_pointVec.push_back(vPos);
    }

    for (int i = 0; i < m_filePath.length(); i++)
    {
        readCSVFile((m_dirPath + m_filePath.at(i)).toStdString().c_str(), i);
    }
}
QPoint CSVReader::setDirPath(const char *fileDirPath)
{
    m_pointArr.clear();
    m_fileCount = 0;
    m_pointCount = 0;
    QDir m_fdir = QDir(fileDirPath);
    m_filePath = m_fdir.entryList();
    QCollator collator;
    collator.setNumericMode(true);
    m_filePath = m_filePath.filter(".csv");

    std::sort(m_filePath.begin(), m_filePath.end(),
              [&collator](const QString &str1, const QString &str2) {
                  return collator.compare(str1, str2) < 0;
              });
//    foreach(QFileInfo fileInfo,m_filePath){
//        qDebug()<<fileInfo.filePath();
////        qDebug()<<list.size();
//    }
//    if (m_filePath.at(0) == ".")
//    {
//        m_filePath.removeFirst();
//    }
//    if (m_filePath.at(0) == "..")
//    {
//        m_filePath.removeFirst();
//    }
    m_fileCount = m_filePath.size();
    m_dirPath = m_fdir.absolutePath();
    m_dirPath.append(QDir::separator());
    quickReadCSV();
    return QPoint(m_fileCount, m_pointCount);
}

void CSVReader::quickReadCSV()
{
    FILE *fp =
        fopen((m_dirPath + this->m_filePath.at(0)).toStdString().c_str(), "rb");
    if (fp == nullptr)
    {
        qDebug() << " error" << errno;
        return;
    }
    memset(buf, 0, sizeof(char) * 10);
    bool isEnd = false;
    int count = 0;
    int idx = 0; //用于标记在tmpbuf上的位置
    while (!isEnd)
    {
        memset(buf, 0, sizeof(char) * (11));
        size_t pos = fread(buf, sizeof(char), 10,
                           fp); // fread返回的是读取的长度，遇到结尾或者错误返回0
        buf[pos] = '\0';
        if (pos == 0)
        {
            isEnd = true;
            break;
        }
        for (int i = 0; i < pos; i++)
        {
            if (buf[i] == ',' || buf[i] == '\n')
            {
                //                QVector<double> vPos ( m_fileCount );
                //                m_pointArr.push_back ( vPos );
                m_pointCount++;
            }
        }
    }
    fclose(fp);
    fp = nullptr;
}

void CSVReader::readCSVFile(const char *filepath, int fileIdx)
{
    FILE *fp = fopen(filepath, "rb");
    if (fp == nullptr)
    {
        qDebug() << " error" << errno;
        emit(readCSVOver(errno, fileIdx));
        return;
    }
    memset(buf, 0, sizeof(char) * 10);
    bool isEnd = false;
    int count = 0;
    int idx = 0; //用于标记在tmpbuf上的位置
    while (!isEnd)
    {
        memset(buf, 0, sizeof(char) * (11));
        size_t pos = fread(buf, sizeof(char), 10,
                           fp); // fread返回的是读取的长度，遇到结尾或者错误返回0
        buf[pos] = '\0';
        if (pos == 0)
        {
            isEnd = true;
            break;
        }
        for (int i = 0; i < pos; i++)
        {
            tmpBuf[idx++] = buf[i];
            if (buf[i] == ',' || buf[i] == '\n')
            {
                tmpBuf[idx - 1] = 0;
                idx = 0;
                int p = atoi(tmpBuf);//将字符数组转为整数
                m_pointVec[count][fileIdx] = p;
                count++;
            }
        }
    }
    fclose(fp);
    fp = nullptr;
    emit(readCSVOver(0, fileIdx));
    return;
}
