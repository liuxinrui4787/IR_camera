#include "fileplan.h"
#include <QCollator>
#include <QFileDialog>
#include <QDebug>

FilePlan::FilePlan(QObject *parent)
    : QObject(parent)
{
    init();
}

FilePlan::~FilePlan()
{
    deleteWorkTHreads();
}

void FilePlan::deleteWorkTHreads()
{
    for (int i = 0; i < this->m_meanWorker.length(); i++)
    {
        this->m_meanWorker.at(i)->quit();
        this->m_meanWorker.at(i)->wait();
        delete this->m_meanWorker.at(i);
    }
    this->m_meanWorker.clear();
}

void FilePlan::initWorkThreads(int count)
{
    deleteWorkTHreads();
    for (int i = 0; i < count; i++)
    {
        TimeMeanFile *reader = new TimeMeanFile();
        m_meanWorker.push_back(reader);

        connect(reader, SIGNAL(progress(int)), this, SLOT(timemove_progress(int)));
        connect(reader, SIGNAL(over(bool, QString)), this, SLOT(timemove_over(bool, QString)));
        connect(reader, SIGNAL(step(int)), this, SLOT(timemean_step(int)));
    }
}

void FilePlan::timemean_step(int)
{
    //    if (step == 0)
    //    {
    //        this->ui->ratio_status->setText("未开始");
    //    }
    //    else if (step == 1)
    //    {
    //        this->ui->ratio_status->setText("数据读取");
    //    }7/
    //    else if (step == 2)
    //    {

    //        //计算时间均值
    //        this->ui->ratio_status->setText("均值计算");
    //    }
    //    else if (step == 3)
    //    {

    //        //所有时间的与均值的差
    //        this->ui->ratio_status->setText("差值计算");
    //    }
}

void FilePlan::timemove_progress(int progress)
{
    if (progress > this->m_progress)
    {
        this->m_progress = progress;
    }
    emit showProgress(this->m_progress);
    //当前进度
    //    this->ui->s1_progress->setValue(this->m_progress);
}
void FilePlan::timemove_over(bool result, QString msg)
{
    this->m_overNums++;
    if (this->m_overNums >= this->m_thread_words_nums)
    {
        qDebug()<<81;
        //完成
//        emit over();
        emit filePlanFinished();
    }

    if (!result)
    {
        //FIXME  报错显示
        emit errorMessage(msg);
    }
}

bool FilePlan::init()
{
    //创建10个读取文件的线程
    initWorkThreads(10);
    return true;
}

bool FilePlan::checkInputPath(QString filePath)
{
    if (filePath == QString::fromLocal8Bit("") && this->m_inputFileName == QString::fromLocal8Bit(""))
    {
        errorMessage("当前文件夹不存在");
        return false;
    }
    QDir m_fdir = QDir(filePath);
    if (!m_fdir.exists())
    {
        errorMessage("当前文件夹不存在");
        return false;
    }

    this->m_fileNames.clear();
    QStringList tmpList = m_fdir.entryList();
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(tmpList.begin(), tmpList.end(),
              [&collator](const QString &str1, const QString &str2) {
                  return collator.compare(str1, str2) < 0;
              });
    int column = 0;
    int allRow = 0;

    //文件大小
    this->m_imgSize = QSize(0, 0);

    for (int i = 0; i < tmpList.length(); i++)
    {
        TmpFileUtils::TmpFileInfo info = TmpFileUtils::ReadFileInfo(m_fdir.absolutePath() + QDir::separator() + tmpList.at(i));
        if (info.type == TmpFileUtils::normal)
        {
            if (column == 0)
            {
                column = info.column;
                this->m_imgSize.setWidth(info.xRange.height() - info.xRange.width());
            }
            else if (column != info.column)
            {
                continue;
            }
            this->m_imgSize.setHeight(this->m_imgSize.height() + info.yRange.height());
            allRow += info.row;
            this->m_fileNames.push_back(m_fdir.absolutePath() + QDir::separator() + tmpList.at(i));
        }
        else if (info.type == TmpFileUtils::time_avg)
        {
            readTimeAvgFile(m_fdir.absolutePath() + QDir::separator() + tmpList.at(i));
        }
    }

    if (filePath != QString::fromLocal8Bit("") && this->m_inputFileName != filePath)
    {
        this->m_inputFileName = filePath;
    }

    return true;
}

/**
 * @brief 读取时间均值文件
 * @param fileName
 */
void FilePlan::readTimeAvgFile(QString fileName)
{
    if (this->m_timeAvgFilePath == fileName)
        return;
    this->m_timesavg.clear();
    this->m_timeAvgFilePath = fileName;
    FILE *timeFp = fopen(fileName.toStdString().c_str(), "rb");
    if (timeFp == nullptr)
        return;
    TmpFileUtils::TmpFileInfo info = TmpFileUtils::ReadHeader(timeFp);
    int maxCount = info.column * info.row;
    this->m_timesavg.resize(maxCount);
    double value = 0;
    for (int i = 0; i < maxCount; i++)
    {
        fread(&value, 1, sizeof(double), timeFp);
        this->m_timesavg[i] = value;
    }
    fclose(timeFp);
    timeFp = nullptr;
}

void FilePlan::Start(QString inputPath, QString savepath, double avg_value, bool isReadAll, QSize xRange, QSize yRange)
{
    if (!checkInputPath(inputPath))
    {
        //当前文件读取出错，不再继续
        return;
    }

    //保存设置

    QDir m_saveDir(savepath);
    if (!m_saveDir.exists())
    {
        m_saveDir.mkdir(savepath);
    }

    if (this->m_fileNames.length() > this->m_meanWorker.length())
    {
        this->initWorkThreads(this->m_fileNames.length());
    }
    this->m_thread_words_nums = this->m_fileNames.length();

    m_progress = 0;
    m_overNums = 0;

    for (int i = 0; i < this->m_fileNames.length(); i++)
    {
        this->m_meanWorker.at(i)->SetConfig(this->m_fileNames.at(i).toStdString().c_str(), savepath.toStdString().c_str(), this->m_imgSize, &this->m_timesavg, avg_value, isReadAll, xRange,yRange);
        this->m_meanWorker.at(i)->start();
    }
}

