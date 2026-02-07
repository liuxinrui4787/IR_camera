#ifndef IMGFEATURETOOL_H
#define IMGFEATURETOOL_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QImage>

using namespace cv;
using namespace std;
class imgFeatureTool
{
public:
    static void img2ALCHE(Mat img, Mat &img_alche);
    static Mat threshold_inv(Mat m, double thresh);
    static Mat imgThreshold(Mat m, double thresh);
    static Mat imgCanny(Mat m, double thresh1, double thresh2);
    static void rotateIamge(cv::Mat &src, cv::Mat &dst, double angle);
    static QPoint match(vector<Rect> rect_lines, Rect reference_lines);
    static void removeIsland(Mat img,Mat &img_clear);
    static void thresholdIntegral(cv::Mat &inputMat, cv::Mat &outputMat, int type);
    static void filterNoiseContour(Mat &m);
    static void getContourRect(Mat m, cv::Vec4i &contourPoint);
    static void filterNinepoint(Mat &m1);
    static void autoHough(Mat localThreshold, cv::Vec4i conPoints, int &minlength, int &maxgap, int &minvote);
    static Mat imgFeatureThreshold(const cv::Mat mat, int thresholdType, int type);
    static int countMatArea(Mat m, int x, int y, int col, int row);
    static Mat Erosion(Mat src, int value);
    static Mat Dilation(Mat src, int value);
    static Mat Morphology(Mat src, int value);
    static Mat imageTranslation(cv::Mat &srcImage, int xOffset, int yOffset, int mmx, int mmy);

    static Point matDiffer1(vector<vector<Mat>> rectList);
    static Mat imageTranslation(Mat srcImage, int xOffset, int yOffset);
    static vector<vector<int> > getOffsetList(vector<vector<Point> > pointList, int &tempx, int &tempy);
    static vector<vector<Point>> getContour(Mat m);
    static Rect getContourRect(Mat m);
    static vector<vector<Point>> getRandomDeleteContour(Mat m);
    static double medianMat(cv::Mat Input, int nVals);
};

#endif // IMGFEATURETOOL_H
