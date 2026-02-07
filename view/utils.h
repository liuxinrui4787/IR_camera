#ifndef UTILS_H
#define UTILS_H

#include <QString>
class Utils
{
  public:
    //    static const char *ShowFilePath(const QString path);
    static QString BytesToSize(qint64 bytes);
    static double Mean(QVector<double>);
    static float Mean(QVector<float>);
    static double Var(QVector<double>);
    static double Var(QVector<double>, double mean);
    static double StandardDeviation(double var);
    static double StandardDeviation(QVector<double>);
    static double fixCenter(QVector<double>, int, int);
};

#endif // UTILS_H
