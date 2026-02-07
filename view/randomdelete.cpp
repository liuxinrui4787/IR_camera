#include "randomdelete.h"
#include "components/qtmaterialstyle.h"
#include "ui_randomdelete.h"
#include <QRandomGenerator>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

randomDelete::randomDelete(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::randomDelete)
{
    ui->setupUi(this);
    readDirPath();
    this->ui->btnOpen->setColor(QtMaterialStyle::instance().themeColor("primary1"));
    this->ui->btnOpen->setIcon(QIcon(":/image/btn_search.svg"));
    this->ui->btnOpen->setIconSize(QSize(30, 30));
}

randomDelete::~randomDelete()
{
    delete ui;
}

void randomDelete::on_btnOpen_clicked()
{
    tempFiles = QFileDialog::getOpenFileNames
            (this, "选择输入图像文件", ui->edit_file->document()->toPlainText(), tr("(*.csv *.xls *.xlsx)"));
    if(!tempFiles.isEmpty())
    {
        fileDir = tempFiles.at(0);
        int index = fileDir.lastIndexOf("/");
        fileDir.truncate(index+1);
        qDebug()<<fileDir;
        SetDirPath(fileDir);
        fileOpenInit(tempFiles);
    }
}
bool randomDelete::checkDirExists(const QString &fp)
{
    //占比函数
    if (fp == QString::fromLocal8Bit(""))
    {
        return false;
    }
    if (!QDir(fp).exists())
    {
        return false;
    }
    ui->edit_file->setText(fp);
    return true;
}
void randomDelete::SetDirPath(const QString &file)
{
    ui->edit_file->setText(file);
    ui->edit_file->setAlignment(Qt::AlignBottom);
    ui->edit_file->moveCursor(QTextCursor::End);

    QSettings iniFile("./config.ini", QSettings::IniFormat);
    iniFile.beginGroup("randomDelete");
    iniFile.setValue("lastDirpath", file);
    iniFile.endGroup();
    iniFile.sync();
}
void randomDelete::readDirPath()
{
    QSettings iniFile("./config.ini", QSettings::IniFormat);
    iniFile.beginGroup("randomDelete");
    QString dirPath = iniFile.value("lastDirpath", "").toString();

    if(checkDirExists(dirPath))
    {
        //预加载内容
        fileDir = dirPath;
//        fileOpenInit(dirPath);多文件没做ini数据读取，现在不能用
    }
}
//预加载图像
void randomDelete::fileOpenInit(QStringList& pathList)
{
    FileReader_Thread *m_csvreader = new FileReader_Thread(this);
    std::sort(pathList.begin(), pathList.end(), [](const QString& s1, const QString& s2)
    {
        QString tmp_1, tmp_2;
        for (int j = 0; j < s1.length(); j++)
        {
            if (s1[j] >= '0' && s1[j] <= '9')
            {
                tmp_1.append(s1[j]);
            }
        }
        for (int j = 0; j < s2.length(); j++)
        {
            if (s2[j] >= '0' && s2[j] <= '9')
            {
                tmp_2.append(s2[j]);
            }
        }
        return tmp_1.toInt() < tmp_2.toInt();
    });

    QString filename = pathList.at(0);
    m_csvreader->readfile(filename.toStdString());
    Mat fileMat = m_csvreader->readFileToMat(filename.toStdString());
    initial = fileMat.clone();

    imageFormat::mat2gray(fileMat);
    QImage showIMG = imageFormat::MatToQImage(fileMat);
    signalRDImg(QPixmap::fromImage(showIMG));
//    SignalShowImg(QPixmap::fromImage(showIMG),0);//int 0-1 选择在指定面板显示图像
    signalSetLog(0,u8"偏移图像数据已载入");
}
void randomDelete::on_viewButton_clicked()
{
    FileReader_Thread *m_csvreader = new FileReader_Thread(this);
    m_csvreader->readfile(tempFiles.at(0).toStdString());
    Mat fileMat = m_csvreader->readFileToMat(tempFiles.at(0).toStdString());
    imageFormat::mat2gray(fileMat);
    //***二值化，需要修改为自适应调节的算法****
    threshold(fileMat,fileMat,0.5,255,CV_THRESH_BINARY);
    //***二值化，需要修改为自适应调节的算法****
    vector<vector<Point>> contour = imgFeatureTool::getRandomDeleteContour(fileMat);
    if(contour.size() < 1)
    {
        QMessageBox::warning(Q_NULLPTR, QString(u8"警告"),QString(u8"当前数据文件不支持随机删除"));
        return;
    }
    fileMat = initial.clone();

    randomList.clear();
    for(int t = 0; t < deletetimes; t++)
    {
        isRepeatPoint(fileMat,contour);
        randomList.push_back(cv::Point(x,y));
    }
    imageFormat::mat2gray(fileMat);
    cvtColor(fileMat,fileMat,CV_GRAY2BGR);
    signalSetLog(0,"randomList:"+QString::number(randomList.size()));
    for(int t = 0; t < randomList.size(); t++)
    {
        int x = randomList.at(t).x;
        int y = randomList.at(t).y;

        int radioId=0;
        if(ui->radioButton_1x1->isChecked())
        {
            radioId = 1;
        }else if(ui->radioButton_2x2->isChecked())
        {
            radioId = 2;
        }else if(ui->radioButton_3x3->isChecked())
        {
            radioId = 3;
        }
        else if(ui->radioButton_4x4->isChecked())
        {
            radioId = 4;
        }
        for(int a = 0; a < radioId; a++)
        {
            for(int b = 0; b < radioId; b++)
            {
                signalSetLog(0,"坐标点:"+QString::number(x+1+a)+","+QString::number(y+1+b)+"-已删除");
            }
        }
        signalSetLog(0,"---------------------------------------------");

        rectangle(fileMat, Rect(x-5,y-5,10,10), Scalar(255, 255, 0), 2);
    }

    QImage showIMG = imageFormat::MatToQImage(fileMat);
    signalRDImg(QPixmap::fromImage(showIMG));
}

void randomDelete::on_startButton_clicked()
{

}

cv::Point randomDelete::makeRandomPoint(vector<vector<Point>>& contour)
{
    int randomCont = QRandomGenerator::global()->bounded(0, contour.size());
    int randomPointID = QRandomGenerator::global()->bounded(0, contour.at(randomCont).size());

    return cv::Point(contour.at(randomCont)[randomPointID].x,contour.at(randomCont)[randomPointID].y);
}
bool randomDelete::isRepeatPoint(Mat &fileMat,vector<vector<Point>>& contour)
{

}

void randomDelete::on_spinBox_valueChanged(int arg1)
{
    deletetimes = arg1;
}
