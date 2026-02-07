<<<<<<< HEAD
﻿#include "mainwindow.h"
#include "components/qtmaterialiconbutton.h"
#include "components/qtmaterialstyle.h"
#include "module/generatereferencemap.h"
#include "ui_mainwindow.h"
#include "logpagetest.h"

MainWindow::MainWindow(QWidget *parent)
    : BaseWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    log->hide();
    this->init();

    ui->stepDebug4->hide();
    ui->stepDebug5->hide();
    ui->stepDebug6->hide();
    ui->stepDebug7->hide();

    ui->step4->hide();
    ui->step5->hide();
    ui->step6->hide();
    ui->step7->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QStringList icons = {u8"icon_pic_ref.svg", u8"icon_pic_cor.svg", u8"icon_pic_jiaozheng.svg"};
    QStringList names = {u8"图像生成", u8"图像对齐", u8" 图像矫正"};
    stepList = {this->ui->step1, this->ui->step2, this->ui->step3};
    for (int i = 0; i < stepList.length(); i++)
    {
        stepList.at(i)->setIcon(":/image/" + icons.at(i));
        stepList.at(i)->setText(names.at(i));
        stepList.at(i)->setWaiting(i + 1);
//        stepList[i]->setSuccess();
    }
//    stepList[6]->setLoading();

    this->ui->btnExit->setColor(QtMaterialStyle::instance().themeColor("primary1"));
    this->ui->btnExit->setIcon(QIcon(":/image/back_home.svg"));
    this->ui->btnExit->setIconSize(QSize(ui->btnExit->width(), ui->btnExit->height()));

    //设置结果文件按钮
