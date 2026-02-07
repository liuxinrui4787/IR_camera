#include "csvfileinfo.h"
#include <QElapsedTimer>
#include <QFileInfo>
#include <QObject>
#include <QTextCodec>
#include <QtDebug>
#include "tmpfileutils.h"
CSVFileInfo::CSVFileInfo()
{
}
CSVFileInfo::~CSVFileInfo()
{
}
CSVInfo CSVFileInfo::GetInfo(const char *filename)
{
    CSVInfo info;
    info.maxX = 0;
    info.maxY = 0;
    info.count = 0;
    info.suffix = "";
    info.fileSize = 0;

    QFileInfo fileInfo(filename);
    FILE *fp = fopen(filename, "rb");
    if (fp == nullptr)
    {
        info.isExists = false;
        return info;
    }
    info.isExists = true;

    uint fileSize = 0;
    info.suffix = fileInfo.suffix();
    info.fileSize = fileInfo.size();
    if (info.suffix == QString::fromLocal8Bit("tmp"))
    {
        //读取前20的位置，直接查看大小
        int tmpNum = 0;
        fread(&tmpNum, 1, sizeof(int), fp);
        fread(&tmpNum, 1, sizeof(int), fp);
        info.maxX = tmpNum;
        fread(&tmpNum, 1, sizeof(int), fp);
        info.maxY = tmpNum;
        fclose(fp);
        fp = nullptr;
        return info;
    }

    char buf[4001]; //读取数据
    bool isEnd = false;
    while (!isEnd)
    {
        memset(buf, 0, sizeof(char) * (4001));
        size_t pos = fread(buf, sizeof(char), 4000,
                           fp); // fread返回的是读取的长度，遇到结尾或者错误返回0
        buf[pos] = '\0';
        fileSize += pos;
        if (pos == 0)
        {
            isEnd = true;
            break;
        }
        for (size_t i = 0; i < pos; i++)
        {
            if (buf[i] == ',')
            {
                if (info.maxY == 0)
                {
                    info.maxX++;
                }
                info.count++;
            }
            else if (buf[i] == '\n')
            {
                if (info.maxY == 0)
                {
                    info.maxX++;
                }
                info.maxY++;
            }
        }
    }
    fclose(fp);
    fp = nullptr;
    return info;
}
