#include "generatereferencemap.h"
#include "ui_generatereferencemap.h"
#include <QMessageBox>
#include <qdebug.h>
#include "view/basepara.h"

GenerateReferenceMap::GenerateReferenceMap(QWidget *parent)
    : BaseModule(parent),
      ui(new Ui::GenerateReferenceMap)
{
    ui->setupUi(this);
    baseParaObj = this->ui->page;
    BaseFileInputObj = this->ui->page1;
    randomDeleteObj = this->ui->page_2;

    this->init();
}

GenerateReferenceMap::~GenerateReferenceMap()
{
    delete ui;
}

void GenerateReferenceMap::init()
{
    updateButton(this->ui->btnFile, ":/image/icon_file_select.svg");
    updateButton(this->ui->btnProportion, ":/image/icon_proportion.svg");
    updateButton(this->ui->btnProportion_2, ":/image/delete.svg");
    updateButton(this->ui->btnProportion_3, ":/image/Binarization.svg");
    updateButton(this->ui->btnProportion_4, ":/image/rotate.svg");

    /*********************连接信号 *****************************
     * ************************************************/

    connect(this,SIGNAL(pageIdx(int)),this->ui->page,SLOT(recPageIdx(int)));
    //  传文件
    connect(BaseFileInputObj->inputFileObj,SIGNAL(signalinfraredFile(QStringList)),baseParaObj,SLOT(slotinfraredFile(QStringList)));
    connect(BaseFileInputObj->inputFileObj,SIGNAL(signalDesignFilePath(QStringList)),baseParaObj,SLOT(slotDesignFilePath(QStringList)));
    connect(BaseFileInputObj->inputFileObj,SIGNAL(testCheck(int)),baseParaObj,SLOT(testCheck(int)));
    connect(baseParaObj,SIGNAL(signalComplete()),this,SLOT(slotComplete()));
    //传输随机删除图像
    connect(ui->page_2,SIGNAL(signalRDImg(QPixmap)),this,SLOT(slotRDImg(QPixmap)));
    //二值化
    connect(ui->page_3,SIGNAL(signalRunBinarizaType(int)),baseParaObj,SLOT(slotRunBinarizaType(int)));
    connect(ui->page_3,SIGNAL(signalSetThreasholdType(int)),baseParaObj,SLOT(slotSetThreasholdType(int)));
    connect(ui->page_3,SIGNAL(signalRunThreashold(int)),baseParaObj,SLOT(slotRunThreashold(int)));

}

//设置按钮
void GenerateReferenceMap::setButtonState(int page)
{
    switch (page) {
    case 0:
        this->ui->btnFile->setEnabled(true);
        this->ui->btnProportion->setEnabled(true);
        this->ui->btnProportion_2->setEnabled(true);
        this->ui->btnProportion_3->setEnabled(false);
        this->ui->btnProportion_4->setEnabled(false);
        break;
    case 1:
        this->ui->btnFile->setEnabled(true);
        this->ui->btnProportion->setEnabled(true);
        this->ui->btnProportion_2->setEnabled(false);
        this->ui->btnProportion_3->setEnabled(true);
        this->ui->btnProportion_4->setEnabled(true);
        break;
    case 2:
        this->ui->btnFile->setEnabled(true);
        this->ui->btnProportion->setEnabled(true);
        this->ui->btnProportion_2->setEnabled(false);
        this->ui->btnProportion_3->setEnabled(false);
        this->ui->btnProportion_4->setEnabled(false);
        break;
    case 3:
        this->ui->btnFile->setEnabled(true);
        this->ui->btnProportion->setEnabled(true);
        this->ui->btnProportion_2->setEnabled(false);
        this->ui->btnProportion_3->setEnabled(false);
        this->ui->btnProportion_4->setEnabled(false);
        break;
    case 4:
        this->ui->btnFile->setEnabled(true);
        this->ui->btnProportion->setEnabled(true);
        this->ui->btnProportion_2->setEnabled(false);
        this->ui->btnProportion_3->setEnabled(false);
        this->ui->btnProportion_4->setEnabled(false);
        break;
    case 5:
        this->ui->btnFile->setEnabled(true);
        this->ui->btnProportion->setEnabled(true);
        this->ui->btnProportion_2->setEnabled(false);
        this->ui->btnProportion_3->setEnabled(false);
        this->ui->btnProportion_4->setEnabled(false);
        break;
    case 6:
        this->ui->btnFile->setEnabled(true);
        this->ui->btnProportion->setEnabled(false);
        this->ui->btnProportion_2->setEnabled(false);
        this->ui->btnProportion_3->setEnabled(false);
        this->ui->btnProportion_4->setEnabled(false);
        break;
    }
}