//    this->ui->btnExit_2->setColor(QtMaterialStyle::instance().themeColor("primary1"));
//    this->ui->btnExit_2->setIcon(QIcon(":/image/btn_add.png"));
//    this->ui->btnExit_2->setText("显示结果");

    //将所有的模块添加进来

    this->ui->scrollDebug->setAttribute(Qt::WA_TranslucentBackground, true);
    this->ui->scrollDebugWidge->setAttribute(Qt::WA_TranslucentBackground, true);
    this->ui->scrollDebugWidge->setStyleSheet("background-color:transparent;");
    this->ui->scrollDebug->setStyleSheet("background-color:transparent;");
    //    this->ui->scrollDebug->setVisible(false);

    this->ui->stepDebug1->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug2->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug3->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug4->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug5->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug6->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug7->setBackgroundMode(Qt::TransparentMode);

    /********************信号连接代码***********************
    ***********************************************************/
    connect(this,SIGNAL(currentShowPage(int)),this->ui->page,SLOT(currentShowIdx(int)));

    connect(ui->page,SIGNAL(completeViewId(int)),this,SLOT(completeViewId(int)));
    connect(ui->page,SIGNAL(jumpViewId(int)),this,SLOT(jumpViewId(int)));
    connect(ui->page,SIGNAL(signalrimg(QPixmap)),ui->graphicsView_8,SLOT(slotrimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(completeViewId(int)),this,SLOT(completeViewId(int)));
    connect(ui->page->baseParaObj,SIGNAL(jumpViewId(int)),this,SLOT(jumpViewId(int)));
    connect(ui->page->baseParaObj,SIGNAL(startProgressViewId(int)),this,SLOT(startProgressViewId(int)));

    connect(ui->page->baseParaObj,SIGNAL(signalSetLog(int,QString)),this,SLOT(slotSetLog(int,QString)));
    connect(ui->page->randomDeleteObj,SIGNAL(signalSetLog(int,QString)),this,SLOT(slotSetLog(int,QString)));
    connect(this,SIGNAL(signalLog(int,QString)),log,SLOT(slotLog(int,QString)));//1025

    connect(ui->page->baseParaObj,SIGNAL(signalrimg1(QPixmap)),ui->graphicsView_8,SLOT(slotrimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(signalrimg1(QPixmap)),ui->graphicsView_2,SLOT(slotrimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(signalrimg2(QPixmap)),ui->graphicsView_2,SLOT(slotrimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(signallimg1(QPixmap)),ui->graphicsView,SLOT(slotlimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(signallimg2(QPixmap)),ui->graphicsView_3,SLOT(slotlimg(QPixmap)));

    connect(ui->page->baseParaObj->computeRedressObj,SIGNAL(signalImgSelect(int)),ui->graphicsView,SLOT(SlotCapScreen(int)));
    connect(ui->graphicsView,SIGNAL(SignalSelectRect(cv::Rect)),ui->page->baseParaObj,SLOT(SlotSelectRect(cv::Rect)));

    connect(ui->page->baseParaObj,SIGNAL(SignalCapScreen1(int)),ui->graphicsView,SLOT(SlotCapScreen(int)));
    connect(ui->page->baseParaObj,SIGNAL(SignalCapScreen2(int)),ui->graphicsView_3,SLOT(SlotCapScreen(int)));
    connect(ui->page->baseParaObj,SIGNAL(SignalDesignScreen(int)),ui->graphicsView_2,SLOT(SlotCapScreen(int)));
    connect(ui->graphicsView_2,SIGNAL(SignalDesignRect(cv::Rect)),ui->page->baseParaObj,SLOT(SlotDesignRect(cv::Rect)));
    connect(ui->graphicsView_3,SIGNAL(SignalSelectRect(cv::Rect)),ui->page->baseParaObj,SLOT(SlotSelectRect2(cv::Rect)));
    connect(ui->graphicsView_3,SIGNAL(SignalDeleteRect(cv::Rect)),ui->page->baseParaObj,SLOT(SlotDeleteRect(cv::Rect)));


    connect(ui->page->baseParaObj,SIGNAL(signalSetModule(int)),this,SLOT(onModuleSelect(int)));

    this->setDebug(true);

    GraphicsListL.push_back(ui->graphicsView);//图像对齐
    GraphicsListL.push_back(ui->graphicsView_3);//图像矫正
    GraphicsListL.push_back(ui->graphicsView_4);
    GraphicsListL.push_back(ui->graphicsView_5);
    GraphicsListL.push_back(ui->graphicsView_6);

    GraphicsListR.push_back(ui->graphicsView_2);//图像矫正
    GraphicsListR.push_back(ui->graphicsView_7);
    GraphicsListR.push_back(ui->graphicsView_8);//参考图

    showWindowL(0);
    showWindowR(0);

    onModuleSelect(0);
}

void MainWindow::onModuleSelect(int page)
{
    this->ui->leftMenu->setCurrentIndex(page);
    currentShowPage(page);
    this->ui->step1->setIsWaitForConfig(page == 0 ? true : false);
    this->ui->step2->setIsWaitForConfig(page == 1 ? true : false);
    this->ui->step3->setIsWaitForConfig(page == 2 ? true : false);
    this->ui->step4->setIsWaitForConfig(page == 3 ? true : false);
    this->ui->step5->setIsWaitForConfig(page == 4 ? true : false);
    this->ui->step6->setIsWaitForConfig(page == 5 ? true : false);
    this->ui->step7->setIsWaitForConfig(page == 6 ? true : false);
    if(page == 0)
    {
        showWindowL(0);
        showWindowR(2);
    }
    else if(page == 1)
    {
        showWindowL(0);
        showWindowR(2);
    }else if(page == 2)
    {
        showWindowL(1);
        showWindowR(0);
    }else if(page == 3)
    {
        showWindowL(2);
        showWindowR(0);
    }else if(page == 4)
    {
        if(ui->graphicsView_4->getPixmap().width()!=0){
//                showWindowL(2);
        }
        showWindowR(0);
    }else if(page == 5)
    {
        showWindowL(3);
        if(ui->graphicsView_5->getPixmap().width()==0){
            ui->graphicsView_5->setPixmap(ui->graphicsView_4->getPixmap());
            ui->graphicsView_5->showlimg(ui->graphicsView_4->getPixmap());
        }
        showWindowR(0);
    }else if(page == 6)
    {
        showWindowL(4);
        if(ui->graphicsView_6->getPixmap().width()==0){
            ui->graphicsView_6->setPixmap(ui->graphicsView_5->getPixmap());
            ui->graphicsView_6->showlimg(ui->graphicsView_5->getPixmap());
        }
        showWindowR(0);
    }

}

void MainWindow::setDebug(bool isDebug)
{
    this->ui->scrollDebug->setVisible(isDebug);
}

void MainWindow::on_stepDebug1_clicked()
{
    this->onModuleSelect(0);
}

void MainWindow::on_stepDebug2_clicked()
{
    this->onModuleSelect(1);
}

void MainWindow::on_stepDebug3_clicked()
{
    this->onModuleSelect(2);
}

//等待动画
void MainWindow::startProgressViewId(int id)
{
    for (int i = 0; i < stepList.length(); i++)
    {
        if(id == i)
        {
            stepList.at(i)->setLoading();
            break;
        }
    }
}

void MainWindow::completeViewId(int id)
{
    for (int i = 0; i < stepList.length(); i++)
    {
        if(id == i)
        {
            stepList.at(i)->setSuccess();
            break;
        }
    }
}

void MainWindow::jumpViewId(int id)
{
    onModuleSelect(id);
}


void MainWindow::showWindowL(int viewId)
{
    for(int i = 0; i < GraphicsListL.size(); i++)
    {
        if(i == viewId)
        {
            GraphicsListL[i]->setEnabled(true);
            GraphicsListL[i]->show();
        }else
        {
            GraphicsListL[i]->setEnabled(false);
            GraphicsListL[i]->hide();
        }
    }
}

void MainWindow::showWindowR(int viewId)
{
    for(int i = 0; i < GraphicsListR.size(); i++)
    {
        if(i == viewId)
        {
            GraphicsListR[i]->setEnabled(true);
            GraphicsListR[i]->show();
        }else
        {
            GraphicsListR[i]->setEnabled(false);
            GraphicsListR[i]->hide();
        }
    }
}

//实际图像显示
void MainWindow::actualParaImg(QPixmap pixmap)
{
    showWindowL(2);
    this->ui->graphicsView_4->slotlimg(pixmap);
    //    slotlimg(pixmap);
}

//分布图图像显示
void MainWindow::dismapImg(QPixmap  pixmap)
{
    showWindowL(4);
    this->ui->graphicsView_6->slotlimg(pixmap);
    //    slotlimg(pixmap);
}
//1025
void MainWindow::slotSetLog(int id,QString s)
{
    qDebug()<<s;
    signalLog(id,s);
}

//切换左侧侧边栏


void MainWindow::on_btnExit_2_clicked()
{
    log->show();
}
//显示降噪路径
void MainWindow::receiveDenFromcol(QString str)
{
    log->denTextBrowser->insertPlainText(str);
}
//显示随机路径
void MainWindow::receivedirectStrFrom(QString str)
{
    log->disinputBrowser->insertPlainText(str);
}
//显示 分布图结束文字
void MainWindow::receivePathFromConMain(QString  str)
{
    log->disparaBrowser->insertPlainText(str);
}
//2024年10月25日  接收异常点信息
void MainWindow::receiveYichangFromLog(QString str)
{
    qDebug()<<"1211"<<str;
    log->logBrowser->insertPlainText(str);
}
//显示分布图结果


=======
﻿#include "mainwindow.h"
#include "components/qtmaterialiconbutton.h"
#include "components/qtmaterialstyle.h"
#include "module/generatereferencemap.h"
#include "ui_mainwindow.h"
#include "logpagetest.h"

MainWindow::MainWindow(QWidget *parent)
    : BaseWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    log->hide();
    this->init();

    ui->stepDebug4->hide();
    ui->stepDebug5->hide();
    ui->stepDebug6->hide();
    ui->stepDebug7->hide();

    ui->step4->hide();
    ui->step5->hide();
    ui->step6->hide();
    ui->step7->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QStringList icons = {u8"icon_pic_ref.svg", u8"icon_pic_cor.svg", u8"icon_pic_jiaozheng.svg"};
    QStringList names = {u8"图像生成", u8"图像对齐", u8" 图像矫正"};
    stepList = {this->ui->step1, this->ui->step2, this->ui->step3};
    for (int i = 0; i < stepList.length(); i++)
    {
        stepList.at(i)->setIcon(":/image/" + icons.at(i));
        stepList.at(i)->setText(names.at(i));
        stepList.at(i)->setWaiting(i + 1);
//        stepList[i]->setSuccess();
    }
//    stepList[6]->setLoading();

    this->ui->btnExit->setColor(QtMaterialStyle::instance().themeColor("primary1"));
    this->ui->btnExit->setIcon(QIcon(":/image/back_home.svg"));
    this->ui->btnExit->setIconSize(QSize(ui->btnExit->width(), ui->btnExit->height()));

    //设置结果文件按钮
//    this->ui->btnExit_2->setColor(QtMaterialStyle::instance().themeColor("primary1"));
//    this->ui->btnExit_2->setIcon(QIcon(":/image/btn_add.png"));
//    this->ui->btnExit_2->setText("显示结果");

    //将所有的模块添加进来

    this->ui->scrollDebug->setAttribute(Qt::WA_TranslucentBackground, true);
    this->ui->scrollDebugWidge->setAttribute(Qt::WA_TranslucentBackground, true);
    this->ui->scrollDebugWidge->setStyleSheet("background-color:transparent;");
    this->ui->scrollDebug->setStyleSheet("background-color:transparent;");
    //    this->ui->scrollDebug->setVisible(false);

    this->ui->stepDebug1->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug2->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug3->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug4->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug5->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug6->setBackgroundMode(Qt::TransparentMode);
    this->ui->stepDebug7->setBackgroundMode(Qt::TransparentMode);

    /********************信号连接代码***********************
    ***********************************************************/
    connect(this,SIGNAL(currentShowPage(int)),this->ui->page,SLOT(currentShowIdx(int)));

    connect(ui->page,SIGNAL(completeViewId(int)),this,SLOT(completeViewId(int)));
    connect(ui->page,SIGNAL(jumpViewId(int)),this,SLOT(jumpViewId(int)));
    connect(ui->page,SIGNAL(signalrimg(QPixmap)),ui->graphicsView_8,SLOT(slotrimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(completeViewId(int)),this,SLOT(completeViewId(int)));
    connect(ui->page->baseParaObj,SIGNAL(jumpViewId(int)),this,SLOT(jumpViewId(int)));
    connect(ui->page->baseParaObj,SIGNAL(startProgressViewId(int)),this,SLOT(startProgressViewId(int)));

    connect(ui->page->baseParaObj,SIGNAL(signalSetLog(int,QString)),this,SLOT(slotSetLog(int,QString)));
    connect(ui->page->randomDeleteObj,SIGNAL(signalSetLog(int,QString)),this,SLOT(slotSetLog(int,QString)));
    connect(this,SIGNAL(signalLog(int,QString)),log,SLOT(slotLog(int,QString)));//1025

    connect(ui->page->baseParaObj,SIGNAL(signalrimg1(QPixmap)),ui->graphicsView_8,SLOT(slotrimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(signalrimg1(QPixmap)),ui->graphicsView_2,SLOT(slotrimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(signalrimg2(QPixmap)),ui->graphicsView_2,SLOT(slotrimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(signallimg1(QPixmap)),ui->graphicsView,SLOT(slotlimg(QPixmap)));
    connect(ui->page->baseParaObj,SIGNAL(signallimg2(QPixmap)),ui->graphicsView_3,SLOT(slotlimg(QPixmap)));

    connect(ui->page->baseParaObj->computeRedressObj,SIGNAL(signalImgSelect(int)),ui->graphicsView,SLOT(SlotCapScreen(int)));
    connect(ui->graphicsView,SIGNAL(SignalSelectRect(cv::Rect)),ui->page->baseParaObj,SLOT(SlotSelectRect(cv::Rect)));

    connect(ui->page->baseParaObj,SIGNAL(SignalCapScreen1(int)),ui->graphicsView,SLOT(SlotCapScreen(int)));
    connect(ui->page->baseParaObj,SIGNAL(SignalCapScreen2(int)),ui->graphicsView_3,SLOT(SlotCapScreen(int)));
    connect(ui->page->baseParaObj,SIGNAL(SignalDesignScreen(int)),ui->graphicsView_2,SLOT(SlotCapScreen(int)));
    connect(ui->graphicsView_2,SIGNAL(SignalDesignRect(cv::Rect)),ui->page->baseParaObj,SLOT(SlotDesignRect(cv::Rect)));
    connect(ui->graphicsView_3,SIGNAL(SignalSelectRect(cv::Rect)),ui->page->baseParaObj,SLOT(SlotSelectRect2(cv::Rect)));
    connect(ui->graphicsView_3,SIGNAL(SignalDeleteRect(cv::Rect)),ui->page->baseParaObj,SLOT(SlotDeleteRect(cv::Rect)));


    connect(ui->page->baseParaObj,SIGNAL(signalSetModule(int)),this,SLOT(onModuleSelect(int)));

    this->setDebug(true);

    GraphicsListL.push_back(ui->graphicsView);//图像对齐
    GraphicsListL.push_back(ui->graphicsView_3);//图像矫正
    GraphicsListL.push_back(ui->graphicsView_4);
    GraphicsListL.push_back(ui->graphicsView_5);
    GraphicsListL.push_back(ui->graphicsView_6);

    GraphicsListR.push_back(ui->graphicsView_2);//图像矫正
    GraphicsListR.push_back(ui->graphicsView_7);
    GraphicsListR.push_back(ui->graphicsView_8);//参考图

    showWindowL(0);
    showWindowR(0);

    onModuleSelect(0);
}

void MainWindow::onModuleSelect(int page)
{
    this->ui->leftMenu->setCurrentIndex(page);
    currentShowPage(page);
    this->ui->step1->setIsWaitForConfig(page == 0 ? true : false);
    this->ui->step2->setIsWaitForConfig(page == 1 ? true : false);
    this->ui->step3->setIsWaitForConfig(page == 2 ? true : false);
    this->ui->step4->setIsWaitForConfig(page == 3 ? true : false);
    this->ui->step5->setIsWaitForConfig(page == 4 ? true : false);
    this->ui->step6->setIsWaitForConfig(page == 5 ? true : false);
    this->ui->step7->setIsWaitForConfig(page == 6 ? true : false);
    if(page == 0)
    {
        showWindowL(0);
        showWindowR(2);
    }
    else if(page == 1)
    {
        showWindowL(0);
        showWindowR(2);
    }else if(page == 2)
    {
        showWindowL(1);
        showWindowR(0);
    }else if(page == 3)
    {
        showWindowL(2);
        showWindowR(0);
    }else if(page == 4)
    {
        if(ui->graphicsView_4->getPixmap().width()!=0){
//                showWindowL(2);
        }
        showWindowR(0);
    }else if(page == 5)
    {
        showWindowL(3);
        if(ui->graphicsView_5->getPixmap().width()==0){
            ui->graphicsView_5->setPixmap(ui->graphicsView_4->getPixmap());
            ui->graphicsView_5->showlimg(ui->graphicsView_4->getPixmap());
        }
        showWindowR(0);
    }else if(page == 6)
    {
        showWindowL(4);
        if(ui->graphicsView_6->getPixmap().width()==0){
            ui->graphicsView_6->setPixmap(ui->graphicsView_5->getPixmap());
            ui->graphicsView_6->showlimg(ui->graphicsView_5->getPixmap());
        }
        showWindowR(0);
    }

}

void MainWindow::setDebug(bool isDebug)
{
    this->ui->scrollDebug->setVisible(isDebug);
}

void MainWindow::on_stepDebug1_clicked()
{
    this->onModuleSelect(0);
}

void MainWindow::on_stepDebug2_clicked()
{
    this->onModuleSelect(1);
}

void MainWindow::on_stepDebug3_clicked()
{
    this->onModuleSelect(2);
}

//等待动画
void MainWindow::startProgressViewId(int id)
{
    for (int i = 0; i < stepList.length(); i++)
    {
        if(id == i)
        {
            stepList.at(i)->setLoading();
            break;
        }
    }
}

void MainWindow::completeViewId(int id)
{
    for (int i = 0; i < stepList.length(); i++)
    {
        if(id == i)
        {
            stepList.at(i)->setSuccess();
            break;
        }
    }
}

void MainWindow::jumpViewId(int id)
{
    onModuleSelect(id);
}


void MainWindow::showWindowL(int viewId)
{
    for(int i = 0; i < GraphicsListL.size(); i++)
    {
        if(i == viewId)
        {
            GraphicsListL[i]->setEnabled(true);
            GraphicsListL[i]->show();
        }else
        {
            GraphicsListL[i]->setEnabled(false);
            GraphicsListL[i]->hide();
        }
    }
}

void MainWindow::showWindowR(int viewId)
{
    for(int i = 0; i < GraphicsListR.size(); i++)
    {
        if(i == viewId)
        {
            GraphicsListR[i]->setEnabled(true);
            GraphicsListR[i]->show();
        }else
        {
            GraphicsListR[i]->setEnabled(false);
            GraphicsListR[i]->hide();
        }
    }
}

//实际图像显示
void MainWindow::actualParaImg(QPixmap pixmap)
{
    showWindowL(2);
    this->ui->graphicsView_4->slotlimg(pixmap);
    //    slotlimg(pixmap);
}

//分布图图像显示
void MainWindow::dismapImg(QPixmap  pixmap)
{
    showWindowL(4);
    this->ui->graphicsView_6->slotlimg(pixmap);
    //    slotlimg(pixmap);
}
//1025
void MainWindow::slotSetLog(int id,QString s)
{
    qDebug()<<s;
    signalLog(id,s);
}

//切换左侧侧边栏


void MainWindow::on_btnExit_2_clicked()
{
    log->show();
}
//显示降噪路径
void MainWindow::receiveDenFromcol(QString str)
{
    log->denTextBrowser->insertPlainText(str);
}
//显示随机路径
void MainWindow::receivedirectStrFrom(QString str)
{
    log->disinputBrowser->insertPlainText(str);
}
//显示 分布图结束文字
void MainWindow::receivePathFromConMain(QString  str)
{
    log->disparaBrowser->insertPlainText(str);
}
//2024年10月25日  接收异常点信息
void MainWindow::receiveYichangFromLog(QString str)
{
    qDebug()<<"1211"<<str;
    log->logBrowser->insertPlainText(str);
}
//显示分布图结果


>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
