#include "filereader.h"
#include <QFileInfo>
FileReader::FileReader(QObject *parent)
    : QThread(parent)
{
}

/**
  * @brief 文件读取方法
  * @param 文件路径
  * @param 全部读取
  * @param 读取范围
  * @return
  */
bool FileReader::Reader(const char *filename, readType type)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == nullptr)
    {
        return false;
    }

    //    TmpFileUtils::TmpFileInfo =
    TmpFileUtils::TmpFileInfo info = TmpFileUtils::ReadHeader(fp);
    if (info.type != TmpFileUtils::unknow)
    {
        QVector<float> lineData;
        this->readHeaderInfoOver(true, info);
        if (info.column > 0 && info.row > 0)
        {
            float value = 0;
            size_t doubleSize = sizeof(float);
            lineData.clear();
            if (type == ROW)
            {
                for (int i = 0; i < info.row; i++)
                {
                    lineData.clear();
                    for (int l = 0; l < info.column; l++)
                    {
                        fread(&value, 1, doubleSize, fp);
                        lineData.push_back(value);
                    }
                    readOnceOver(type, i, lineData);
                }
            }
            else
            {

                long lineSize = (static_cast<long>(info.column) * static_cast<long>(doubleSize));
                //按列读取
                for (int i = 0; i < info.column; i++)
                {
                    //当前读取的起始位置点
                    lineData.clear();
                    //                    long fpStartIdx = ftell(fp);
                    for (int l = 0; l < info.row; l++)
                    {
                        //                        fseek(fp, fpStartIdx + lineSize * l, SEEK_SET);
                        fread(&value, 1, doubleSize, fp);
                        fseek(fp, lineSize, SEEK_CUR);
                        lineData.push_back(value);
                    }
                    readOnceOver(type, i, lineData);
                }
            }
            allOver(info.column, info.row);
            fclose(fp);
            fp = nullptr;
            return true;
        }
    }
    else
    {
        this->readHeaderInfoOver(false, info);
        emit over(false, u8"文件格式不正确");
    }
    return true;
}

void FileReader::run()
{
}

void FileReader::readOnceOver(readType, int, QVector<float>)
{
}

void FileReader::readHeaderInfoOver(bool, TmpFileUtils::TmpFileInfo)
{
}

void FileReader::allOver(int, int)
{
}
