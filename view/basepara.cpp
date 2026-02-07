<<<<<<< HEAD
﻿#include "FileReader_Thread.h"
#include "basepara.h"
#include "imageformat.h"
#include "imgFeatureTool.h"
#include "ui_basepara.h"
#include <qdebug.h>
#include <qeventloop.h>
#include <qtimer.h>
#include <QTimer>
#include <QtMath>
#include <QMessageBox>
#include "imgcutthread.h"
basePara::basePara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::basePara)
{
    ui->setupUi(this);


    genChipDesignObj  = ui->page;
    computeRedressObj = ui->page_2;
    imageRotateObj    = ui->page_3;

    connect(genChipDesignObj,SIGNAL(signalGenChipDesignPara(int)),this,SLOT(slotGenChipDesignPara(int)));
    //图像选区

    connect(computeRedressObj,SIGNAL(signalReduct()),this,SLOT(slotReduct()));
    connect(computeRedressObj,SIGNAL(signalSaveRect()),this,SLOT(slotSaveRect()));
    connect(computeRedressObj,SIGNAL(signalComputer()),this,SLOT(slotComputer()));
    connect(computeRedressObj,SIGNAL(signalImgCrop()),this,SLOT(slotImgCrop()));

    //选区
    connect(imageRotateObj,SIGNAL(signalImgSelect(int)),this,SLOT(slotImgSelect(int)));
    connect(imageRotateObj,SIGNAL(signalReduct()),this,SLOT(slotReduct2()));

    //二值化
    connect(imageRotateObj->binarizationObj,SIGNAL(signalRunBinarizaType(int)),this,SLOT(slotRunBinarizaType(int)));
    connect(imageRotateObj->binarizationObj,SIGNAL(signalSetThreasholdType(int)),this,SLOT(slotSetThreasholdType(int)));
    connect(imageRotateObj->binarizationObj,SIGNAL(signalRunThreashold(int)),this,SLOT(slotRunThreashold(int)));
    //直线提取
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalMinLength(int)),this,SLOT(slotMinLength(int)));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalMaxGap(int)),this,SLOT(slotMaxGap(int)));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalMinVote(int)),this,SLOT(slotMinVote(int)));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalAutoLine()),this,SLOT(slotAutoLine()));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalAggregateLine()),this,SLOT(slotAggregateLine()));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalSaveLine()),this,SLOT(slotSaveLine()));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalImgSelect(int)),this,SLOT(slotImgSelect(int)));
    //比对
    connect(imageRotateObj,SIGNAL(signalDesignSelect(int)),this,SLOT(slotDesignSelect(int)));
    connect(imageRotateObj,SIGNAL(signalMatch()),this,SLOT(slotMatch()));





    init();
}

basePara::~basePara()
{
    delete ui;
}

void basePara::init()
{
    imgCropThread = new ImageCropThreadInfo();
    lineFinderTool = new LineFinder();
    connect(imgCropThread, SIGNAL(computerResult()), this, SLOT(showComputer()));
    connect(imgCropThread, SIGNAL(allOver()), this, SLOT(workOver()));
    connect(this->ui->page_2,SIGNAL(signalDeliverParaToBasepara(bool,int,int)),this,SLOT(receiveFromBasePara(bool,int,int)));
}
//红外文件路径
void basePara::slotinfraredFile(QStringList s)
{
    infraredPathList = s;
    emit deliverDenPathSignal(s.at(0).left(s.at(0).lastIndexOf("/")+1));
    infraredOpenInit();
    jumpViewId(0);
}
//芯片文件路径
void basePara::slotDesignFilePath(QStringList s)
{
    DesignPathList = s;
    emit deliverRefPath(s.at(0).left(s.at(0).lastIndexOf("/")+1));
    //**********参考图原始数据不产生Mat图像(图像对齐调试用)!!!***************
    if(showDebugMode)
    {
        Mat imgDesign = fileOpenInit(DesignPathList.at(0));
        equalizeHist( imgDesign, imgDesign );
        threshold(imgDesign,imgDesign,30,255,THRESH_BINARY/*_INV*/);
    //    imgDesign = imgFeatureTool::imgFeatureThreshold(imgDesign,2,THRESH_BINARY);

        designMat = imgDesign.clone();
        QImage showIMG = imageFormat::MatToQImage(designMat);
        signalrimg1(QPixmap::fromImage(showIMG));
        signalrimg2(QPixmap::fromImage(showIMG));
    }
}
//图像生成参数
void basePara::slotGenChipDesignPara(int offsetType)
{
    scale = genChipDesignObj->scale;
    if(!showDebugMode)
    {
        if(!GenChipDesignisRun)
        {

            GenChipDesignisRun = true;
            startProgressViewId(0);
        }
    }else
    {
        complete();
        referpath = DesignPathList.at(0).left(DesignPathList.at(0).lastIndexOf("/")+1);
    }
}

void basePara::recPageIdx(int page)
{
    this->showPage = page;
    this->ui->stackedWidget->setCurrentIndex(page);
}

//图像生成完成
void basePara::complete()
{
    GenChipDesignisRun = false;
    signalComplete();
    signalSetLog(0,"图像生成完成");
}
//参考图展示
void basePara::showFisrtMat(Mat ist, QString path)
{
    imageFormat::mat2gray(ist);
    equalizeHist( ist, ist );
//    threshold(ist,ist,5,255,THRESH_BINARY_INV);
    designMat = ist.clone();
    QImage showIMG = imageFormat::MatToQImage(ist);
    signalrimg1(QPixmap::fromImage(showIMG));
    signalrimg2(QPixmap::fromImage(showIMG));
    //传递参考图路径
    referpath = path;
    emit deliverRefPath(referpath);
}
//运行进度
void basePara::slotProgress(int p)
{

}
void basePara::infraredOpenInit()
{
    rectList.clear();
    rect_lines.clear();
    FileReader_Thread *m_csvreader = new FileReader_Thread(this);
    QString dirpath = infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1);
    QStringList files = m_csvreader->readPath(dirpath);
    if(files.size() <= 0)
    {
        return;
    }
    QString filename = dirpath + files.at(0);
    m_csvreader->readfile(filename.toStdString());
    rows = m_csvreader->getSize().height();
    cols = m_csvreader->getSize().width();
    selectRect = Rect(0,0,cols,rows);
    signalSetLog(1,"infraRed Rect:"+QString("(x%1,y%2)").arg(cols).arg(rows));
    signalInfraredRect(QRect(0,0,cols,rows));//传输实际图尺寸
    initial = m_csvreader->readFileToMat(filename.toStdString());
    //    featureTool->rotateIamge(initial,initial,90);    //旋转90度
    imageFormat::mat2gray(initial);
    showMat = initial.clone();
//    imgFeatureTool::img2ALCHE(initial,alche);
    //canny
    //    alche = featureTool->imgCanny(alche,80,80);
    QImage showIMG = imageFormat::MatToQImage(initial);
    signallimg1(QPixmap::fromImage(showIMG));
    signallimg2(QPixmap::fromImage(showIMG));
    //传递红外文件给图像矫正
//    signalInfraredMat(initial);
}

Mat basePara::fileOpenInit(QString path)
{
    Mat fileMat;
    FileReader_Thread *fileReaderTool = new FileReader_Thread();
    if(path.endsWith(".csv") || path.endsWith(".xls") || path.endsWith(".xlsx"))
    {
        fileReaderTool->readfile(path.toStdString());
        fileMat = fileReaderTool->readFileToMat(path.toStdString());

        imageFormat::mat2gray(fileMat);
//        imshow(path.toStdString(),fileMat);
        signalSetLog(2,"design Rect:"+QString("(x%1,y%2)").arg(fileReaderTool->getSize().width()).arg(fileReaderTool->getSize().height()));
//        imwrite(path.toStdString()+".jpg",fileMat);
    }else
    {
        fileMat = imread(path.toStdString(),CV_BGR2GRAY);
    }
    //红外图像判断
    if(infraredPathList.size() > 0)
    {
        if(infraredPathList.at(0) == path)
        {
            initial = fileMat.clone();
            showMat = initial.clone();
            selectRect = Rect(0,0,initial.cols,initial.rows);
        }
    }
    return fileMat;
}
void basePara::slotReduct()
{
    showMat = initial.clone();
    selectRect = Rect(0,0,initial.cols,initial.rows);
    QImage showIMG = imageFormat::MatToQImage(showMat);
    signallimg1(QPixmap::fromImage(showIMG));
    SignalCapScreen1(-1);
}

