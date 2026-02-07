#include "imgFeatureTool.h"
#include "imageformat.h"
#include "linefinder.h"
#include <QDebug>

void imgFeatureTool::img2ALCHE(Mat img, Mat& img_alche)
{
    Mat gray;
    if(!(img.type() == CV_8UC1))
    {
        cvtColor(img,gray,COLOR_BGR2GRAY);
    }else
    {
        gray = img.clone();
    }

//    medianBlur(gray,gray,5);
    std::vector<cv::Mat> channels(3);
    cv::split(gray, channels);//图像通道分离

    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    // 直方图的柱子高度大于计算后的ClipLimit的部分被裁剪掉，然后将其平均分配给整张直方图
    // 从而提升整个图像
    clahe->setClipLimit(4.);	// (int)(4.*(8*8)/256)
    //clahe->setTilesGridSize(Size(8, 8)); // 将图像分为8*8块
    cv::Mat dst;
    clahe->apply(channels[0], dst);
    dst.copyTo(channels[0]);
    cv::merge(channels, img_alche);
}
Mat imgFeatureTool::imgThreshold(Mat m,double thresh)
{
    Mat img;
    medianBlur(m,img,3);
    threshold(img,img,thresh,255,THRESH_BINARY);
    return img;
}
Mat  imgFeatureTool::threshold_inv(Mat m,double thresh)
{
    //---------------------反二值化----------------------
    Mat inv;
    medianBlur(m,inv,3);
    threshold(inv,inv, thresh,255,THRESH_BINARY_INV);
    return inv;
}
Mat imgFeatureTool::imgCanny(Mat m,double thresh1,double thresh2)
{
    Mat img;
    Canny(m,img,thresh1,thresh2);
    return img;
}
void imgFeatureTool::rotateIamge(cv::Mat& src, cv::Mat& dst, double angle)
{
    cv::Point2f center(src.cols / 2, src.rows / 2);
    cv::Mat rot = getRotationMatrix2D(center, angle, 1);
    cv::Rect box = RotatedRect(center, src.size(), angle).boundingRect(); // get circumscribed rectangle
    cv::warpAffine(src, dst, rot, box.size());
}

//以中心点为匹配目标，返回偏移值
QPoint imgFeatureTool::match(vector<Rect> rect_lines, Rect reference_lines)
{
    vector<Rect> rl;
    rl.push_back(rect_lines.at(0));
    int reference_lines_y = ceil(reference_lines.height/2)+reference_lines.y;
    int reference_lines_x = ceil(reference_lines.width/2)+reference_lines.x;
    int rl_y = ceil(rl[0].height/2)+rl[0].y;
    int rl_x = ceil(rl[0].width/2)+rl[0].x;
    int offset_x = reference_lines_x - rl_x;
    int offset_y = reference_lines_y - rl_y;
    return QPoint(offset_x,offset_y);
}


