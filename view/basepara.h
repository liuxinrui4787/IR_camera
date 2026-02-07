<<<<<<< HEAD
﻿#ifndef BASEPARA_H
#define BASEPARA_H

#include <QWidget>
#include "computeredress.h"
#include "genchipdesignpara.h"
#include "imagecropthreadinfo.h"
#include "imagerotatepara.h"

namespace Ui {
class basePara;
}

class basePara : public QWidget
{
    Q_OBJECT

public:
    explicit basePara(QWidget *parent = nullptr);
    ~basePara();
public:
    QWidget *A;
    genChipDesignPara *genChipDesignObj;
    computeRedress *computeRedressObj;
    imageRotatePara *imageRotateObj;
    QString referpath;
    Mat initial,designMat;
signals:
    void referencePath(QString);
    void signalComplete();
    void signallimg1(QPixmap);
    void signalrimg1(QPixmap);
    void signallimg2(QPixmap);
    void signalrimg2(QPixmap);
    void startProgressViewId(int);
    void SignalCapScreen1(int);
    void SignalCapScreen2(int);
    void SignalDesignScreen(int);

    void completeViewId(int);
    void jumpViewId(int);
    void signalSetModule(int);
    void signalInfraredDate(QString,QPixmap);
    void signalInfraredRect(QRect rect);//传输实际图尺寸
    void denStartSignal(int);
    void deliverRefPath(QString);
    void deliverDenPathSignal(QString);
    void referenceResultDate(QString path,Mat design);
    void irPosition(int,int,int,int);
    void dePosition(int,int,int,int);
    void signalSetLog(int,QString);
private slots:
    void recPageIdx(int);
    void slotinfraredFile(QStringList s);
    void slotDesignFilePath(QStringList s);

    void slotGenChipDesignPara(int offsetType);

    void complete();

    void showFisrtMat(Mat ist,QString path);

    void slotProgress(int);

    void slotReduct();
    void slotSaveRect();
    void slotComputer();
    void showComputer();
    void slotImgCrop();
    void workOver();
    void SlotSelectRect(cv::Rect rect);
    void slotRunBinarizaType(int bt);
    void slotSetThreasholdType(int tt);
    void slotRunThreashold(int t);
    void slotReduct2();
    void slotImgSelect(int step);
    void slotDesignSelect(int step);
    void slotMatch();
    void SlotSelectRect2(cv::Rect rect);
    void SlotDesignRect(cv::Rect MatRect);
    void SlotDeleteRect(cv::Rect rect);
    void slotMinLength(int ml);
    void slotMaxGap(int mg);
    void slotMinVote(int mv);
    void slotAutoLine();
    void slotAggregateLine();
    void slotSaveLine();

    void receiveFromBasePara(bool,int,int);
    void testCheck(int arg1);
    void slotSetLog(int,QString);

private:
    Ui::basePara *ui;
    int showPage;
    void init();
    QList<cv::Rect> rectList;
    cv::Rect selectRect;
    cv::Mat showMat,resultMat;

    LineFinder *lineFinderTool;
    ImageCropThreadInfo *imgCropThread;
    bool GenChipDesignisRun=false;
    bool iscomplete=false;
    QStringList infraredPathList,DesignPathList;
    int rows, cols;
    int scale=1500;
    int BinarizaType=0, ThreasholdType=0, Threashold=0;
    int MinLength=0, MaxGap=0, MinVote=0;
    double correctAngle = 0.0;
    vector<Rect> rect_lines;
    vector<Vec4i> hough_lines;
    vector<Vec4i> temp_houghLines;
    cv::Rect reference_lines;
    cv::Rect temp_hough_rect;
    vector<Rect> referenceList;
    std::vector<std::vector<double>> pointlist_x;
    std::vector<std::vector<double>> pointlist_y;
    std::vector<std::vector<double>> pointlist;
    bool lineIsHorizon;
    double resultX=0,resultY=0;
    int ir_xstart,ir_xend,de_xstart,de_xend;
    int ir_ystart,ir_yend,de_ystart,de_yend;

    void infraredOpenInit();
    void setAutoThresholdSetting(Mat &thresh);
    void imgThreshold(Mat &mat, int threshold, int type);
    void houghLineFeature(Mat mat, int minlength, int maxgap, int minvote);
    void cutoverAims();
    Mat fileOpenInit(QString path);
    int ret;
    void completeAlign();
    void completeImgCorrection();
    void test1();
    bool showDebugMode=true;
};