void basePara::slotReduct2()
{
    showMat = initial.clone();
    selectRect = Rect(0,0,initial.cols,initial.rows);
    QImage showIMG = imageFormat::MatToQImage(showMat);
    signallimg2(QPixmap::fromImage(showIMG));
    SignalCapScreen2(-1);
}

void basePara::slotImgSelect(int step)
{
    SignalCapScreen2(step);//0禁用框选1红外图像选区2参考图像3删除图像
    if(step == 3)
    {
        imageRotateObj->toolBoxObj->setCurrentIndex(0);
    }
}

void basePara::slotDesignSelect(int step)
{
    SignalDesignScreen(step);//0禁用框选1红外图像选区2参考图像3删除图像
}
void basePara::slotSaveRect()
{
    rectList.push_back(selectRect);
    selectRect = Rect(0,0,initial.cols,initial.rows);
    Mat show = initial.clone();
    cvtColor(show,show,CV_GRAY2BGR);
    for(int i = 0; i < rectList.size(); i++)
    {
        rectangle(show, rectList.at(i), Scalar(255, 255, 0), 2);
    }
    QImage showIMG = imageFormat::MatToQImage(show);
    signallimg1(QPixmap::fromImage(showIMG));
}
//计算偏移
void basePara::slotComputer()
{
    if(rectList.size() < 2)
    {
        //弹窗阻止
        return;
    }
    QString dirpath = infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1);
    startProgressViewId(1);
    imgCropThread->setRedress(QSize(cols,rows),rectList,dirpath);
    imgCropThread->start();
}
//图像对齐的偏移计算
void basePara::showComputer()
{
    int index=0;
    for(Point p:imgCropThread->getResultList())
    {
        if(p.x != 0 || p.y != 0)
        {
            signalSetLog(1,u8"发现一个图像偏移："+QString::number(p.x)+"，"+QString::number(p.y));
            index++;
        }
    }
    signalSetLog(1,u8"本次检测发现偏移图像的数量："+QString::number(index));
    iscomplete = true;
    slotImgCrop();//计算后开始裁剪
}

void basePara::slotImgCrop()
{
    signalSetLog(1,u8"开始红外偏移裁剪");
    if(iscomplete)
    {
        imgCropThread->DoCrop();
        imgCropThread->start();
    }
}

void basePara::workOver()
{
    //完成图像裁剪
    signalSetLog(1,"完成红外图像裁剪");
    QString dirpath = infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1)+"result/";
    signalSetLog(1,"结果保存于:"+dirpath);
    FileReader_Thread *m_csvreader = new FileReader_Thread(this);
    QStringList files = m_csvreader->readPath(dirpath,"*.csv");
    QString filename = dirpath + files.at(0);
    m_csvreader->readfile(filename.toStdString());
    Mat infraredMat = m_csvreader->readFileToMat(filename.toStdString());
    //传递红外文件给图像矫正
//    signalInfraredMat(infraredMat);
    //传递给图像降噪
    imageFormat::mat2gray(infraredMat);
    initial = infraredMat.clone();
    showMat = initial.clone();
    rows = initial.rows;
    cols = initial.cols;
    selectRect = Rect(0,0,cols,rows);

    //传递结果和文件生成路径给降噪
    qDebug()<<dirpath;
    signalInfraredDate(dirpath,QPixmap::fromImage(imageFormat::MatToQImage(infraredMat)));

    signalSetModule(2);
    completeImgCorrection();
    completeViewId(1);
    jumpViewId(2);
}

