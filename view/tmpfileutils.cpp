<<<<<<< HEAD
#include "tmpfileutils.h"

bool TmpFileUtils::CreateFileHeader(FILE *fp, fileType type, bool isDouble, int column, int row, QSize xRange, QSize yRange)
{
    if (fp == nullptr)
        return false;
    fseek(fp, 0, SEEK_SET);
    int8_t tmp = type;

    char buffer[10] = "tmp";
    fwrite(buffer, 1, 3, fp);
    fwrite(&tmp, 1, sizeof(int8_t), fp);

    //    fseek(fp, 4, SEEK_CUR);

    //当前头文件长度为20位，保存在columen的底8位内
    int headerLen = 3 + sizeof(int8_t) + 2 * sizeof(int) + 4 * sizeof(uint16_t);
    column = (column << 8) + headerLen;

    fwrite(&column, 1, sizeof(int), fp);

    row = (row << 2) + (isDouble == true ? 1 : 0);

    fwrite(&row, 1, sizeof(int), fp);

    //    fseek(fp, 2 * sizeof(int), SEEK_CUR);

    uint16_t xStart = static_cast<uint16_t>(xRange.width());
    uint16_t xSize = static_cast<uint16_t>(xRange.height());
    uint16_t yStart = static_cast<uint16_t>(yRange.width());
    uint16_t ySize = static_cast<uint16_t>(yRange.height());
    fwrite(&xStart, 1, sizeof(uint16_t), fp);
    fwrite(&xSize, 1, sizeof(uint16_t), fp);

    //    fseek(fp, 2 * sizeof(uint16_t), SEEK_CUR);
    fwrite(&yStart, 1, sizeof(uint16_t), fp);
    fwrite(&ySize, 1, sizeof(uint16_t), fp);
    return true;
}

bool TmpFileUtils::ChangeFileHeader(QString filePath, fileType type, bool isDouble, int column, int row, QSize xRange, QSize yRange)
{
    FILE *fp = fopen(filePath.toStdString().c_str(), "rb+");
    if (fp == nullptr)
        return false;
    CreateFileHeader(fp, type, isDouble, column, row, xRange, yRange);
    fclose(fp);
    fp = nullptr;
    return true;
}

void TmpFileUtils::SkipFileHeader(FILE *fp)
{
    if (fp == nullptr)
        return;
    fseek(fp, 3 + sizeof(int8_t) + 2 * sizeof(int) + 4 * sizeof(uint16_t), SEEK_SET);
}

TmpFileUtils::TmpFileInfo TmpFileUtils::ReadFileInfo(QString file)
{
    FILE *fp = fopen(file.toStdString().c_str(), "rb");
    TmpFileUtils::TmpFileInfo info = TmpFileUtils::ReadHeader(fp);
    if (fp != nullptr)
    {
        fclose(fp);
        fp = nullptr;
    }
    return info;
}

TmpFileUtils::TmpFileInfo TmpFileUtils::ReadHeader(FILE *fp)
{
    TmpFileUtils::TmpFileInfo info;
    info.type = fileType::unknow;
    if (fp == nullptr)
        return info;
    fseek(fp, 0, SEEK_SET);
    char tmp[3];
    fread(tmp, 1, 3, fp);
    if (tmp[0] == 't' && tmp[1] == 'm' && tmp[2] == 'p')
    {
        int8_t tmp;
        fread(&tmp, 1, sizeof(int8_t), fp);
        info.type = static_cast<fileType>(tmp);
        fread(&info.column, 1, sizeof(int), fp);
        //
        int headerlen = info.column & 0xff;
        info.column = info.column >> 8;

        if (headerlen == (3 + sizeof(int8_t) + 2 * sizeof(int) + 4 * sizeof(uint16_t)))
        {
            //20位的读取内容
            fread(&info.row, 1, sizeof(int), fp);
            info.isDouble = (info.row & 0x01) == 0x01 ? true : false;
            info.row = info.row >> 2;
            uint16_t tmpint16 = 0;
            fread(&tmpint16, 1, sizeof(uint16_t), fp);
            info.xRange.setWidth(tmpint16);
            fread(&tmpint16, 1, sizeof(uint16_t), fp);
            info.xRange.setHeight(tmpint16);
            fread(&tmpint16, 1, sizeof(uint16_t), fp);
            info.yRange.setWidth(tmpint16);
            fread(&tmpint16, 1, sizeof(uint16_t), fp);
            info.yRange.setHeight(tmpint16);
        }
    }
    return info;
}
=======
#include "tmpfileutils.h"

