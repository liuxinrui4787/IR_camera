#ifndef CSVFILEINFO_H
#define CSVFILEINFO_H
#include <QString>
/******************************************************************************
 * Copyright(C),2020-2021,National University of Defense Technology .
 * All right reserved. See COPYRIGHT for detailed Information.
 * FileName:   csvfileinfo.h
 * Author:     guohuan<guohuan@qq.com>
 * Version:    1.0.0
 * Date:       2021/12/14
 * Description: //获取CSV与临时文件的信息
 *****************************************************************************/

struct CSVInfo
{
    bool isExists; //是否存在
    int maxX; //最大X范围
    int maxY; //最大Y范围
    int count; //总点数量
    qint64 fileSize;
    QString suffix;
};

class CSVFileInfo
{
  public:
    CSVFileInfo();
    virtual ~CSVFileInfo();
    static CSVInfo GetInfo(const char *);
};

#endif // CSVFILEINFO_H