void basePara::SlotSelectRect(Rect rect)
{
    selectRect = Rect(selectRect.x+rect.x, selectRect.y+rect.y, rect.width, rect.height);
    showMat = initial(selectRect);
    QImage showIMG = imageFormat::MatToQImage(showMat);
    signallimg1(QPixmap::fromImage(showIMG));
}
void basePara::SlotSelectRect2(Rect rect)
{
    selectRect = Rect(selectRect.x+rect.x, selectRect.y+rect.y, rect.width, rect.height);
    showMat = initial(selectRect);
    QImage showIMG = imageFormat::MatToQImage(showMat);
    signallimg2(QPixmap::fromImage(showIMG));
    imageRotateObj->toolBoxObj->setCurrentIndex(1);
}
void basePara::slotRunBinarizaType(int bt)
{
    BinarizaType = bt;
    setAutoThresholdSetting(resultMat);

    QImage showIMG = imageFormat::MatToQImage(resultMat);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::slotSetThreasholdType(int tt)
{
    ThreasholdType = tt;
    if(resultMat.empty())
    {
        return;
    }
    setAutoThresholdSetting(resultMat);

    QImage showIMG = imageFormat::MatToQImage(resultMat);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::slotRunThreashold(int t)
{
    Threashold = t;
    imgThreshold(resultMat,Threashold,ThreasholdType);

    QImage showIMG = imageFormat::MatToQImage(resultMat);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::setAutoThresholdSetting(Mat& thresh)
{
    thresh = imgFeatureTool::imgFeatureThreshold(showMat,BinarizaType,ThreasholdType);
}

void basePara::imgThreshold(Mat &mat, int threshold, int type)
{
    cv::threshold(showMat, mat, threshold, 255, type);
    imgFeatureTool::removeIsland(mat,mat);
}

void basePara::slotMinLength(int ml)
{
    MinLength = ml;
//    houghLineFeature(resultMat,MinLength,MaxGap,MinVote);
}

void basePara::slotMaxGap(int mg)
{
    MaxGap = mg;
//    houghLineFeature(resultMat,MinLength,MaxGap,MinVote);
}

void basePara::slotMinVote(int mv)
{
    MinVote = mv;
//    houghLineFeature(resultMat,MinLength,MaxGap,MinVote);
}

void basePara::houghLineFeature(Mat mat, int minlength, int maxgap, int minvote)
{
    temp_houghLines.clear();
    if(resultMat.empty())
    {
        signalSetLog(2,"图像未处理");
        return;
    }
    lineFinderTool->setAccResolution(1,CV_PI / 2);
    lineFinderTool->setLengthAndGap(minlength,maxgap);
    lineFinderTool->setminVote(minvote);
    temp_houghLines = lineFinderTool->findLines(mat);

    Mat HoughLine;
    cvtColor(mat,HoughLine,COLOR_GRAY2BGR);
    for (int i = 0, steps = temp_houghLines.size(); i < steps; i++)
    {
        line(HoughLine,Point(temp_houghLines[i][0], temp_houghLines[i][1]), Point(temp_houghLines[i][2], temp_houghLines[i][3]), Scalar(50, 255, 200), 1, 8);
    }

    QImage showIMG = imageFormat::MatToQImage(HoughLine);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::slotAutoLine()
{
    Vec4i conPoints;
    imgFeatureTool::getContourRect(resultMat, conPoints);
    temp_hough_rect = Rect(conPoints[0],conPoints[1],conPoints[2]-conPoints[0],conPoints[3]-conPoints[1]);
    Mat HoughLine;
    cvtColor(resultMat,HoughLine,COLOR_GRAY2BGR);
    rectangle(HoughLine, temp_hough_rect, Scalar(0, 255, 255), 2);

    QImage showIMG = imageFormat::MatToQImage(HoughLine);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::slotAggregateLine()
{
    QString rectLinesInfo;
    for(Rect rect: rect_lines)
    {
        rectLinesInfo += QString::number(rect.x)+","+QString::number(rect.y)+","+QString::number(rect.width)+","+QString::number(rect.height) + '\r\n';
    }
    //展示直线
    Mat rectangle_mat;
    cvtColor(initial,rectangle_mat,CV_GRAY2BGR);
    int linegroup_count = 0;
    for(Rect r : rect_lines)
    {
        signalSetLog(2,"-----红外实际特征坐标-------");
        signalSetLog(2,QString::number(linegroup_count)+":"+QString("Rect(x%1,y%2,w%3,h%4)").arg(r.x).arg(r.y).arg(r.width).arg(r.height));
        if(linegroup_count++ == 0)
        {
            rectangle(rectangle_mat, r, Scalar(255, 255, 0), 2);
        }else
        {
            rectangle(rectangle_mat, r, Scalar(0, 255, 255), 2);
        }
    }
    QImage showIMG = imageFormat::MatToQImage(rectangle_mat);
    signallimg2(QPixmap::fromImage(showIMG));

    signalSetLog(2,"直线特征已获取");
    imageRotateObj->toolBoxObj->setCurrentIndex(3);
}
//保存直线
void basePara::slotSaveLine()
{

    Rect r=Rect(temp_hough_rect.x+ selectRect.x,temp_hough_rect.y+ selectRect.y,temp_hough_rect.width,temp_hough_rect.height);
    rect_lines.push_back(r);

    //展示直线
    Mat HoughLine;
    cvtColor(initial,HoughLine,CV_GRAY2BGR);

    rectangle(HoughLine, r, Scalar(0, 255, 255), 2);
    QPixmap showIMG = QPixmap::fromImage(imageFormat::MatToQImage(HoughLine));
    signallimg2(showIMG);
    selectRect = Rect(0,0,initial.cols,initial.rows);

}

//发送降噪开始信号
void basePara::receiveFromBasePara(bool checkbox, int pointCount, int  threadCount)
{
    //现将数据传送给降噪类

    // 再传递开始信号

}
void basePara::testCheck(int arg1)
{

}

void basePara::slotSetLog(int i, QString s)
{
    signalSetLog(i, s);
}
void basePara::slotMatch()
{
    scale = genChipDesignObj->scale;
    //-----------------------------------按框选对齐测试------------------------------------------
    QPoint offset = imgFeatureTool::match(rect_lines,reference_lines);

    signalSetLog(2,"当前对比特征  参考图-实际特征的坐标差值:"+QString("Point(x%1,y%2)").arg(offset.x()).arg(offset.y()));
    Mat matchImg;
    cvtColor(initial,matchImg,CV_GRAY2BGR);
    //    ui->label_messg->setText("对齐后的各直线坐标：\n");
    int i = 1;
//    if(!lineIsHorizon)
//    {
        vector<double> point_x;
        for(Rect r : rect_lines)
        {
            if(i == 1)
            {
                rectangle(matchImg,Rect(r.x + offset.x(), r.y + offset.y(), r.width, r.height),Scalar(255 , 240 , 0), 2);
            }else
            {
                rectangle(matchImg,Rect(r.x + offset.x(), r.y + offset.y(), r.width, r.height),Scalar(0, 102, 255), 2);
            }
            putText(matchImg,to_string(i), Point(r.x + offset.x() - 10, r.y + offset.y() - 10),FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 240, 235), 1);
            //        ui->label_messg->setText(ui->label_messg->text()+QString::number(i)+":"+QString::number(r.x + offset.x())+","+QString::number(r.y + offset.y())+","+QString::number(r.x + offset.x() + r.width)+","+QString::number(r.y + offset.y() + r.height)+'\n');
//            qDebug()<<"point-"+QString::number(i)+":"<<QRect((r.x + offset.x()),(r.y + offset.y()),(r.x + offset.x() + r.width),(r.y + offset.y() + r.height));
            i++;
            point_x.push_back(r.x + offset.x() + r.width / 2);
        }
        sort(point_x.begin(),point_x.end());
        pointlist_x.push_back(point_x);
        pointlist.push_back(point_x);
        referenceList.push_back(reference_lines);
        if(pointlist_x.size() == 2)
        {
            int size = rect_lines.size();
            double designX1 = referenceList[1].x + referenceList[1].width/2 - (referenceList[0].x + referenceList[0].width/2);
            double realX1 = rect_lines[0].x + rect_lines[0].width/2 - (rect_lines[size-1].x + rect_lines[size-1].width/2);
            resultX = designX1 / realX1;
            double designY1 = referenceList[1].y + referenceList[1].height/2 - (referenceList[0].y + referenceList[0].height/2);
            double realY1 = rect_lines[0].y + rect_lines[0].height/2 - (rect_lines[size-1].y + rect_lines[size-1].height/2);
            resultY = designY1 / realY1;

            signalSetLog(2,"两特征相对距离：x:"+QString("%1,%2").arg(designX1).arg(realX1));
            signalSetLog(2,"两特征相对距离：y:"+QString("%1,%2").arg(designY1).arg(realY1));
            signalSetLog(2,"比例值:x:" + QString::number(resultX*scale));
            signalSetLog(2,"比例值:y:" + QString::number(resultY*scale));
            completeViewId(2);
            jumpViewId(0);
            //若图像比例对齐，则先裁剪图像再跳转图像偏移
            //需要的数据：红外图特征的坐标、设计图特征的坐标、红外图特征与设计图特征最小坐标判断、两图特征相对位置判断（作差）、
            //|红外特征左上坐标-设计特征左上坐标|=两图相对距离差mmx、mmy。offset_xy=两图相对距离差-红外特征坐标、两图相对距离差-设计特征坐标
            //imgFeatureTool::imageTranslation(m,offset_x,offset_y,mmx,mmy);
            ir_xstart = 0,ir_xend = initial.cols,de_xstart = 0,de_xend = designMat.cols;
            ir_ystart = 0,ir_yend = initial.rows,de_ystart = 0,de_yend = designMat.rows;
            if(offset.x()<0)
            {
                ir_xstart = abs(offset.x());
            }else
            {
                de_xstart = offset.x();
            }
            if(offset.y()<0)
            {
                ir_ystart = abs(offset.y());
            }else
            {
                de_ystart = offset.y();
            }
            int x_size_ir = ir_xend-ir_xstart; //对齐后实际图x大小
            int x_size_de = de_xend-de_xstart; //对齐后参考图x大小
            int x_size_diff = x_size_de - x_size_ir; //对齐后参考-实际图x差异

            int y_size_ir = ir_yend-ir_ystart; //对齐后实际图y大小
            int y_size_de = de_yend-de_ystart; //对齐后实际图y大小
            int y_size_diff = y_size_de - y_size_ir;//对齐后参考-实际图y差异

            if(x_size_diff<0)
            {
                ir_xend = ir_xend-abs(x_size_diff);
            }else
            {
                de_xend = de_xend-abs(x_size_diff);
            }
            if(y_size_diff<0)
            {
                ir_yend = ir_yend-abs(y_size_diff);
            }else
            {
                de_yend = de_yend-abs(y_size_diff);
            }
            initial = initial(Range(ir_ystart,ir_yend), Range(ir_xstart,ir_xend));
            designMat = designMat(Range(de_ystart,de_yend), Range(de_xstart,de_xend));
            test1();
            completeAlign();

            signalSetLog(2,"比例值:" + QString::number(resultX*scale));
            signalSetLog(2,"比例值:" + QString::number(resultY*scale));
        }

        signalSetLog(2,"对齐匹配完成，正在切换目标");
        signalSetLog(2,"当前已计算对齐:" + QString::number(pointlist_x.size()) + "次");
        signalSetLog(2,"还需计算对齐:" + QString::number(rect_lines.size() - pointlist_x.size()) + "次");

    QPixmap showIMG = QPixmap::fromImage(imageFormat::MatToQImage(matchImg));
    signallimg2(showIMG);
    cutoverAims();
}

//选参考边
void basePara::SlotDesignRect(Rect MatRect)
{
    int x1 = MatRect.x;
    int y1 = MatRect.y;
    int x2 = MatRect.x + MatRect.width;
    int y2 = MatRect.y + MatRect.height;
    //选区内检测直线特征
    Mat design_select = designMat(MatRect);
    lineFinderTool->setAccResolution(1,CV_PI / 2);
    lineFinderTool->setLengthAndGap(20, 10);
    lineFinderTool->setminVote(40);
    vector<Vec4i> select_hough = lineFinderTool->findLines(design_select);
    //若阈值检测不到特征，则以新的阈值重新检测
    if(select_hough.size() == 0)
    {
        lineFinderTool->setLengthAndGap(10, 5);
        lineFinderTool->setminVote(30);
        select_hough = lineFinderTool->findLines(design_select);

        signalSetLog(2,"参考边特征正在重新检测1");
    }
    if(select_hough.size() == 0)
    {
        lineFinderTool->setLengthAndGap(5, 5);
        lineFinderTool->setminVote(20);
        select_hough = lineFinderTool->findLines(design_select);

        signalSetLog(2,"参考边特征正在重新检测2");
    }
    if(select_hough.size() == 0)
    {

        signalSetLog(2,"未检测到直线特征，请重选参考边");
        return;
    }
    vector<Vec4i> design_hough;
    for(Vec4i line : select_hough)
    {
        design_hough.push_back(Vec4i(line[0] + x1, line[1] + y1, line[2] + x1, line[3] + y1));
    }
    //聚合选区内直线特征，确定选区特征位置
    vector<Vec4i> select_lines;
    vector<Vec4i> temp_lines;
    select_lines = lineFinderTool->selectLine(design_hough,x1,y1,x2,y2);
    if(x2 - x1 > y2 - y1)
    {
        //判断选区特征为横线
        lineIsHorizon = true;
        for(Vec4i line : select_lines)
        {
            if(line[1] == line[3])
            {
                temp_lines.push_back(line);
            }
        }
    }else
    {
        //判断选区特征为竖线
        lineIsHorizon = false;
        for(Vec4i line : select_lines)
        {
            if(line[0] == line[2])
            {
                temp_lines.push_back(line);
            }
        }
    }
    //求矩形位置与大小,参考线
    reference_lines = lineFinderTool->line_rectangle(temp_lines);

    signalSetLog(2,"参考图特征坐标:"+QString("Rect(x%1,y%2,w%3,h%4)").arg(reference_lines.x).arg(reference_lines.y).arg(reference_lines.width).arg(reference_lines.height));
    Mat rectangle_mat;
    cvtColor(designMat,rectangle_mat,CV_GRAY2BGR);
    rectangle(rectangle_mat, reference_lines, Scalar(255, 255, 0), 2);
    QImage showIMG = imageFormat::MatToQImage(rectangle_mat);
    signalrimg2(QPixmap::fromImage(showIMG));

    signalSetLog(2,"参考边特征已获取");
}

//删除直线deleteLine
void basePara::SlotDeleteRect(Rect rect)
{
    lineFinderTool->setLine(hough_lines);
    hough_lines = lineFinderTool->delete_line(rect.x,rect.y,rect.x+rect.width,rect.y+rect.height);
    //展示直线
    Mat HoughLine;
    cvtColor(initial,HoughLine,CV_GRAY2BGR);
    for (int i = 0, steps = hough_lines.size(); i < steps; i++)
    {
        line(HoughLine,Point(hough_lines[i][0], hough_lines[i][1]), Point(hough_lines[i][2], hough_lines[i][3]), Scalar(50, 255, 200), 1, 8);
    }
    QImage showIMG = imageFormat::MatToQImage(HoughLine);
    signallimg2(QPixmap::fromImage(showIMG));
}

//切换目标
void basePara::cutoverAims()
{
    //移位
    vector<Rect> newRectLines;
    for(int i = 1; i < rect_lines.size(); i++)
    {
        newRectLines.push_back(rect_lines.at(i));
    }
    newRectLines.push_back(rect_lines.at(0));
    rect_lines = newRectLines;

    //展示直线
    Mat rectangle_mat;
    cvtColor(initial,rectangle_mat,CV_GRAY2BGR);
    int linegroup_count = 0;
    for(Rect r : rect_lines)
    {
        if(++linegroup_count == 1)
        {
            rectangle(rectangle_mat, r, Scalar(255, 255, 0), 2);
        }else
        {
            rectangle(rectangle_mat, r, Scalar(0, 255, 255), 2);
        }
    }
    QPixmap showIMG = QPixmap::fromImage(imageFormat::MatToQImage(rectangle_mat));
    signallimg2(showIMG);
    //计算结果
    if(pointlist_x.size() == 2 || pointlist_y.size() == 2)
    {
        signalrimg2(showIMG);

        Mat rectangle_mat;
        cvtColor(initial,rectangle_mat,CV_GRAY2BGR);
        int linegroup_count = 0;
        for(Rect r : rect_lines)
        {
            if(linegroup_count++ == 0)
            {
                rectangle(rectangle_mat, r, Scalar(255, 255, 0), 2);
            }else
            {
                rectangle(rectangle_mat, r, Scalar(0, 255, 255), 2);
            }
        }
        QImage showLiftIMG = imageFormat::MatToQImage(rectangle_mat);
        signallimg2(QPixmap::fromImage(showLiftIMG));
        signalSetModule(1);

        pointlist_x.clear();
        referenceList.clear();
        pointlist_y.clear();
//        signalDesignPixmap(QPixmap::fromImage(imageFormat::MatToQImage(designMat)));//传递给图像降噪
    }
}
void basePara::completeAlign()
{
    completeViewId(2);
    QMessageBox msgBox;
    msgBox.setText(QString("图像对齐完成。\n计算比例值为:%1,%2.\n建议缩放比例:%3,%4.").arg(resultX).arg(resultY).arg(resultX*scale).arg(resultY*scale));
    msgBox.setInformativeText("是否跳转图像偏移？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Yes:
        referenceResultDate(referpath,designMat);

        jumpViewId(4);
        break;
    case QMessageBox::No:
        jumpViewId(1);
        break;
    default:
        // should never be reached
        break;
    }
}
void basePara::completeImgCorrection()
{
    completeViewId(1);
}
void basePara::test1()//红外图参考图批量裁剪
{
    signalSetLog(2,"开始图像裁剪");
    Mat testMat,testMat2;
    QList<Mat> fileMatList;
    FileReader_Thread *fileReader = new FileReader_Thread();
    QStringList ref_MatList = fileReader->readPath(referpath);

    if(DesignPathList.size() > 0)
    {
        ref_MatList = DesignPathList;//
        referpath="";//
    }
    QDir DebugPath;
    DebugPath.mkdir(DesignPathList.at(0).left(DesignPathList.at(0).lastIndexOf("/")+1) + "cut/");//241024新建裁减后文件夹

    for(QString path:ref_MatList)
    {
        signalSetLog(2,"参考图像裁剪路径："+referpath+path);
        fileReader->readfile((referpath+path).toStdString());
        Mat tempMat;
        tempMat = fileReader->readFileToMat((referpath+path).toStdString());
        tempMat = tempMat(Range(de_ystart,de_yend), Range(de_xstart,de_xend));

        path.chop(4);//
        path += "_cut.csv";//
        imageFormat::writeCSV((referpath+path).toStdString(),tempMat);
        testMat = tempMat.clone();
        emit deliverRefPath(path.left(path.lastIndexOf("/")+1)+"cut/");//241024，传路径
//        emit deliverRefPath(referpath+path);//241024，只传文件，没传路径
    }

    QStringList ir_MatList = fileReader->readPath(infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1)+"result/");//
//    ir_MatList = infraredPathList;//
    for(QString s:ir_MatList)
    {
        s = infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1)+"result/"+s;

        signalSetLog(2,"红外采样图像裁剪路径："+s);
        fileReader->readfile((s).toStdString());
        Mat tempMat;
        tempMat = fileReader->readFileToMat((s).toStdString());
        tempMat = tempMat(Range(ir_ystart,ir_yend), Range(ir_xstart,ir_xend));

//        s.chop(4);//
//        s += "_cut.csv";//
        imageFormat::writeCSV((s).toStdString(),tempMat);
        testMat2 = tempMat.clone();
    }
    imageFormat::mat2gray(testMat);
    imageFormat::mat2gray(testMat2);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(testMat, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE );
    Mat linePic = Mat::zeros(testMat.rows, testMat.cols, CV_8UC3);
    for (int index = 0; index < contours.size(); index++){
        drawContours(linePic, contours, index, Scalar(0, 0, 255), 1, 8, hierarchy);
    }
//    imshow("1",linePic);
    cvtColor(testMat2,testMat2,CV_GRAY2BGR);
    addWeighted(linePic, 0.4, testMat2, 0.6, 0, linePic);
//    imshow("test",linePic);
}
=======
﻿#include "FileReader_Thread.h"
#include "basepara.h"
#include "imageformat.h"
#include "imgFeatureTool.h"
#include "ui_basepara.h"
#include <qdebug.h>
#include <qeventloop.h>
#include <qtimer.h>
#include <QTimer>
#include <QtMath>
#include <QMessageBox>
#include "imgcutthread.h"
basePara::basePara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::basePara)
{
    ui->setupUi(this);


    genChipDesignObj  = ui->page;
    computeRedressObj = ui->page_2;
    imageRotateObj    = ui->page_3;

    connect(genChipDesignObj,SIGNAL(signalGenChipDesignPara(int)),this,SLOT(slotGenChipDesignPara(int)));
    //图像选区

    connect(computeRedressObj,SIGNAL(signalReduct()),this,SLOT(slotReduct()));
    connect(computeRedressObj,SIGNAL(signalSaveRect()),this,SLOT(slotSaveRect()));
    connect(computeRedressObj,SIGNAL(signalComputer()),this,SLOT(slotComputer()));
    connect(computeRedressObj,SIGNAL(signalImgCrop()),this,SLOT(slotImgCrop()));

    //选区
    connect(imageRotateObj,SIGNAL(signalImgSelect(int)),this,SLOT(slotImgSelect(int)));
    connect(imageRotateObj,SIGNAL(signalReduct()),this,SLOT(slotReduct2()));

    //二值化
    connect(imageRotateObj->binarizationObj,SIGNAL(signalRunBinarizaType(int)),this,SLOT(slotRunBinarizaType(int)));
    connect(imageRotateObj->binarizationObj,SIGNAL(signalSetThreasholdType(int)),this,SLOT(slotSetThreasholdType(int)));
    connect(imageRotateObj->binarizationObj,SIGNAL(signalRunThreashold(int)),this,SLOT(slotRunThreashold(int)));
    //直线提取
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalMinLength(int)),this,SLOT(slotMinLength(int)));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalMaxGap(int)),this,SLOT(slotMaxGap(int)));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalMinVote(int)),this,SLOT(slotMinVote(int)));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalAutoLine()),this,SLOT(slotAutoLine()));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalAggregateLine()),this,SLOT(slotAggregateLine()));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalSaveLine()),this,SLOT(slotSaveLine()));
    connect(imageRotateObj->lineFeatureObj,SIGNAL(signalImgSelect(int)),this,SLOT(slotImgSelect(int)));
    //比对
    connect(imageRotateObj,SIGNAL(signalDesignSelect(int)),this,SLOT(slotDesignSelect(int)));
    connect(imageRotateObj,SIGNAL(signalMatch()),this,SLOT(slotMatch()));





    init();
}

