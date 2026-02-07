#ifndef LINEFINDER_H
#define LINEFINDER_H
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/line_descriptor/descriptor.hpp>

using namespace cv;
using namespace cv::line_descriptor;
using namespace std;

typedef struct
{
    int gray;
    int y;
    int x;
}img_gray_point;

class LineFinder
{
public:
    //水平、垂直直线
    vector<Vec4i> linehorizon;
    vector<Vec4i> lineplumb;
    //虚拟直线
    vector<Vec4i> linedotted;
    void setLine(std::vector<cv::Vec4i> &hough_line)
    {
        temp_lines.clear();
        temp_lines = hough_line;
    }
    vector<Vec4i> getline_erase()
    {
        return line_erase;
    }
    void setComplementValve(int valve)
    {
        complement_valve = valve;
    }
    //被删除直线
    std::vector<cv::Vec4i> line_erase;

private:
    cv::Mat img;
    //包含被检测直线的端点的向量
    std::vector<cv::Vec4i> lines;

    //累加器分辨率参数
    double deltaRho;
    double deltaTheta;

    //确认直线之前必须受到的最小投票数
    int minVote;

    //直线的最小长度
    double minLength;
    //直线上允许的最大空隙
    double maxGap;

   //临时存放直线
    std::vector<cv::Vec4i> temp_lines;

    vector<img_gray_point> line_matrix;

    int complement_valve;//补全直线的阈值

public:
    LineFinder() :deltaRho(1), deltaTheta(CV_PI / 180), minVote(10), minLength(0.0), maxGap(0.0) {}
    void setAccResolution(double dRho, double dTheta)
    {
        deltaRho = dRho;
        deltaTheta = dTheta;
    }
    void setminVote(int minv)
    {
        minVote = minv;
    }
    void setLengthAndGap(double length, double gap)
    {
        minLength = length;
        maxGap = gap;
    }
    std::vector<Vec4i> findLines(cv::Mat& binary)
    {
        lines.clear();
        cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
        return lines;
    }
    void drawDetectedLines(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 255, 255))
    {
        std::vector<Vec4i>::const_iterator it = lines.begin();
        while (it!=lines.end())
        {
            cv::Point pt1((*it)[0], (*it)[1]);
            cv::Point pt2((*it)[2], (*it)[3]);
            cv::line(image, pt1, pt2, color);
            it++;
        }
    }
    std::vector<Vec4i> delete_line(int x1,int y1,int x2,int y2)
    {
        std::list<Vec4i> linesList;
        line_erase.clear();
        for(int i = 0; i < temp_lines.size(); i++)
        {
            if(temp_lines[i][0] >= x1 && temp_lines[i][0] <= x2 && temp_lines[i][2] >= x1 && temp_lines[i][2] <= x2   &&  temp_lines[i][1] >= y1 && temp_lines[i][1] <= y2 && temp_lines[i][3] >= y1 && temp_lines[i][3] <= y2)
            {
                line_erase.push_back(temp_lines[i]);
            }else
            {
                linesList.push_back(temp_lines[i]);
            }
        }
        temp_lines.clear();
        for(Vec4i line : linesList)
        {
            temp_lines.push_back(line);
        }
//        for(Vec4i line :temp_lines)
//        {
//            qDebug()<<line[0]<<","<<line[1]<<","<<line[2]<<","<<line[3];
//        }
        return temp_lines;
    }
    double angleForCorrect(const cv::Vec4i& line)
    {
        Vec4i unitXVector(0, 0, 1, 0);
        double angle = angle_lines(unitXVector, line);  // here angle belongs to [0, pi/2]
        // @attention: the increment direction of X and Y axis of OpenCV is different from usual rectangular coordinate system. The origin point is in the upper left corner of the image
        if (angle < 45)
        {
            // consider in the horizontal direction
            if (line[1] > line[3])
            {
                angle = -angle;
            }
        }
        else
        {
            // consider in the vertical direction
            if (line[1] > line[3])
            {
                angle = 90 - angle;
            }
            else
            {
                angle = angle - 90;
            }
        }

        return angle;
    }
    vector<Vec4i> selectLine(vector<Vec4i> lines,int x1,int y1,int x2,int y2)
    {
        vector<Vec4i> linesList;
        for(int i = 0; i < lines.size(); i++)
        {
            if(lines[i][0] >= x1 && lines[i][0] <= x2 && lines[i][2] >= x1 && lines[i][2] <= x2   &&  lines[i][1] >= y1 && lines[i][1] <= y2 && lines[i][3] >= y1 && lines[i][3] <= y2)
            {
                linesList.push_back(lines[i]);
            }
        }
        return linesList;
    }
    double angle_lines(const Vec4i& line1, const Vec4i& line2)
    {
        double moduleLine1 = sqrt(pow(line1[0] - line1[2], 2) + pow(line1[1] - line1[3], 2));
        double moduleLine2 = sqrt(pow(line2[0] - line2[2], 2) + pow(line2[1] - line2[3], 2));
        double dotProduct = (line1[0] - line1[2]) * (line2[0] - line2[2]) + (line1[1] - line1[3]) * (line2[1] - line2[3]);
        return acos(dotProduct / moduleLine1 / moduleLine2) * 180 / CV_PI;
    }
    //分离垂直和水平线
    void separate(std::vector<Vec4i> houghlines)
    {
        for(Vec4i line : houghlines)
        {
            if(abs(angle_lines(line,Vec4i(0,0,10,0))) < 1)
            {
                linehorizon.push_back(line);
            }else
            {
                lineplumb.push_back(line);
            }
        }
    }