//设置选中样式
void GenerateReferenceMap::setButtonTrueCss(int buttonIdx)
{
    switch(buttonIdx){
    case 0:
        this->ui->btnFile->setUseThemeColors(1);
        break;
    case 1:
        this->ui->btnProportion ->setUseThemeColors(1);
        break;
    case 2:
        this->ui->btnProportion_2->setUseThemeColors(1);
        break;
    case 3:
        this->ui->btnProportion_3->setUseThemeColors(1);
        break;
    case 4:
        this->ui->btnProportion_4->setUseThemeColors(1);
        break;
    }
}

//设置选不中样式
void GenerateReferenceMap::setButtonFalseCss(int buttonIdx)
{
    switch(buttonIdx){
    case 0:
        this->ui->btnProportion ->setUseThemeColors(0);
        this->ui->btnProportion_2 ->setUseThemeColors(0);
        this->ui->btnProportion_3 ->setUseThemeColors(0);
        this->ui->btnProportion_4->setUseThemeColors(0);
        break;
    case 1:
        this->ui->btnFile ->setUseThemeColors(0);
        this->ui->btnProportion_2 ->setUseThemeColors(0);
        this->ui->btnProportion_3 ->setUseThemeColors(0);
        this->ui->btnProportion_4->setUseThemeColors(0);
        break;
    case 2:
        this->ui->btnFile ->setUseThemeColors(0);
        this->ui->btnProportion ->setUseThemeColors(0);
        this->ui->btnProportion_3 ->setUseThemeColors(0);
        this->ui->btnProportion_4->setUseThemeColors(0);
        break;
    case 3:
        this->ui->btnFile ->setUseThemeColors(0);
        this->ui->btnProportion ->setUseThemeColors(0);
        this->ui->btnProportion_2 ->setUseThemeColors(0);
        this->ui->btnProportion_4->setUseThemeColors(0);
        break;
    case 4:
        this->ui->btnFile ->setUseThemeColors(0);
        this->ui->btnProportion ->setUseThemeColors(0);
        this->ui->btnProportion_2 ->setUseThemeColors(0);
        this->ui->btnProportion_3->setUseThemeColors(0);
        break;
    case 5:
        this->ui->btnFile ->setUseThemeColors(0);
        this->ui->btnProportion ->setUseThemeColors(0);
        this->ui->btnProportion_2 ->setUseThemeColors(0);
        this->ui->btnProportion_3->setUseThemeColors(0);
        this->ui->btnProportion_4 ->setUseThemeColors(0);
        break;
    }

}

void GenerateReferenceMap::on_btnFile_clicked()
{
    this->ui->bottomMenu->setCurrentIndex(0);
    setButtonFalseCss(0);
    setButtonTrueCss(0);
}

void GenerateReferenceMap::on_btnProportion_clicked()
{

    this->ui->bottomMenu->setCurrentIndex(1);
    setButtonFalseCss(1);
    setButtonTrueCss(1);
}

void GenerateReferenceMap::on_btnProportion_2_clicked()
{
    this->ui->bottomMenu->setCurrentIndex(2);
    setButtonFalseCss(2);
    setButtonTrueCss(2);
}

void GenerateReferenceMap::on_btnProportion_3_clicked()
{
    this->ui->bottomMenu->setCurrentIndex(3);
    setButtonFalseCss(3);
    setButtonTrueCss(3);
}

void GenerateReferenceMap::on_btnProportion_4_clicked()
{
    this->ui->bottomMenu->setCurrentIndex(4);
    setButtonFalseCss(4);
    setButtonTrueCss(4);
}



//显示页面
void GenerateReferenceMap::currentShowIdx(int page)
{
    qDebug()<<"page::"<<page;
    this->pageIndix= page;
    if(this->pageIndix!=0){
        this->ui->bottomMenu->setCurrentIndex(1);
        setButtonFalseCss(1);
        setButtonTrueCss(1);
    }else{
        this->ui->bottomMenu->setCurrentIndex(0);
        setButtonFalseCss(0);
        setButtonTrueCss(0);
    }
    setButtonState(this->pageIndix);
    emit pageIdx(page);
    if(page == 0 || page == 1 || page == 2)
    {
        BaseFileInputObj->recPageIdx(0);
    }else
    {
        BaseFileInputObj->recPageIdx(1);
    }
}

void GenerateReferenceMap::slotComplete()
{
    completeViewId(0);
    QMessageBox msgBox;
    msgBox.setText("参考图生成完成。");
    msgBox.setInformativeText("是否跳转图像偏移？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Yes:
        referenceResultDate(baseParaObj->referpath,baseParaObj->designMat);
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
void GenerateReferenceMap::slotRDImg(QPixmap pix)
{
    signalrimg(pix);
}