basePara::~basePara()
{
    delete ui;
}

void basePara::init()
{
    imgCropThread = new ImageCropThreadInfo();
    lineFinderTool = new LineFinder();
    connect(imgCropThread, SIGNAL(computerResult()), this, SLOT(showComputer()));
    connect(imgCropThread, SIGNAL(allOver()), this, SLOT(workOver()));
    connect(this->ui->page_2,SIGNAL(signalDeliverParaToBasepara(bool,int,int)),this,SLOT(receiveFromBasePara(bool,int,int)));
}
//红外文件路径
void basePara::slotinfraredFile(QStringList s)
{
    infraredPathList = s;
    emit deliverDenPathSignal(s.at(0).left(s.at(0).lastIndexOf("/")+1));
    infraredOpenInit();
    jumpViewId(0);
}
//芯片文件路径
void basePara::slotDesignFilePath(QStringList s)
{
    DesignPathList = s;
    emit deliverRefPath(s.at(0).left(s.at(0).lastIndexOf("/")+1));
    //**********参考图原始数据不产生Mat图像(图像对齐调试用)!!!***************
    if(showDebugMode)
    {
        Mat imgDesign = fileOpenInit(DesignPathList.at(0));
        equalizeHist( imgDesign, imgDesign );
        threshold(imgDesign,imgDesign,30,255,THRESH_BINARY/*_INV*/);
    //    imgDesign = imgFeatureTool::imgFeatureThreshold(imgDesign,2,THRESH_BINARY);

        designMat = imgDesign.clone();
        QImage showIMG = imageFormat::MatToQImage(designMat);
        signalrimg1(QPixmap::fromImage(showIMG));
        signalrimg2(QPixmap::fromImage(showIMG));
    }
}
//图像生成参数
void basePara::slotGenChipDesignPara(int offsetType)
{
    scale = genChipDesignObj->scale;
    if(!showDebugMode)
    {
        if(!GenChipDesignisRun)
        {

            GenChipDesignisRun = true;
            startProgressViewId(0);
        }
    }else
    {
        complete();
        referpath = DesignPathList.at(0).left(DesignPathList.at(0).lastIndexOf("/")+1);
    }
}

