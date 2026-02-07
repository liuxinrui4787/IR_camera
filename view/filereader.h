#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QSize>
#include <QThread>
#include "tmpfileutils.h"
/******************************************************************************
 * Copyright(C),2020-2021,National University of Defense Technology .
 * All right reserved. See COPYRIGHT for detailed Information.
 * FileName:   filereader.h
 * Author:     guohuan<guohuan@qq.com>
 * Version:    1.0.0
 * Date:       2021/12/14
 * Description: //文件基础读取类

 *****************************************************************************/

class FileReader : public QThread
{
  public:
    enum readType
    {
        ROW, //按行读
        COLUMN //竖读
    };
    Q_OBJECT
  public:
    explicit FileReader(QObject *parent = nullptr);
    bool Reader(const char *filePath, readType type = ROW);

  protected:
    virtual void run();
    virtual void readOnceOver(readType, int, QVector<float>);
    virtual void allOver(int column, int row);
    virtual void readHeaderInfoOver(bool, TmpFileUtils::TmpFileInfo);
  signals:
    void progress(int);
    void over(bool resccess, QString errmessage);
};

#endif // FILEREADER_H