#endif // BASEPARA_H
=======
﻿#ifndef BASEPARA_H
#define BASEPARA_H

#include <QWidget>
#include "computeredress.h"
#include "genchipdesignpara.h"
#include "imagecropthreadinfo.h"
#include "imagerotatepara.h"

namespace Ui {
class basePara;
}

class basePara : public QWidget
{
    Q_OBJECT

public:
    explicit basePara(QWidget *parent = nullptr);
    ~basePara();
public:
    QWidget *A;
    genChipDesignPara *genChipDesignObj;
    computeRedress *computeRedressObj;
    imageRotatePara *imageRotateObj;
    QString referpath;
    Mat initial,designMat;
signals:
    void referencePath(QString);
    void signalComplete();
    void signallimg1(QPixmap);
    void signalrimg1(QPixmap);
    void signallimg2(QPixmap);
    void signalrimg2(QPixmap);
    void startProgressViewId(int);
    void SignalCapScreen1(int);
    void SignalCapScreen2(int);
    void SignalDesignScreen(int);

    void completeViewId(int);
    void jumpViewId(int);
    void signalSetModule(int);
    void signalInfraredDate(QString,QPixmap);
    void signalInfraredRect(QRect rect);//传输实际图尺寸
    void denStartSignal(int);
    void deliverRefPath(QString);
    void deliverDenPathSignal(QString);
    void referenceResultDate(QString path,Mat design);
    void irPosition(int,int,int,int);
    void dePosition(int,int,int,int);
    void signalSetLog(int,QString);
private slots:
    void recPageIdx(int);
    void slotinfraredFile(QStringList s);
    void slotDesignFilePath(QStringList s);

    void slotGenChipDesignPara(int offsetType);

    void complete();

    void showFisrtMat(Mat ist,QString path);

    void slotProgress(int);

    void slotReduct();
    void slotSaveRect();
    void slotComputer();
    void showComputer();
    void slotImgCrop();
    void workOver();
    void SlotSelectRect(cv::Rect rect);
    void slotRunBinarizaType(int bt);
    void slotSetThreasholdType(int tt);
    void slotRunThreashold(int t);
    void slotReduct2();
    void slotImgSelect(int step);
    void slotDesignSelect(int step);
    void slotMatch();
    void SlotSelectRect2(cv::Rect rect);
    void SlotDesignRect(cv::Rect MatRect);
    void SlotDeleteRect(cv::Rect rect);
    void slotMinLength(int ml);
    void slotMaxGap(int mg);
    void slotMinVote(int mv);
    void slotAutoLine();
    void slotAggregateLine();
    void slotSaveLine();

    void receiveFromBasePara(bool,int,int);
    void testCheck(int arg1);
    void slotSetLog(int,QString);

private:
    Ui::basePara *ui;
    int showPage;
    void init();
    QList<cv::Rect> rectList;
    cv::Rect selectRect;
    cv::Mat showMat,resultMat;

    LineFinder *lineFinderTool;
    ImageCropThreadInfo *imgCropThread;
    bool GenChipDesignisRun=false;
    bool iscomplete=false;
    QStringList infraredPathList,DesignPathList;
    int rows, cols;
    int scale=1500;
    int BinarizaType=0, ThreasholdType=0, Threashold=0;
    int MinLength=0, MaxGap=0, MinVote=0;
    double correctAngle = 0.0;
    vector<Rect> rect_lines;
    vector<Vec4i> hough_lines;
    vector<Vec4i> temp_houghLines;
    cv::Rect reference_lines;
    cv::Rect temp_hough_rect;
    vector<Rect> referenceList;
    std::vector<std::vector<double>> pointlist_x;
    std::vector<std::vector<double>> pointlist_y;
    std::vector<std::vector<double>> pointlist;
    bool lineIsHorizon;
    double resultX=0,resultY=0;
    int ir_xstart,ir_xend,de_xstart,de_xend;
    int ir_ystart,ir_yend,de_ystart,de_yend;

    void infraredOpenInit();
    void setAutoThresholdSetting(Mat &thresh);
    void imgThreshold(Mat &mat, int threshold, int type);
    void houghLineFeature(Mat mat, int minlength, int maxgap, int minvote);
    void cutoverAims();
    Mat fileOpenInit(QString path);
    int ret;
    void completeAlign();
    void completeImgCorrection();
    void test1();
    bool showDebugMode=true;
};

#endif // BASEPARA_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