void basePara::recPageIdx(int page)
{
    this->showPage = page;
    this->ui->stackedWidget->setCurrentIndex(page);
}

//图像生成完成
void basePara::complete()
{
    GenChipDesignisRun = false;
    signalComplete();
    signalSetLog(0,"图像生成完成");
}
//参考图展示
void basePara::showFisrtMat(Mat ist, QString path)
{
    imageFormat::mat2gray(ist);
    equalizeHist( ist, ist );
//    threshold(ist,ist,5,255,THRESH_BINARY_INV);
    designMat = ist.clone();
    QImage showIMG = imageFormat::MatToQImage(ist);
    signalrimg1(QPixmap::fromImage(showIMG));
    signalrimg2(QPixmap::fromImage(showIMG));
    //传递参考图路径
    referpath = path;
    emit deliverRefPath(referpath);
}
//运行进度
void basePara::slotProgress(int p)
{

}
void basePara::infraredOpenInit()
{
    rectList.clear();
    rect_lines.clear();
    FileReader_Thread *m_csvreader = new FileReader_Thread(this);
    QString dirpath = infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1);
    QStringList files = m_csvreader->readPath(dirpath);
    if(files.size() <= 0)
    {
        return;
    }
    QString filename = dirpath + files.at(0);
    m_csvreader->readfile(filename.toStdString());
    rows = m_csvreader->getSize().height();
    cols = m_csvreader->getSize().width();
    selectRect = Rect(0,0,cols,rows);
    signalSetLog(1,"infraRed Rect:"+QString("(x%1,y%2)").arg(cols).arg(rows));
    signalInfraredRect(QRect(0,0,cols,rows));//传输实际图尺寸
    initial = m_csvreader->readFileToMat(filename.toStdString());
    //    featureTool->rotateIamge(initial,initial,90);    //旋转90度
    imageFormat::mat2gray(initial);
    showMat = initial.clone();
//    imgFeatureTool::img2ALCHE(initial,alche);
    //canny
    //    alche = featureTool->imgCanny(alche,80,80);
    QImage showIMG = imageFormat::MatToQImage(initial);
    signallimg1(QPixmap::fromImage(showIMG));
    signallimg2(QPixmap::fromImage(showIMG));
    //传递红外文件给图像矫正
//    signalInfraredMat(initial);
}

Mat basePara::fileOpenInit(QString path)
{
    Mat fileMat;
    FileReader_Thread *fileReaderTool = new FileReader_Thread();
    if(path.endsWith(".csv") || path.endsWith(".xls") || path.endsWith(".xlsx"))
    {
        fileReaderTool->readfile(path.toStdString());
        fileMat = fileReaderTool->readFileToMat(path.toStdString());

        imageFormat::mat2gray(fileMat);
//        imshow(path.toStdString(),fileMat);
        signalSetLog(2,"design Rect:"+QString("(x%1,y%2)").arg(fileReaderTool->getSize().width()).arg(fileReaderTool->getSize().height()));
//        imwrite(path.toStdString()+".jpg",fileMat);
    }else
    {
        fileMat = imread(path.toStdString(),CV_BGR2GRAY);
    }
    //红外图像判断
    if(infraredPathList.size() > 0)
    {
        if(infraredPathList.at(0) == path)
        {
            initial = fileMat.clone();
            showMat = initial.clone();
            selectRect = Rect(0,0,initial.cols,initial.rows);
        }
    }
    return fileMat;
}
void basePara::slotReduct()
{
    showMat = initial.clone();
    selectRect = Rect(0,0,initial.cols,initial.rows);
    QImage showIMG = imageFormat::MatToQImage(showMat);
    signallimg1(QPixmap::fromImage(showIMG));
    SignalCapScreen1(-1);
}

