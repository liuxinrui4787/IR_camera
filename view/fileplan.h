#ifndef FILEPLAN_H
#define FILEPLAN_H

#include <QObject>
//异常处理
#include "timemeanfile.h"
#include "timemovefile.h"
//=======
#include <QThread>

class FilePlan : public QObject
{
    Q_OBJECT
  public:
    explicit FilePlan(QObject *parent = nullptr);
    ~FilePlan();
    bool init();
    //检测输入文件夹
    bool checkInputPath(QString);
  signals:
    void errorMessage(QString);
    void showProgress(int);
    void over();
    void filePlanFinished();
  public slots:
    void readTimeAvgFile(QString);
    void deleteWorkTHreads();
    void initWorkThreads(int);

    void timemove_progress(int);
    void timemove_over(bool, QString);
    void timemean_step(int);

    void Start(QString inputPath, QString savepath, double, bool isReadAll, QSize xRange, QSize yRange);

  private:
    QList<TimeMeanFile *> m_meanWorker;
    int m_progress = 0; //总进度
    int m_overNums = 0;
    int m_thread_words_nums = 0; //总工作的线程数
    QSize m_imgSize; //文件本身的大小
    QSize m_rangX; //X的读取范围
    QSize m_rangY; //Y的读取范围
    QString m_inputFileName; //输入原始文件的文件路径
    QStringList m_fileNames; //原始文件列表
    QVector<double> m_timesavg; //
    QString m_timeAvgFilePath;
    int x_start, y_start;
};

#endif // FILEPLAN_H