//    void complement()
//    {
//        for(Vec4i hv4i : linehorizon)
//        {
//            if(hv4i[0] > hv4i[2])
//            {
//                hv4i = Vec4i(hv4i[2],hv4i[1],hv4i[0],hv4i[3]);
//            }
//            for(Vec4i pv4i : lineplumb)
//            {
//                if(pv4i[1] > pv4i[3])
//                {
//                    pv4i = Vec4i(pv4i[0],pv4i[3],pv4i[2],pv4i[1]);
//                }
//                if(pv4i[0] + 5 > hv4i[0] && pv4i[0] - 5 < hv4i[2] && abs(angle_lines(hv4i, pv4i) - 90) < 2)
//                {
//                    if(abs(hv4i[1] - pv4i[1]) < abs(hv4i[1] - pv4i[3]))
//                    {
//                        if(abs(hv4i[1] - pv4i[1]) < complement_valve)//后期可设计为可动条，两直线连接判断的最大距离，水平线到垂直线上端点
//                        linedotted.push_back(Vec4i(pv4i[0],hv4i[1],pv4i[2],pv4i[3]));
//                    }else
//                    {
//                        if(abs(hv4i[1] - pv4i[3]) < complement_valve)//后期可设计为可动条，两直线连接判断的最大距离,水平线到垂直线下端点
//                        linedotted.push_back(Vec4i(pv4i[0],pv4i[1],pv4i[2],hv4i[3]));
//                    }
//                }
//            }
//        }
//    }
    vector<Vec4i> detectPrimary(vector<Vec4i> hough_lines, vector<vector<Vec4i>>& lineFiltered)
    {
        list<Vec4i> linesList;
        vector<Rect> rectangleLines;
        for(vector<Vec4i>::iterator itor = hough_lines.begin(); itor != hough_lines.end(); itor++)
        {
            linesList.push_back(*itor);
        }
        int i = 0;
        vector<Vec4i> line_temp;

        for(list<Vec4i>::iterator itorOuter = linesList.begin(); itorOuter != linesList.end();)
        {
            for(list<Vec4i>::iterator itorInner = ++linesList.begin(); itorInner != linesList.end();)
            {
                //集合中没有相邻线段时
                if(line_temp.size() == 0)
                {
                    //相邻判断
                    if(checkVertical(*itorOuter, *itorInner))//垂直判断
                    {
                        if(linesList.size() > 2)
                        {
                            //发现符合条件的相邻线，删除inner线
                            line_temp.push_back(*itorOuter);
                            line_temp.push_back(*itorInner);
                            itorInner = linesList.erase(itorInner);
                            break;
                        }else
                        {
                            //只剩2线，停止循环
                            line_temp.push_back(*itorOuter);
                            line_temp.push_back(*itorInner);
                            lineFiltered.push_back(line_temp);
                            line_temp.clear();
                            itorOuter = linesList.end();
                            break;
                        }
                    }else
                    {
                        //当没有发现相邻线段时
                        //当inner完成一个循环
                        if(itorInner == --linesList.end())
                        {
                            if (linesList.size() > 2)
                            {
                                line_temp.push_back(*itorOuter);
                                lineFiltered.push_back(line_temp);
                                line_temp.clear();
                                itorOuter = linesList.erase(itorOuter);
                                break;
                            }else //只剩2线
                            {
                                line_temp.push_back(*itorOuter);
                                lineFiltered.push_back(line_temp);
                                line_temp.clear();
                                line_temp.push_back(*itorInner);
                                lineFiltered.push_back(line_temp);
                                line_temp.clear();
                                itorOuter = linesList.end();
                                break;
                            }
                        }
                        //检测inner没到end时，继续遍历inner
                        itorInner++;
                    }
                }else//当遍历找到有相邻直线时,即line_temp.size不为0
                {
                    if(checkVertical(*itorOuter, *itorInner))
                    {
                        //符合条件时，重新遍历inner并检测inner是否与数组相邻
                        if(linesList.size() > 2)
                        {
                            line_temp.push_back(*itorInner);
                            itorInner = linesList.erase(itorInner);
                            break;
                        }else //只剩2线，判断是否与数组相邻
                        {
                            line_temp.push_back(*itorInner);
                            lineFiltered.push_back(line_temp);
                            line_temp.clear();
                            itorOuter = linesList.end();
                            break;
                        }
                    }else
                    {
                        //当遍历完没有发现相邻线段时
                        if(itorInner == --linesList.end())
                        {
                            //当Inner遍历没有与数组相邻线，删除原数组中的outer,进入新循环
                            if (linesList.size() > 2)
                            {
                                lineFiltered.push_back(line_temp);
                                line_temp.clear();
                                itorOuter = linesList.erase(itorOuter);
                                break;
                            }else //只剩2线,不相邻时
                            {
                                lineFiltered.push_back(line_temp);
                                line_temp.clear();
                                line_temp.push_back(*itorInner);
                                lineFiltered.push_back(line_temp);
                                line_temp.clear();
                                itorOuter = linesList.end();
                                break;
                            }
                        }
                        itorInner++;
                    }
                }
            }
        }
        int temp_size = lineFiltered.at(0).size();
        int max_index;
        for(int i = 0; i < lineFiltered.size(); i++)
        {
            if(temp_size <= lineFiltered.at(i).size())
            {
                temp_size = lineFiltered.at(i).size();
                max_index = i;
            }
        }
        vector<Vec4i> lines = lineFiltered.at(max_index);
        return lines;
    }
    bool checkVertical(const Vec4i& outer, const Vec4i& inner)
    {
        if(abs(angle_lines(outer, inner) - 90) < 0.1)
        {
            return true;
        }
        if(abs(angle_lines(outer, inner)) < 0.1)
        {
            return true;
        }
        return false;
    }
    //自动聚合直线
    vector<Rect> auto_lineGroup(vector<Vec4i> &lineAll)
    {
        list<Vec4i> linesList;
        vector<Rect> rectangleLines;

        return rectangleLines;
    }
    bool closestLines(const Vec4i& outer, const Vec4i& inner)
    {
        Vec4i v1 = sortVec4i(outer);
        Vec4i v2 = sortVec4i(inner);
        int threshold = 2;//聚合直线的相邻阈值

        if(v1[0] == v1[2] && v2[0] == v2[2] && abs(v2[0] - v1[0]) <= threshold)
        {
            if(v1[1] <= v2[1] && v1[3] + threshold >= v2[1])
            {
                return true;
            }else if(v2[1] < v1[1] && v2[3] + threshold >= v1[1])
            {
                return true;
            }
        }else if(v1[1] == v1[3] && v2[1] == v2[3] && abs(v2[1] - v1[1]) <= threshold)
        {
            if(v1[0] <= v2[0] && v1[2] + threshold >= v2[0])
            {
                return true;
            }else if(v2[0] < v1[0] && v2[2] + threshold >= v1[0])
            {
                return true;
            }
        }
        return false;
    }
    bool closestLines(const Vec4i& inner,vector<Vec4i> line_temp)
    {
        Vec4i v2 = sortVec4i(inner);
        int threshold = 2;//聚合直线的相邻阈值
        for(Vec4i v1 : line_temp)
        {
            v1 = sortVec4i(v1);
            if(v1[0] == v1[2] && v2[0] == v2[2] && abs(v2[0] - v1[0]) <= threshold)
            {
                if(v1[1] <= v2[1] && v1[3] + threshold >= v2[1])
                {
                    return true;
                }else if(v2[1] < v1[1] && v2[3] + threshold >= v1[1])
                {
                    return true;
                }
            }else if(v1[1] == v1[3] && v2[1] == v2[3] && abs(v2[1] - v1[1]) <= threshold)
            {
                if(v1[0] <= v2[0] && v1[2] + threshold >= v2[0])
                {
                    return true;
                }else if(v2[0] < v1[0] && v2[2] + threshold >= v1[0])
                {
                    return true;
                }
            }
        }
        return false;
    }
    Rect line_rectangle(vector<Vec4i> linelist)
    {
        if(linelist.size() < 1)
        {
            return Rect(0,0,0,0);
        }
        int temp_x1 = linelist[0][0],temp_y1 = linelist[0][1],temp_x2 = linelist[0][2],temp_y2 = linelist[0][3];
        for(Vec4i line : linelist)
        {
            //找最小点x
            if(temp_x1 > line[0])
            {
                temp_x1 = line[0];
            }else if(temp_x1 > line[2])
            {
                temp_x1 = line[2];
            }
            //找最小点y
            if(temp_y1 > line[1])
            {
                temp_y1 = line[1];
            }else if(temp_y1 > line[3])
            {
                temp_y1 = line[3];
            }
            //找最大点x
            if(temp_x2 < line[2])
            {
                temp_x2 = line[2];
            }else if(temp_x2 < line[0])
            {
                temp_x2 = line[0];
            }
            //找最大点y
            if(temp_y2 < line[3])
            {
                temp_y2 = line[3];
            }else if(temp_y2 < line[1])
            {
                temp_y2 = line[1];
            }
        }
        return Rect (temp_x1,temp_y1,temp_x2-temp_x1,temp_y2-temp_y1);
    }
    vector<img_gray_point> line_alignment(vector<Rect> rect_lines,Mat design_threshimg,int min_x,int min_y,int max_x,int max_y)
    {
        img_gray_point gray_img;
        vector<img_gray_point> line_matrix;
        for(int y = 0; y < design_threshimg.rows - (max_y - min_y); y++)
        {
            for(int x = 0; x < design_threshimg.cols - (max_x - min_x); x++)
            {
                int temp_gray = 0;
                for(Rect r : rect_lines)
                {

                    for(int i = 0; i < r.height; i++)
                    {
                        for(int j = 0; j < r.width; j++)
                        {
                            temp_gray += design_threshimg.at<uchar>(y + i + (r.y - min_y),x + j + (r.x - min_x));
                        }
                    }
                }
                gray_img.gray = temp_gray;
                gray_img.x = x;
                gray_img.y = y;
                line_matrix.push_back(gray_img);
            }
        }
        return line_matrix;
    }
    Vec4i sortVec4i(Vec4i sp)
    {
        int x1,y1,x2,y2;
        x1 = sp[0] < sp[2] ? sp[0] : sp[2];
        y1 = sp[1] < sp[3] ? sp[1] : sp[3];
        x2 = qAbs(sp[0] - sp[2]) + x1;
        y2 = qAbs(sp[1] - sp[3]) + y1;
        return Vec4i(x1,y1,x2,y2);
    }
};

#endif // LINEFINDER_H