void basePara::slotReduct2()
{
    showMat = initial.clone();
    selectRect = Rect(0,0,initial.cols,initial.rows);
    QImage showIMG = imageFormat::MatToQImage(showMat);
    signallimg2(QPixmap::fromImage(showIMG));
    SignalCapScreen2(-1);
}

void basePara::slotImgSelect(int step)
{
    SignalCapScreen2(step);//0禁用框选1红外图像选区2参考图像3删除图像
    if(step == 3)
    {
        imageRotateObj->toolBoxObj->setCurrentIndex(0);
    }
}

void basePara::slotDesignSelect(int step)
{
    SignalDesignScreen(step);//0禁用框选1红外图像选区2参考图像3删除图像
}
void basePara::slotSaveRect()
{
    rectList.push_back(selectRect);
    selectRect = Rect(0,0,initial.cols,initial.rows);
    Mat show = initial.clone();
    cvtColor(show,show,CV_GRAY2BGR);
    for(int i = 0; i < rectList.size(); i++)
    {
        rectangle(show, rectList.at(i), Scalar(255, 255, 0), 2);
    }
    QImage showIMG = imageFormat::MatToQImage(show);
    signallimg1(QPixmap::fromImage(showIMG));
}
//计算偏移
void basePara::slotComputer()
{
    if(rectList.size() < 2)
    {
        //弹窗阻止
        return;
    }
    QString dirpath = infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1);
    startProgressViewId(1);
    imgCropThread->setRedress(QSize(cols,rows),rectList,dirpath);
    imgCropThread->start();
}
//图像对齐的偏移计算
void basePara::showComputer()
{
    int index=0;
    for(Point p:imgCropThread->getResultList())
    {
        if(p.x != 0 || p.y != 0)
        {
            signalSetLog(1,u8"发现一个图像偏移："+QString::number(p.x)+"，"+QString::number(p.y));
            index++;
        }
    }
    signalSetLog(1,u8"本次检测发现偏移图像的数量："+QString::number(index));
    iscomplete = true;
    slotImgCrop();//计算后开始裁剪
}

void basePara::slotImgCrop()
{
    signalSetLog(1,u8"开始红外偏移裁剪");
    if(iscomplete)
    {
        imgCropThread->DoCrop();
        imgCropThread->start();
    }
}

void basePara::workOver()
{
    //完成图像裁剪
    signalSetLog(1,"完成红外图像裁剪");
    QString dirpath = infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1)+"result/";
    signalSetLog(1,"结果保存于:"+dirpath);
    FileReader_Thread *m_csvreader = new FileReader_Thread(this);
    QStringList files = m_csvreader->readPath(dirpath,"*.csv");
    QString filename = dirpath + files.at(0);
    m_csvreader->readfile(filename.toStdString());
    Mat infraredMat = m_csvreader->readFileToMat(filename.toStdString());
    //传递红外文件给图像矫正
//    signalInfraredMat(infraredMat);
    //传递给图像降噪
    imageFormat::mat2gray(infraredMat);
    initial = infraredMat.clone();
    showMat = initial.clone();
    rows = initial.rows;
    cols = initial.cols;
    selectRect = Rect(0,0,cols,rows);

    //传递结果和文件生成路径给降噪
    qDebug()<<dirpath;
    signalInfraredDate(dirpath,QPixmap::fromImage(imageFormat::MatToQImage(infraredMat)));

    signalSetModule(2);
    completeImgCorrection();
    completeViewId(1);
    jumpViewId(2);
}

