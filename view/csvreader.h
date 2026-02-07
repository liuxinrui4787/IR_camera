#ifndef CSVREADER_H
#define CSVREADER_H

#include <QList>
#include <QObject>
#include <QThread>
#include <QVector>

class CSVReader : public QObject
{
    Q_OBJECT
  public:
    explicit CSVReader(QObject *parent = nullptr);

  signals:
    void readCSVOver(int errorCode, int fileIdx);

  public:
    QPoint setDirPath(const char *fileDirPath);
    void start();

  public:
    QVector<QVector<int>> getPointMap() const
    {
        return m_pointVec;
    }
    QStringList getFilePath() const
    {
        return m_filePath;
    }
    QString getDirPath() const
    {
        return m_dirPath;
    }

  private:
    void quickReadCSV();
    void readCSVFile(const char *fileName, int fileIdx);
    QList<QVector<int>> m_pointArr;
    QVector<QVector<int>> m_pointVec;
    int m_fileCount = 0;
    int m_pointCount = 0; //
    int m_startFileIdx;
    QStringList m_filePath;
    QString m_dirPath;
    char buf[11]; //读取数据
    char tmpBuf[10]; //上次数据
};

#endif // CSVREADER_H
