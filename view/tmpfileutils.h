#ifndef TMPFILEUTILS_H
#define TMPFILEUTILS_H
#include <QObject>
#include <QSize>
/******************************************************************************
 * Copyright(C),2020-2021,National University of Defense Technology .
 * All right reserved. See COPYRIGHT for detailed Information.
 * FileName:   tmpfileutils.h
 * Author:     guohuan<guohuan@qq.com>
 * Version:    1.0.0
 * Date:       2021/12/15
 * Description: //临时文件帮助类
 *****************************************************************************/

class TmpFileUtils
{

  public:
    enum fileType
    {
        unknow = 0,
        normal = 1, //原始文件
        time_move = 2, //时间偏移文件
        time_avg = 3, //时间均值
        ratio_offset = 4, //占比偏移文件
    };

    struct TmpFileInfo
    {
        fileType type = unknow;
        int column = 0;
        int row = 0;
        QSize xRange = QSize(0, 0);
        QSize yRange = QSize(0, 0);
        bool isDouble = false;
    };

  public:
    //生成tmp类型的文件头
    static bool CreateFileHeader(FILE *fp, fileType type, bool isDouble, int column, int row, QSize xRange, QSize yRange);
    static void SkipFileHeader(FILE *fp);
    static TmpFileInfo ReadHeader(FILE *fp);
    static TmpFileInfo ReadFileInfo(QString file);
    static bool ChangeFileHeader(QString filePath, fileType type, bool isDouble, int column, int row, QSize xRange, QSize yRange);
};

#endif // TMPFILEUTILS_H