void basePara::SlotSelectRect(Rect rect)
{
    selectRect = Rect(selectRect.x+rect.x, selectRect.y+rect.y, rect.width, rect.height);
    showMat = initial(selectRect);
    QImage showIMG = imageFormat::MatToQImage(showMat);
    signallimg1(QPixmap::fromImage(showIMG));
}
void basePara::SlotSelectRect2(Rect rect)
{
    selectRect = Rect(selectRect.x+rect.x, selectRect.y+rect.y, rect.width, rect.height);
    showMat = initial(selectRect);
    QImage showIMG = imageFormat::MatToQImage(showMat);
    signallimg2(QPixmap::fromImage(showIMG));
    imageRotateObj->toolBoxObj->setCurrentIndex(1);
}
void basePara::slotRunBinarizaType(int bt)
{
    BinarizaType = bt;
    setAutoThresholdSetting(resultMat);

    QImage showIMG = imageFormat::MatToQImage(resultMat);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::slotSetThreasholdType(int tt)
{
    ThreasholdType = tt;
    if(resultMat.empty())
    {
        return;
    }
    setAutoThresholdSetting(resultMat);

    QImage showIMG = imageFormat::MatToQImage(resultMat);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::slotRunThreashold(int t)
{
    Threashold = t;
    imgThreshold(resultMat,Threashold,ThreasholdType);

    QImage showIMG = imageFormat::MatToQImage(resultMat);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::setAutoThresholdSetting(Mat& thresh)
{
    thresh = imgFeatureTool::imgFeatureThreshold(showMat,BinarizaType,ThreasholdType);
}

void basePara::imgThreshold(Mat &mat, int threshold, int type)
{
    cv::threshold(showMat, mat, threshold, 255, type);
    imgFeatureTool::removeIsland(mat,mat);
}

void basePara::slotMinLength(int ml)
{
    MinLength = ml;
//    houghLineFeature(resultMat,MinLength,MaxGap,MinVote);
}

void basePara::slotMaxGap(int mg)
{
    MaxGap = mg;
//    houghLineFeature(resultMat,MinLength,MaxGap,MinVote);
}

void basePara::slotMinVote(int mv)
{
    MinVote = mv;
//    houghLineFeature(resultMat,MinLength,MaxGap,MinVote);
}

void basePara::houghLineFeature(Mat mat, int minlength, int maxgap, int minvote)
{
    temp_houghLines.clear();
    if(resultMat.empty())
    {
        signalSetLog(2,"图像未处理");
        return;
    }
    lineFinderTool->setAccResolution(1,CV_PI / 2);
    lineFinderTool->setLengthAndGap(minlength,maxgap);
    lineFinderTool->setminVote(minvote);
    temp_houghLines = lineFinderTool->findLines(mat);

    Mat HoughLine;
    cvtColor(mat,HoughLine,COLOR_GRAY2BGR);
    for (int i = 0, steps = temp_houghLines.size(); i < steps; i++)
    {
        line(HoughLine,Point(temp_houghLines[i][0], temp_houghLines[i][1]), Point(temp_houghLines[i][2], temp_houghLines[i][3]), Scalar(50, 255, 200), 1, 8);
    }

    QImage showIMG = imageFormat::MatToQImage(HoughLine);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::slotAutoLine()
{
    Vec4i conPoints;
    imgFeatureTool::getContourRect(resultMat, conPoints);
    temp_hough_rect = Rect(conPoints[0],conPoints[1],conPoints[2]-conPoints[0],conPoints[3]-conPoints[1]);
    Mat HoughLine;
    cvtColor(resultMat,HoughLine,COLOR_GRAY2BGR);
    rectangle(HoughLine, temp_hough_rect, Scalar(0, 255, 255), 2);

    QImage showIMG = imageFormat::MatToQImage(HoughLine);
    signallimg2(QPixmap::fromImage(showIMG));
}

void basePara::slotAggregateLine()
{
    QString rectLinesInfo;
    for(Rect rect: rect_lines)
    {
        rectLinesInfo += QString::number(rect.x)+","+QString::number(rect.y)+","+QString::number(rect.width)+","+QString::number(rect.height) + '\r\n';
    }
    //展示直线
    Mat rectangle_mat;
    cvtColor(initial,rectangle_mat,CV_GRAY2BGR);
    int linegroup_count = 0;
    for(Rect r : rect_lines)
    {
        signalSetLog(2,"-----红外实际特征坐标-------");
        signalSetLog(2,QString::number(linegroup_count)+":"+QString("Rect(x%1,y%2,w%3,h%4)").arg(r.x).arg(r.y).arg(r.width).arg(r.height));
        if(linegroup_count++ == 0)
        {
            rectangle(rectangle_mat, r, Scalar(255, 255, 0), 2);
        }else
        {
            rectangle(rectangle_mat, r, Scalar(0, 255, 255), 2);
        }
    }
    QImage showIMG = imageFormat::MatToQImage(rectangle_mat);
    signallimg2(QPixmap::fromImage(showIMG));

    signalSetLog(2,"直线特征已获取");
    imageRotateObj->toolBoxObj->setCurrentIndex(3);
}
//保存直线
void basePara::slotSaveLine()
{

    Rect r=Rect(temp_hough_rect.x+ selectRect.x,temp_hough_rect.y+ selectRect.y,temp_hough_rect.width,temp_hough_rect.height);
    rect_lines.push_back(r);

    //展示直线
    Mat HoughLine;
    cvtColor(initial,HoughLine,CV_GRAY2BGR);

    rectangle(HoughLine, r, Scalar(0, 255, 255), 2);
    QPixmap showIMG = QPixmap::fromImage(imageFormat::MatToQImage(HoughLine));
    signallimg2(showIMG);
    selectRect = Rect(0,0,initial.cols,initial.rows);

}

//发送降噪开始信号
void basePara::receiveFromBasePara(bool checkbox, int pointCount, int  threadCount)
{
    //现将数据传送给降噪类

    // 再传递开始信号

}
void basePara::testCheck(int arg1)
{

}

void basePara::slotSetLog(int i, QString s)
{
    signalSetLog(i, s);
}
void basePara::slotMatch()
{
    scale = genChipDesignObj->scale;
    //-----------------------------------按框选对齐测试------------------------------------------
    QPoint offset = imgFeatureTool::match(rect_lines,reference_lines);

    signalSetLog(2,"当前对比特征  参考图-实际特征的坐标差值:"+QString("Point(x%1,y%2)").arg(offset.x()).arg(offset.y()));
    Mat matchImg;
    cvtColor(initial,matchImg,CV_GRAY2BGR);
    //    ui->label_messg->setText("对齐后的各直线坐标：\n");
    int i = 1;
//    if(!lineIsHorizon)
//    {
        vector<double> point_x;
        for(Rect r : rect_lines)
        {
            if(i == 1)
            {
                rectangle(matchImg,Rect(r.x + offset.x(), r.y + offset.y(), r.width, r.height),Scalar(255 , 240 , 0), 2);
            }else
            {
                rectangle(matchImg,Rect(r.x + offset.x(), r.y + offset.y(), r.width, r.height),Scalar(0, 102, 255), 2);
            }
            putText(matchImg,to_string(i), Point(r.x + offset.x() - 10, r.y + offset.y() - 10),FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 240, 235), 1);
            //        ui->label_messg->setText(ui->label_messg->text()+QString::number(i)+":"+QString::number(r.x + offset.x())+","+QString::number(r.y + offset.y())+","+QString::number(r.x + offset.x() + r.width)+","+QString::number(r.y + offset.y() + r.height)+'\n');
//            qDebug()<<"point-"+QString::number(i)+":"<<QRect((r.x + offset.x()),(r.y + offset.y()),(r.x + offset.x() + r.width),(r.y + offset.y() + r.height));
            i++;
            point_x.push_back(r.x + offset.x() + r.width / 2);
        }
        sort(point_x.begin(),point_x.end());
        pointlist_x.push_back(point_x);
        pointlist.push_back(point_x);
        referenceList.push_back(reference_lines);
        if(pointlist_x.size() == 2)
        {
            int size = rect_lines.size();
            double designX1 = referenceList[1].x + referenceList[1].width/2 - (referenceList[0].x + referenceList[0].width/2);
            double realX1 = rect_lines[0].x + rect_lines[0].width/2 - (rect_lines[size-1].x + rect_lines[size-1].width/2);
            resultX = designX1 / realX1;
            double designY1 = referenceList[1].y + referenceList[1].height/2 - (referenceList[0].y + referenceList[0].height/2);
            double realY1 = rect_lines[0].y + rect_lines[0].height/2 - (rect_lines[size-1].y + rect_lines[size-1].height/2);
            resultY = designY1 / realY1;

            signalSetLog(2,"两特征相对距离：x:"+QString("%1,%2").arg(designX1).arg(realX1));
            signalSetLog(2,"两特征相对距离：y:"+QString("%1,%2").arg(designY1).arg(realY1));
            signalSetLog(2,"比例值:x:" + QString::number(resultX*scale));
            signalSetLog(2,"比例值:y:" + QString::number(resultY*scale));
            completeViewId(2);
            jumpViewId(0);
            //若图像比例对齐，则先裁剪图像再跳转图像偏移
            //需要的数据：红外图特征的坐标、设计图特征的坐标、红外图特征与设计图特征最小坐标判断、两图特征相对位置判断（作差）、
            //|红外特征左上坐标-设计特征左上坐标|=两图相对距离差mmx、mmy。offset_xy=两图相对距离差-红外特征坐标、两图相对距离差-设计特征坐标
            //imgFeatureTool::imageTranslation(m,offset_x,offset_y,mmx,mmy);
            ir_xstart = 0,ir_xend = initial.cols,de_xstart = 0,de_xend = designMat.cols;
            ir_ystart = 0,ir_yend = initial.rows,de_ystart = 0,de_yend = designMat.rows;
            if(offset.x()<0)
            {
                ir_xstart = abs(offset.x());
            }else
            {
                de_xstart = offset.x();
            }
            if(offset.y()<0)
            {
                ir_ystart = abs(offset.y());
            }else
            {
                de_ystart = offset.y();
            }
            int x_size_ir = ir_xend-ir_xstart; //对齐后实际图x大小
            int x_size_de = de_xend-de_xstart; //对齐后参考图x大小
            int x_size_diff = x_size_de - x_size_ir; //对齐后参考-实际图x差异

            int y_size_ir = ir_yend-ir_ystart; //对齐后实际图y大小
            int y_size_de = de_yend-de_ystart; //对齐后实际图y大小
            int y_size_diff = y_size_de - y_size_ir;//对齐后参考-实际图y差异

            if(x_size_diff<0)
            {
                ir_xend = ir_xend-abs(x_size_diff);
            }else
            {
                de_xend = de_xend-abs(x_size_diff);
            }
            if(y_size_diff<0)
            {
                ir_yend = ir_yend-abs(y_size_diff);
            }else
            {
                de_yend = de_yend-abs(y_size_diff);
            }
            initial = initial(Range(ir_ystart,ir_yend), Range(ir_xstart,ir_xend));
            designMat = designMat(Range(de_ystart,de_yend), Range(de_xstart,de_xend));
            test1();
            completeAlign();

            signalSetLog(2,"比例值:" + QString::number(resultX*scale));
            signalSetLog(2,"比例值:" + QString::number(resultY*scale));
        }

        signalSetLog(2,"对齐匹配完成，正在切换目标");
        signalSetLog(2,"当前已计算对齐:" + QString::number(pointlist_x.size()) + "次");
        signalSetLog(2,"还需计算对齐:" + QString::number(rect_lines.size() - pointlist_x.size()) + "次");

    QPixmap showIMG = QPixmap::fromImage(imageFormat::MatToQImage(matchImg));
    signallimg2(showIMG);
    cutoverAims();
}

//选参考边
void basePara::SlotDesignRect(Rect MatRect)
{
    int x1 = MatRect.x;
    int y1 = MatRect.y;
    int x2 = MatRect.x + MatRect.width;
    int y2 = MatRect.y + MatRect.height;
    //选区内检测直线特征
    Mat design_select = designMat(MatRect);
    lineFinderTool->setAccResolution(1,CV_PI / 2);
    lineFinderTool->setLengthAndGap(20, 10);
    lineFinderTool->setminVote(40);
    vector<Vec4i> select_hough = lineFinderTool->findLines(design_select);
    //若阈值检测不到特征，则以新的阈值重新检测
    if(select_hough.size() == 0)
    {
        lineFinderTool->setLengthAndGap(10, 5);
        lineFinderTool->setminVote(30);
        select_hough = lineFinderTool->findLines(design_select);

        signalSetLog(2,"参考边特征正在重新检测1");
    }
    if(select_hough.size() == 0)
    {
        lineFinderTool->setLengthAndGap(5, 5);
        lineFinderTool->setminVote(20);
        select_hough = lineFinderTool->findLines(design_select);

        signalSetLog(2,"参考边特征正在重新检测2");
    }
    if(select_hough.size() == 0)
    {

        signalSetLog(2,"未检测到直线特征，请重选参考边");
        return;
    }
    vector<Vec4i> design_hough;
    for(Vec4i line : select_hough)
    {
        design_hough.push_back(Vec4i(line[0] + x1, line[1] + y1, line[2] + x1, line[3] + y1));
    }
    //聚合选区内直线特征，确定选区特征位置
    vector<Vec4i> select_lines;
    vector<Vec4i> temp_lines;
    select_lines = lineFinderTool->selectLine(design_hough,x1,y1,x2,y2);
    if(x2 - x1 > y2 - y1)
    {
        //判断选区特征为横线
        lineIsHorizon = true;
        for(Vec4i line : select_lines)
        {
            if(line[1] == line[3])
            {
                temp_lines.push_back(line);
            }
        }
    }else
    {
        //判断选区特征为竖线
        lineIsHorizon = false;
        for(Vec4i line : select_lines)
        {
            if(line[0] == line[2])
            {
                temp_lines.push_back(line);
            }
        }
    }
    //求矩形位置与大小,参考线
    reference_lines = lineFinderTool->line_rectangle(temp_lines);

    signalSetLog(2,"参考图特征坐标:"+QString("Rect(x%1,y%2,w%3,h%4)").arg(reference_lines.x).arg(reference_lines.y).arg(reference_lines.width).arg(reference_lines.height));
    Mat rectangle_mat;
    cvtColor(designMat,rectangle_mat,CV_GRAY2BGR);
    rectangle(rectangle_mat, reference_lines, Scalar(255, 255, 0), 2);
    QImage showIMG = imageFormat::MatToQImage(rectangle_mat);
    signalrimg2(QPixmap::fromImage(showIMG));

    signalSetLog(2,"参考边特征已获取");
}

//删除直线deleteLine
void basePara::SlotDeleteRect(Rect rect)
{
    lineFinderTool->setLine(hough_lines);
    hough_lines = lineFinderTool->delete_line(rect.x,rect.y,rect.x+rect.width,rect.y+rect.height);
    //展示直线
    Mat HoughLine;
    cvtColor(initial,HoughLine,CV_GRAY2BGR);
    for (int i = 0, steps = hough_lines.size(); i < steps; i++)
    {
        line(HoughLine,Point(hough_lines[i][0], hough_lines[i][1]), Point(hough_lines[i][2], hough_lines[i][3]), Scalar(50, 255, 200), 1, 8);
    }
    QImage showIMG = imageFormat::MatToQImage(HoughLine);
    signallimg2(QPixmap::fromImage(showIMG));
}

//切换目标
void basePara::cutoverAims()
{
    //移位
    vector<Rect> newRectLines;
    for(int i = 1; i < rect_lines.size(); i++)
    {
        newRectLines.push_back(rect_lines.at(i));
    }
    newRectLines.push_back(rect_lines.at(0));
    rect_lines = newRectLines;

    //展示直线
    Mat rectangle_mat;
    cvtColor(initial,rectangle_mat,CV_GRAY2BGR);
    int linegroup_count = 0;
    for(Rect r : rect_lines)
    {
        if(++linegroup_count == 1)
        {
            rectangle(rectangle_mat, r, Scalar(255, 255, 0), 2);
        }else
        {
            rectangle(rectangle_mat, r, Scalar(0, 255, 255), 2);
        }
    }
    QPixmap showIMG = QPixmap::fromImage(imageFormat::MatToQImage(rectangle_mat));
    signallimg2(showIMG);
    //计算结果
    if(pointlist_x.size() == 2 || pointlist_y.size() == 2)
    {
        signalrimg2(showIMG);

        Mat rectangle_mat;
        cvtColor(initial,rectangle_mat,CV_GRAY2BGR);
        int linegroup_count = 0;
        for(Rect r : rect_lines)
        {
            if(linegroup_count++ == 0)
            {
                rectangle(rectangle_mat, r, Scalar(255, 255, 0), 2);
            }else
            {
                rectangle(rectangle_mat, r, Scalar(0, 255, 255), 2);
            }
        }
        QImage showLiftIMG = imageFormat::MatToQImage(rectangle_mat);
        signallimg2(QPixmap::fromImage(showLiftIMG));
        signalSetModule(1);

        pointlist_x.clear();
        referenceList.clear();
        pointlist_y.clear();
//        signalDesignPixmap(QPixmap::fromImage(imageFormat::MatToQImage(designMat)));//传递给图像降噪
    }
}
void basePara::completeAlign()
{
    completeViewId(2);
    QMessageBox msgBox;
    msgBox.setText(QString("图像对齐完成。\n计算比例值为:%1,%2.\n建议缩放比例:%3,%4.").arg(resultX).arg(resultY).arg(resultX*scale).arg(resultY*scale));
    msgBox.setInformativeText("是否跳转图像偏移？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Yes:
        referenceResultDate(referpath,designMat);

        jumpViewId(4);
        break;
    case QMessageBox::No:
        jumpViewId(1);
        break;
    default:
        // should never be reached
        break;
    }
}
void basePara::completeImgCorrection()
{
    completeViewId(1);
}
void basePara::test1()//红外图参考图批量裁剪
{
    signalSetLog(2,"开始图像裁剪");
    Mat testMat,testMat2;
    QList<Mat> fileMatList;
    FileReader_Thread *fileReader = new FileReader_Thread();
    QStringList ref_MatList = fileReader->readPath(referpath);

    if(DesignPathList.size() > 0)
    {
        ref_MatList = DesignPathList;//
        referpath="";//
    }
    QDir DebugPath;
    DebugPath.mkdir(DesignPathList.at(0).left(DesignPathList.at(0).lastIndexOf("/")+1) + "cut/");//241024新建裁减后文件夹

    for(QString path:ref_MatList)
    {
        signalSetLog(2,"参考图像裁剪路径："+referpath+path);
        fileReader->readfile((referpath+path).toStdString());
        Mat tempMat;
        tempMat = fileReader->readFileToMat((referpath+path).toStdString());
        tempMat = tempMat(Range(de_ystart,de_yend), Range(de_xstart,de_xend));

        path.chop(4);//
        path += "_cut.csv";//
        imageFormat::writeCSV((referpath+path).toStdString(),tempMat);
        testMat = tempMat.clone();
        emit deliverRefPath(path.left(path.lastIndexOf("/")+1)+"cut/");//241024，传路径
//        emit deliverRefPath(referpath+path);//241024，只传文件，没传路径
    }

    QStringList ir_MatList = fileReader->readPath(infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1)+"result/");//
//    ir_MatList = infraredPathList;//
    for(QString s:ir_MatList)
    {
        s = infraredPathList.at(0).left(infraredPathList.at(0).lastIndexOf("/")+1)+"result/"+s;

        signalSetLog(2,"红外采样图像裁剪路径："+s);
        fileReader->readfile((s).toStdString());
        Mat tempMat;
        tempMat = fileReader->readFileToMat((s).toStdString());
        tempMat = tempMat(Range(ir_ystart,ir_yend), Range(ir_xstart,ir_xend));

//        s.chop(4);//
//        s += "_cut.csv";//
        imageFormat::writeCSV((s).toStdString(),tempMat);
        testMat2 = tempMat.clone();
    }
    imageFormat::mat2gray(testMat);
    imageFormat::mat2gray(testMat2);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(testMat, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE );
    Mat linePic = Mat::zeros(testMat.rows, testMat.cols, CV_8UC3);
    for (int index = 0; index < contours.size(); index++){
        drawContours(linePic, contours, index, Scalar(0, 0, 255), 1, 8, hierarchy);
    }
//    imshow("1",linePic);
    cvtColor(testMat2,testMat2,CV_GRAY2BGR);
    addWeighted(linePic, 0.4, testMat2, 0.6, 0, linePic);
//    imshow("test",linePic);
}
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
