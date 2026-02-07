#ifndef FILEREADERTHREAD_H
#define FILEREADERTHREAD_H

#include <QObject>
#include <QThread>

class FileReaderThread : public QThread
{
    Q_OBJECT
    public:
    explicit FileReaderThread(QObject *parent = Q_NULLPTR);

    signals:
void findPoint(int p, int pIdx, int fileIdx);
void readOver(int errorCode, int fileIdx);

    public:
virtual void run();
void pushFileInfo(QString path, QStringList files, int start, int len);
void setPoinstArr(QVector<QVector<int>> *p, int readCount = 0)
{
m_pointArr = p;
m_readCount = readCount;
if (readCount <= 0)
{
 m_readCount = INT_MAX;
}
}

void pushFileInfo(QStringList files, int start, int len);
private:
void readCSVFile(const char *fileName, int fileIdx);
QVector<QVector<int>> *m_pointArr;
int m_readCount; //单次任务读取的数量
int m_startFileIdx;
QStringList m_filePath;
QList<long> m_filePathPos; //对应每个文件读取结束的位置
char buf[4001]; //读取数据
char tmpBuf[10]; //上次数据
};
#endif // FILEREADERTHREAD_H
