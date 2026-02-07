
#include <qdebug.h>
#include "filereader.h"
#include "filereaderthread.h"
FileReaderThread::FileReaderThread(QObject *parent)
    : QThread(parent)
{
}

void FileReaderThread::run()
{


    for (int i = 0; i < this->m_filePath.length(); i++)
    {
        this->readCSVFile(m_filePath.at(i).toStdString().c_str(), i);
        msleep(10);
    }
}

void FileReaderThread::pushFileInfo(QString path, QStringList files, int start, int len)
{
    this->m_readCount = INT_MAX;
    this->m_filePath.clear();
    this->m_filePathPos.clear();
    this->m_startFileIdx = start;
    for (int i = 0; i < len; i++)
    {
        if (files.length() <= (start + i))
            break;
        this->m_filePath.push_back(path + files.at(start + i));
        this->m_filePathPos.push_back(0); //默认的位置是0
    }


}
void FileReaderThread::pushFileInfo(QStringList files,int start, int len)
{
    m_filePath = files;
    for (int i = 0; i < len; i++)
    {
        if (files.length() <= start + i)
            break;
        this->m_filePathPos.push_back(0); //默认的位置是0
    }
}

void FileReaderThread::readCSVFile(const char *filepath, int fileIdx)
{
    FILE *fp = fopen(filepath, "rb");
    if (fp == nullptr)
    {
        qDebug() << " error" << errno;
        emit readOver(errno, fileIdx + this->m_startFileIdx);
        return;
    }
    long seek = this->m_filePathPos.at(fileIdx);
    fseek(fp, seek, SEEK_SET); //把文件内部 指针 移动到离文件开头seek字节处； SEEK_SET： 文件开头
//    memset(buf, 0, sizeof(char) * 10);
    bool isEnd = false;
    int count = 0;
    int idx = 0; //用于标记在tmpbuf上的位置
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
            if (buf[i] == ',' || buf[i] == '\n')
            {
                tmpBuf[idx - 1] = 0;
                idx = 0;
                int p = atoi(tmpBuf);
                (*m_pointArr)[count][m_startFileIdx + fileIdx] = p;
                count++;
                if (count >= m_readCount)
                {
                    this->m_filePathPos.replace(fileIdx, ftell(fp)-4000+i+1);
                    isEnd = true;
                    break;
                }
            }
        }
    }
    fclose(fp);
    fp = nullptr;
    emit readOver(0, fileIdx + this->m_startFileIdx);
    return;
}