void imgFeatureTool::removeIsland(Mat img, Mat &img_clear)
{
    Mat m = img.clone();

    for(int i = 0; i < m.rows; i++)
    {
        for(int j = 0; j < m.cols; j++)
        {
            int sumtemp;
            int temp1;
            int temp2;
            int temp3;
            int temp4;
            int temp5;
            int temp6;
            int temp7;
            int temp8;
            int temp9;
            if(i == 0)
            {
                if(j == 0)
                {
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    temp8 = m.at<uchar>(i+1,j);
                    temp9 = m.at<uchar>(i+1,j+1);
                    sumtemp = temp6 + temp8 + temp9;
                }else if(j > 0 && j < m.cols - 1)
                {
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    temp7 = m.at<uchar>(i+1,j-1);
                    temp8 = m.at<uchar>(i+1,j);
                    temp9 = m.at<uchar>(i+1,j+1);
                    sumtemp = temp4 + temp6 + temp7 + temp8 + temp9;
                }else
                {
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp7 = m.at<uchar>(i+1,j-1);
                    temp8 = m.at<uchar>(i+1,j);
                    sumtemp = temp4 + temp7 + temp8;
                }
            }else if(i < m.rows - 1)
            {
                if(j == 0)
                {
                    temp2 = m.at<uchar>(i-1,j);
                    temp3 = m.at<uchar>(i-1,j+1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    temp8 = m.at<uchar>(i+1,j);
                    temp9 = m.at<uchar>(i+1,j+1);
                    sumtemp = temp2 + temp6 + temp3 + temp8 + temp9;
                }else if(j > 0 && j < m.cols - 1)
                {
                    temp1 = m.at<uchar>(i-1,j-1);
                    temp2 = m.at<uchar>(i-1,j);
                    temp3 = m.at<uchar>(i-1,j+1);
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    temp7 = m.at<uchar>(i+1,j-1);
                    temp8 = m.at<uchar>(i+1,j);
                    temp9 = m.at<uchar>(i+1,j+1);
                    sumtemp = temp1 + temp2 + temp3 + temp4 + temp6 + temp7 + temp8 + temp9;
                }else
                {
                    temp1 = m.at<uchar>(i-1,j-1);
                    temp2 = m.at<uchar>(i-1,j);
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp7 = m.at<uchar>(i+1,j-1);
                    temp8 = m.at<uchar>(i+1,j);
                    sumtemp = temp1 + temp2 + temp4 + temp7 + temp8;
                }
            }else
            {
                if(j == 0)
                {
                    temp2 = m.at<uchar>(i-1,j);
                    temp3 = m.at<uchar>(i-1,j+1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    sumtemp = temp2 + temp3 + temp6;
                }else if(j > 0 && j < m.cols - 1)
                {
                    temp1 = m.at<uchar>(i-1,j-1);
                    temp2 = m.at<uchar>(i-1,j);
                    temp3 = m.at<uchar>(i-1,j+1);
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    sumtemp = temp1 + temp2 + temp3 + temp4 + temp6;
                }else
                {
                    temp1 = m.at<uchar>(i-1,j-1);
                    temp2 = m.at<uchar>(i-1,j);
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    sumtemp = temp1 + temp2 + temp4;
                }
            }

            if(temp5 == 255 && sumtemp <= 255)
            {
                m.at<uchar>(i,j) = 0;
            }
        }
    }
    img_clear = m.clone();
}
//基于Wall算法自适应的图像二值化
void imgFeatureTool::thresholdIntegral(cv::Mat &inputMat, cv::Mat &outputMat, int type)
{
    // accept only char type matrices
    CV_Assert(!inputMat.empty());
    CV_Assert(inputMat.depth() == CV_8U);
    CV_Assert(inputMat.channels() == 1);
    CV_Assert(!outputMat.empty());
    CV_Assert(outputMat.depth() == CV_8U);
    CV_Assert(outputMat.channels() == 1);

    // rows -> height -> y
    int nRows = inputMat.rows;
    // cols -> width -> x
    int nCols = inputMat.cols;

    // create the integral image
    cv::Mat sumMat;
    cv::integral(inputMat, sumMat);

    CV_Assert(sumMat.depth() == CV_32S);
    CV_Assert(sizeof(int) == 4);

    int S = MAX(nRows, nCols)/8;
    double T = 0.15;

    // perform thresholding
    int s2 = S/2;
    int x1, y1, x2, y2, count, sum;

    // CV_Assert(sizeof(int) == 4);
    int *p_y1, *p_y2;
    uchar *p_inputMat, *p_outputMat;

    for( int i = 0; i < nRows; ++i)
    {
        y1 = i-s2;
        y2 = i+s2;

        if (y1 < 0){
            y1 = 0;
        }
        if (y2 >= nRows) {
            y2 = nRows-1;
        }

        p_y1 = sumMat.ptr<int>(y1);
        p_y2 = sumMat.ptr<int>(y2);
        p_inputMat = inputMat.ptr<uchar>(i);
        p_outputMat = outputMat.ptr<uchar>(i);

        for ( int j = 0; j < nCols; ++j)
        {
            // set the SxS region
            x1 = j-s2;
            x2 = j+s2;

            if (x1 < 0) {
                x1 = 0;
            }
            if (x2 >= nCols) {
                x2 = nCols-1;
            }

            count = (x2-x1)*(y2-y1);

            // I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
            sum = p_y2[x2] - p_y1[x2] - p_y2[x1] + p_y1[x1];
            if(type == THRESH_BINARY)
            {
                if ((int)(p_inputMat[j] * count) < (int)(sum*(1.0-T)))
                    p_outputMat[j] = 0;
                else
                    p_outputMat[j] = 255;
            }else if(type == THRESH_BINARY_INV)
            {
                if ((int)(p_inputMat[j] * count) < (int)(sum*(1.0-T)))
                    p_outputMat[j] = 255;
                else
                    p_outputMat[j] = 0;
            }
        }
    }
}
//过滤噪声的轮廓
void imgFeatureTool::filterNoiseContour(Mat &m)
{
    if(m.type() == CV_8UC3)
    {
        cvtColor(m,m,COLOR_BGR2GRAY);
    }
    Mat con;
    Canny(m,con,20,50);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(con, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE );
    Mat linePic = Mat::zeros(m.rows, m.cols, CV_8UC3);
    for (int index = 0; index < contours.size(); index++){
        drawContours(linePic, contours, index, Scalar(rand() & 255, rand() & 255, rand() & 255), 1, 8/*, hierarchy*/);
    }
    int w = 5;
    int h = 5;
    if(m.cols > 100)
    {
        w = m.cols*0.05;
    }
    if(m.rows > 100)
    {
        h = m.rows*0.05;
    }
    for (int i = 0; i < contours.size(); ++i)
    {
        Rect rect = cv::boundingRect(cv::Mat(contours[i]));
//        qDebug()<<QRect(rect.x,rect.y,rect.width,rect.height);
        if(rect.width <= w && rect.height <= h)
        {
            for(int i = 0;i<rect.width;i++)
            {
                for(int j = 0;j<rect.height;j++)
                {
                    m.at<uchar>(rect.y+j,rect.x+i) = 0;
                }
            }
        }
    }
}

//计算轮廓的矩形面积大小
void imgFeatureTool::getContourRect(Mat m,Vec4i &contourPoint)
{
    Mat con;
    Canny(m,con,20,50);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(con, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE );
    Mat linePic = Mat::zeros(m.rows, m.cols, CV_8UC3);
    for (int index = 0; index < contours.size(); index++){
        drawContours(linePic, contours, index, Scalar(rand() & 255, rand() & 255, rand() & 255), 1, 8/*, hierarchy*/);
    }
//    imshow("1",linePic);
    int x1,y1,x2,y2;
    for (int i = 0; i < contours.size(); ++i)
    {
        Rect rect = cv::boundingRect(cv::Mat(contours[i]));
        if(i == 0)
        {
            x1 = rect.x;
            y1 = rect.y;
            x2 = rect.x+rect.width;
            y2 = rect.y+rect.height;
        }
        if(rect.x < x1)
        {
            x1 = rect.x;
        }
        if(rect.y < y1)
        {
            y1 = rect.y;
        }
        if(rect.x+rect.width > x2)
        {
            x2 = rect.x+rect.width;
        }
        if(rect.y+rect.height > y2)
        {
            y2 = rect.y+rect.height;
        }
    }
    Mat polyPic = Mat::zeros(m.rows, m.cols, CV_8UC3);
    rectangle(polyPic, Rect(x1,y1,x2-x1,y2-y1), Scalar(0,255,0), 1);
//    imshow("2",polyPic);
//    qDebug()<<QRect(x1,y1,x2-x1,y2-y1);
    contourPoint = Vec4i(x1,y1,x2,y2);
}
Rect imgFeatureTool::getContourRect(Mat m)
{
    Rect contourRect;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(m, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    Mat linePic = Mat::zeros(m.rows, m.cols, CV_8UC3);
    for (int index = 0; index < contours.size(); index++){
        drawContours(linePic, contours, index, Scalar(rand() & 255, rand() & 255, rand() & 255), 1, 8, hierarchy);
    }
    imshow("1",linePic);
    int x1,y1,x2,y2;
    for (int i = 0; i < contours.size(); ++i)
    {
        Rect rect = cv::boundingRect(cv::Mat(contours[i]));
        if(i == 0)
        {
            x1 = rect.x;
            y1 = rect.y;
            x2 = rect.x+rect.width;
            y2 = rect.y+rect.height;
        }
        if(rect.x < x1)
        {
            x1 = rect.x;
        }
        if(rect.y < y1)
        {
            y1 = rect.y;
        }
        if(rect.x+rect.width > x2)
        {
            x2 = rect.x+rect.width;
        }
        if(rect.y+rect.height > y2)
        {
            y2 = rect.y+rect.height;
        }
    }

    return contourRect = Rect(x1,y1,x2-x1,y2-y1);
}
vector<vector<Point>> imgFeatureTool::getRandomDeleteContour(Mat m)
{
    Rect contourRect;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(m, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    Mat linePic = Mat::zeros(m.rows, m.cols, CV_8UC3);
    for (int index = 0; index < contours.size(); index++){
        drawContours(linePic, contours, index, Scalar(rand() & 255, rand() & 255, rand() & 255), 1, 8, hierarchy);
    }
//    imshow("1",linePic);
    int index=-1;
    for(int i = 0; i < hierarchy.size(); i++)
    {
        if(hierarchy.at(i)[3] == 0 && hierarchy.at(i)[2] != -1)
        {
            index = i;
            break;
        }
    }
    Mat mat = m.clone();
    if(index < 0)
    {

    }else
    {
        //裁剪外层轮廓
        Rect rect = cv::boundingRect(cv::Mat(contours[index]));
        rect = Rect(rect.x+1,rect.y+1,rect.width-8,rect.height-8);

        for(int y = 0; y < m.rows; y++)
        {
            if(y <= rect.y || y >= rect.y+rect.height)
            {
                for(int x = 0; x < m.cols; x++)
                {
                    mat.at<uchar>(y, x) = 0;
                }
            }else
            {
                for(int x = 0; x < m.cols; x++)
                {
                    if(x <= rect.x || x >= rect.x+rect.width)
                    {
                        mat.at<uchar>(y, x) = 0;
                    }
                }
            }
        }

    }
    //重新检测轮廓
//    imshow("mat",mat);
    vector<vector<Point>> contours1;
    vector<Vec4i> hierarchy1;
    vector<vector<Point>> allcontours;
    vector<Vec4i> allhierarchy;
    findContours(mat, contours1, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE );
    findContours(mat, allcontours, allhierarchy, RETR_TREE, CHAIN_APPROX_NONE);

    //去除点线构成的干扰轮廓
    for(int i = 0; i < contours1.size(); i++)
    {
        if(contours1.at(i).size() < 3)
        {
            contours1.erase(contours1.begin()+i);
            allcontours.erase(allcontours.begin()+i);
            i--;
        }
    }
    Mat linePic1 = Mat::zeros(mat.rows, mat.cols, CV_8UC3);
    for (int index = 0; index < contours1.size(); index++){
        drawContours(linePic1, contours1, index, Scalar(rand() & 255, rand() & 255, rand() & 255), 1, 8);
    }
//    imshow("rect",linePic1);
    //去除轮廓拐点
    for(int i = 0; i < allcontours.size(); i++)
    {
        for(int l = 0; l < contours1.at(i).size(); l++)
        {
            for(int j = 0; j < allcontours.at(i).size(); j++)
            {
                if(contours1.at(i)[l] == allcontours.at(i)[j])
                {
                    allcontours.at(i).erase(allcontours.at(i).begin()+j);
                    break;
                }
            }
        }
    }
    //清除空集合
    for (int index = 0; index < allcontours.size(); index++)
    {
        if(allcontours.at(index).size() == 0)
        {
            allcontours.erase(allcontours.begin()+index);
            index--;
        }
    }

    return allcontours;
}
vector<vector<Point>> imgFeatureTool::getContour(Mat m)
{
    Mat con;
    Rect contourRect;
    Canny(m,con,20,50);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(con, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );

    return contours;
}
//九宫格去噪
void imgFeatureTool::filterNinepoint(Mat &m1)
{
    Mat m;
    if(m1.type() == CV_8UC3)
    {
        cvtColor(m1,m,COLOR_BGR2GRAY);
    }else
    {
        m = m1.clone();
    }

    for(int i = 0; i < m.rows; i++)
    {
        for(int j = 0; j < m.cols; j++)
        {
            int sumtemp;
            int temp1;
            int temp2;
            int temp3;
            int temp4;
            int temp5;
            int temp6;
            int temp7;
            int temp8;
            int temp9;
            if(i == 0)
            {
                if(j == 0)
                {
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    temp8 = m.at<uchar>(i+1,j);
                    temp9 = m.at<uchar>(i+1,j+1);
                    sumtemp = temp6 + temp8 + temp9;
                }else if(j > 0 && j < m.cols - 1)
                {
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    temp7 = m.at<uchar>(i+1,j-1);
                    temp8 = m.at<uchar>(i+1,j);
                    temp9 = m.at<uchar>(i+1,j+1);
                    sumtemp = temp4 + temp6 + temp7 + temp8 + temp9;
                }else
                {
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp7 = m.at<uchar>(i+1,j-1);
                    temp8 = m.at<uchar>(i+1,j);
                    sumtemp = temp4 + temp7 + temp8;
                }
            }else if(i < m.rows - 1)
            {
                if(j == 0)
                {
                    temp2 = m.at<uchar>(i-1,j);
                    temp3 = m.at<uchar>(i-1,j+1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    temp8 = m.at<uchar>(i+1,j);
                    temp9 = m.at<uchar>(i+1,j+1);
                    sumtemp = temp2 + temp6 + temp3 + temp8 + temp9;
                }else if(j > 0 && j < m.cols - 1)
                {
                    temp1 = m.at<uchar>(i-1,j-1);
                    temp2 = m.at<uchar>(i-1,j);
                    temp3 = m.at<uchar>(i-1,j+1);
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    temp7 = m.at<uchar>(i+1,j-1);
                    temp8 = m.at<uchar>(i+1,j);
                    temp9 = m.at<uchar>(i+1,j+1);
                    sumtemp = temp1 + temp2 + temp3 + temp4 + temp6 + temp7 + temp8 + temp9;
                }else
                {
                    temp1 = m.at<uchar>(i-1,j-1);
                    temp2 = m.at<uchar>(i-1,j);
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp7 = m.at<uchar>(i+1,j-1);
                    temp8 = m.at<uchar>(i+1,j);
                    sumtemp = temp1 + temp2 + temp4 + temp7 + temp8;
                }
            }else
            {
                if(j == 0)
                {
                    temp2 = m.at<uchar>(i-1,j);
                    temp3 = m.at<uchar>(i-1,j+1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    sumtemp = temp2 + temp3 + temp6;
                }else if(j > 0 && j < m.cols - 1)
                {
                    temp1 = m.at<uchar>(i-1,j-1);
                    temp2 = m.at<uchar>(i-1,j);
                    temp3 = m.at<uchar>(i-1,j+1);
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    temp6 = m.at<uchar>(i,j+1);
                    sumtemp = temp1 + temp2 + temp3 + temp4 + temp6;
                }else
                {
                    temp1 = m.at<uchar>(i-1,j-1);
                    temp2 = m.at<uchar>(i-1,j);
                    temp4 = m.at<uchar>(i,j-1);
                    temp5 = m.at<uchar>(i,j);
                    sumtemp = temp1 + temp2 + temp4;
                }
            }

            if(temp5 == 255 && sumtemp <= 510)
            {
                m.at<uchar>(i,j) = 0;
            }
        }
    }
    m1 = m.clone();
}

void imgFeatureTool::autoHough(Mat localThreshold,Vec4i conPoints,int& minlength,int& maxgap,int& minvote)
{

}
Mat imgFeatureTool::imgFeatureThreshold(const cv::Mat mat, int thresholdType, int type)
{
    Mat gray;
    Mat thresh;
    if(mat.type() == CV_8UC3)
    {
        cvtColor(mat,gray,COLOR_BGR2GRAY);
    }else
    {
        gray = mat.clone();
    }
    medianBlur(gray,gray,3);

    if(thresholdType == 1)
    {
        thresh = Mat::zeros(gray.rows, gray.cols, CV_8UC1);
        thresholdIntegral(gray,thresh,type);
    }else if(thresholdType == 2)
    {
        cv::threshold(gray,thresh,0,255,THRESH_TRIANGLE|type);
    }else if(thresholdType == 3)
    {
        cv::threshold(gray,thresh,0,255,THRESH_OTSU|type);
    }

    filterNinepoint(thresh);
    filterNoiseContour(thresh);
    return thresh;
}
int imgFeatureTool::countMatArea(Mat m, int x, int y, int col, int row)
{
    int tempArea = 0;
    for(int i = y; i < row; i++)
    {
        for(int j = x; j < col; j++)
        {
            if(m.at<uchar>(i,j) == 255)
            {
                tempArea++;
            }
        }
    }
    return tempArea;
}
Mat imgFeatureTool::Erosion(Mat src,int value)
{
    int erosion_size = value;
    Mat erosion_dst;
    Mat element = getStructuringElement( MORPH_RECT,
                         Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                         Point( erosion_size, erosion_size ) );
    erode( src, erosion_dst, element );
    return erosion_dst;
}
Mat imgFeatureTool::Dilation(Mat src,int value)
{
    int dilation_size = value;
    Mat dilation_dst;
    Mat element = getStructuringElement( MORPH_RECT,
                         Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                         Point( dilation_size, dilation_size ) );
    dilate( src, dilation_dst, element );
    return dilation_dst;
}
Mat imgFeatureTool::Morphology(Mat src,int value)
{
    int type = MORPH_CLOSE;
    type = MORPH_OPEN;
    Mat dst;
    Mat element = getStructuringElement( MORPH_RECT,
                                         Size( 2*value + 1, 2*value+1 ),
                                         Point( value, value ) );
    morphologyEx( src, dst, type, element );
    return dst;
}
//按偏移裁剪图像
Mat imgFeatureTool::imageTranslation(cv::Mat & srcImage, int xOffset,int yOffset,int mmx,int mmy){
    Mat resultmage(srcImage.rows - mmy*2, srcImage.cols - mmx*2, srcImage.type(), cv::Scalar::all(0));
    int rows = srcImage.rows;
    int cols = srcImage.cols;
    //遍历图像
    if(mmx == 0 && mmy == 0)
    {
        return srcImage;
    }
    for (int i = 0; i < rows; i++)
    {
        for(int j=0;j<cols;j++)
        {
            //映射变换
            int x = j - xOffset;
            int y = i - yOffset;
        //边界判断
        if (x >= 0 && y >= 0 && x < resultmage.cols && y < resultmage.rows)
            resultmage.at<float>(y,x) = srcImage.at<float>(i, j);
        }
    }
    return resultmage;
}


//两区域遍历点作差
Point imgFeatureTool::matDiffer1(vector<vector<Mat>> rectList)
{
    struct diff_offset
    {
        double differ;
        int xOffset;
        int yOffset;
    };

    Point result = Point(0,0);
//    qDebug()<<"min differ:"<<min;
    return result;
}

Mat imgFeatureTool::imageTranslation(Mat srcImage, int xOffset,int yOffset)
{
    Mat resultmage(srcImage.rows, srcImage.cols, srcImage.type(), cv::Scalar::all(0));
    int rows = srcImage.rows;
    int cols = srcImage.cols;
    //遍历图像
    for (int i = 0; i < rows; i++)
    {
        for(int j=0;j<cols;j++)
        {
            //映射变换
            int x = j - xOffset;
            int y = i - yOffset;
        //边界判断
        if (x >= 0 && y >= 0 && x < cols && y < rows)
            resultmage.at<uchar>(y,x) = srcImage.at<uchar>(i, j);
        }
    }
    return resultmage;
}

double imgFeatureTool::medianMat(cv::Mat Input, int nVals){

    // COMPUTE HISTOGRAM OF SINGLE CHANNEL MATRIX
    float val = nVals;
    float range[] = { 0, val };
    const float* histRange = { range };
    bool uniform = true; bool accumulate = false;
    cv::Mat hist;
    calcHist(&Input, 1, 0, cv::Mat(), hist, 1, &nVals, &histRange, uniform, accumulate);

    // COMPUTE CUMULATIVE DISTRIBUTION FUNCTION (CDF)
    cv::Mat cdf;
    hist.copyTo(cdf);
    for (int i = 1; i <= nVals-1; i++){
        cdf.at<float>(i) += cdf.at<float>(i - 1);
    }
    cdf /= Input.total();

    // COMPUTE MEDIAN
    double medianVal;
    for (int i = 0; i <= nVals-1; i++){
        if (cdf.at<float>(i) >= 0.5) { medianVal = i;  break; }
    }
    return medianVal/nVals;
}
