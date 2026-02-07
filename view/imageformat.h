<<<<<<< HEAD
#ifndef IMAGEFORMAT_H
#define IMAGEFORMAT_H
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/line_descriptor/descriptor.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::line_descriptor;
using namespace std;

class imageFormat
{
public:
    static QImage MatToQImage(const cv::Mat &mat);
    static void mat2gray(Mat &m1);
    static Mat vector2mat(vector<vector<double> > vec);
    static void writeCSV(std::string filename, Mat m);
    static void matMinMaxNoise(Mat &m);
    static void outputCSV(std::string filename, string s);
};

#endif // IMAGEFORMAT_H
=======
#ifndef IMAGEFORMAT_H
#define IMAGEFORMAT_H
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/line_descriptor/descriptor.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::line_descriptor;
using namespace std;

class imageFormat
{
public:
    static QImage MatToQImage(const cv::Mat &mat);
    static void mat2gray(Mat &m1);
    static Mat vector2mat(vector<vector<double> > vec);
    static void writeCSV(std::string filename, Mat m);
    static void matMinMaxNoise(Mat &m);
    static void outputCSV(std::string filename, string s);
};

#endif // IMAGEFORMAT_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