bool TmpFileUtils::CreateFileHeader(FILE *fp, fileType type, bool isDouble, int column, int row, QSize xRange, QSize yRange)
{
    if (fp == nullptr)
        return false;
    fseek(fp, 0, SEEK_SET);
    int8_t tmp = type;

    char buffer[10] = "tmp";
    fwrite(buffer, 1, 3, fp);
    fwrite(&tmp, 1, sizeof(int8_t), fp);

    //    fseek(fp, 4, SEEK_CUR);

    //当前头文件长度为20位，保存在columen的底8位内
    int headerLen = 3 + sizeof(int8_t) + 2 * sizeof(int) + 4 * sizeof(uint16_t);
    column = (column << 8) + headerLen;

    fwrite(&column, 1, sizeof(int), fp);

    row = (row << 2) + (isDouble == true ? 1 : 0);

    fwrite(&row, 1, sizeof(int), fp);

    //    fseek(fp, 2 * sizeof(int), SEEK_CUR);

    uint16_t xStart = static_cast<uint16_t>(xRange.width());
    uint16_t xSize = static_cast<uint16_t>(xRange.height());
    uint16_t yStart = static_cast<uint16_t>(yRange.width());
    uint16_t ySize = static_cast<uint16_t>(yRange.height());
    fwrite(&xStart, 1, sizeof(uint16_t), fp);
    fwrite(&xSize, 1, sizeof(uint16_t), fp);

    //    fseek(fp, 2 * sizeof(uint16_t), SEEK_CUR);
    fwrite(&yStart, 1, sizeof(uint16_t), fp);
    fwrite(&ySize, 1, sizeof(uint16_t), fp);
    return true;
}

bool TmpFileUtils::ChangeFileHeader(QString filePath, fileType type, bool isDouble, int column, int row, QSize xRange, QSize yRange)
{
    FILE *fp = fopen(filePath.toStdString().c_str(), "rb+");
    if (fp == nullptr)
        return false;
    CreateFileHeader(fp, type, isDouble, column, row, xRange, yRange);
    fclose(fp);
    fp = nullptr;
    return true;
}

void TmpFileUtils::SkipFileHeader(FILE *fp)
{
    if (fp == nullptr)
        return;
    fseek(fp, 3 + sizeof(int8_t) + 2 * sizeof(int) + 4 * sizeof(uint16_t), SEEK_SET);
}

TmpFileUtils::TmpFileInfo TmpFileUtils::ReadFileInfo(QString file)
{
    FILE *fp = fopen(file.toStdString().c_str(), "rb");
    TmpFileUtils::TmpFileInfo info = TmpFileUtils::ReadHeader(fp);
    if (fp != nullptr)
    {
        fclose(fp);
        fp = nullptr;
    }
    return info;
}

TmpFileUtils::TmpFileInfo TmpFileUtils::ReadHeader(FILE *fp)
{
    TmpFileUtils::TmpFileInfo info;
    info.type = fileType::unknow;
    if (fp == nullptr)
        return info;
    fseek(fp, 0, SEEK_SET);
    char tmp[3];
    fread(tmp, 1, 3, fp);
    if (tmp[0] == 't' && tmp[1] == 'm' && tmp[2] == 'p')
    {
        int8_t tmp;
        fread(&tmp, 1, sizeof(int8_t), fp);
        info.type = static_cast<fileType>(tmp);
        fread(&info.column, 1, sizeof(int), fp);
        //
        int headerlen = info.column & 0xff;
        info.column = info.column >> 8;

        if (headerlen == (3 + sizeof(int8_t) + 2 * sizeof(int) + 4 * sizeof(uint16_t)))
        {
            //20位的读取内容
            fread(&info.row, 1, sizeof(int), fp);
            info.isDouble = (info.row & 0x01) == 0x01 ? true : false;
            info.row = info.row >> 2;
            uint16_t tmpint16 = 0;
            fread(&tmpint16, 1, sizeof(uint16_t), fp);
            info.xRange.setWidth(tmpint16);
            fread(&tmpint16, 1, sizeof(uint16_t), fp);
            info.xRange.setHeight(tmpint16);
            fread(&tmpint16, 1, sizeof(uint16_t), fp);
            info.yRange.setWidth(tmpint16);
            fread(&tmpint16, 1, sizeof(uint16_t), fp);
            info.yRange.setHeight(tmpint16);
        }
    }
    return info;
}
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
