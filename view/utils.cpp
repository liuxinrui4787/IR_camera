#include "utils.h"
#include "QDir"
#include <QtMath>
//const char *Utils::ShowFilePath(const QString path)
//{
//    if (path.indexOf(QDir::separator()) == -1)
//    {
//        return path.toStdString().c_str();
//    }
//    return QString("%1").arg(path).replace("/", QDir::separator()).toStdString().c_str();
//}

QString Utils::BytesToSize(qint64 bytes)
{
    int nRemainder = 0;
    double nKeyValue = 1024.0;
    qint64 nByteSize = bytes;
    int i = 0;

    while (nByteSize >= nKeyValue && i < 6)
    {
        nRemainder = static_cast<int>(nByteSize) % static_cast<int>(nKeyValue);
        nByteSize = static_cast<ulong>(nByteSize / nKeyValue); //nByteSize /= nKeyValue
        i++;
    }

    nByteSize = static_cast<ulong>(floor(nByteSize));
    QString result = QString::number(nByteSize);
    if (nRemainder > 0)
    {
        result += "." + QString::number(nRemainder).mid(0, 2);
    }
    switch (i)
    {
        case 0:
        {
            result += "B";
        }
        break;
        case 1:
        {
            result += "KB";
        }
        break;
        case 2:
        {
            result += "MB";
        }
        break;
        case 3:
        {
            result += "GB";
        }
        break;
        case 4:
        {
            result += "TB";
        }
        break;
        case 5:
        {
            result += "PB";
        }
        break;
        default:
            return "unknow";
    }
    return result;
}
/**
 * @brief 计算方差
 */
double Utils::Var(QVector<double> vList, double mean)
{
    double var = 0;
    for (int i = 0; i < vList.length(); i++)
    {
        var += (qPow((vList[i] - mean), 2));
    }
    return var / vList.length();
}
/**
 * @brief 标准差
 */
double Utils::StandardDeviation(double var)
{
    return sqrt(var);
}
/**
 * @brief 标准差
 */
double Utils::StandardDeviation(QVector<double> vList)
{
    return StandardDeviation(Var(vList));
}

/**
 * @brief 计算方差
 */
double Utils::Var(QVector<double> vList)
{
    double sum = Mean(vList);
    return Var(vList, sum);
}

/**
 * @brief 计算均值
 */
float Utils::Mean(QVector<float> vList)
{
    float sum = 0;
    for (int i = 0; i < vList.length(); i++)
    {
        sum += vList[i];
    }
    return sum / vList.length();
}

/**
 * @brief 计算均值
 */
double Utils::Mean(QVector<double> vList)
{
    double sum = 0;
    for (int i = 0; i < vList.length(); i++)
    {
        sum += vList[i];
    }
    return sum / vList.length();
}

/**
 * @brief Utils::fixCenter
 * @param v 全数据
 * @param s 起始位置，也是需要修改的位置
 * @param e 结束位置，当前位置往后再延长
 * @return
 */
double Utils::fixCenter(QVector<double> v, int s, int e)
{
    int sIdx = s > 5 ? (s - 5) : 0;
    int len = v.length();
    int eIdx = (e > (len - 6)) ? (len - 5) : (e + 1);

    double sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += v[sIdx + i];
        sum += v[eIdx + i];
    }
    return sum / 10;
}
