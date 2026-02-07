<<<<<<< HEAD
﻿#include "computeredress.h"
#include "ui_computeredress.h"

computeRedress::computeRedress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::computeRedress)
{
    ui->setupUi(this);
    initUiCon();
}

//初始化降噪控件
void computeRedress::initUiCon()
{
    this->ui->checkBox_2->setDisabled(true);
    this->ui->countLineText_2->setDisabled(true);
    this->ui->threadCount_2->setDisabled(true);
    this->ui->pushButton->setDisabled(true);
}

//控件设置可用
void computeRedress::setConAble()
{
    this->ui->checkBox_2->setDisabled(false);
    this->ui->countLineText_2->setDisabled(false);
    this->ui->threadCount_2->setDisabled(false);
    this->ui->pushButton->setDisabled(false);
}

computeRedress::~computeRedress()
{
    delete ui;
}


void computeRedress::on_ImgSelectButton_clicked()
{
    signalImgSelect(1);
}

void computeRedress::on_SaveRectButton_clicked()
{
    signalSaveRect();
}

void computeRedress::on_ComputerButton_clicked()
{
    signalComputer();
}

void computeRedress::on_ImgCropButton_clicked()
{
    signalImgCrop();
}

void computeRedress::on_mouseButton_clicked()
{
    signalImgSelect(0);
}

void computeRedress::on_reductionButton_clicked()
{
    signalReduct();
}

//控件点击开始时传递所有参数设置
void computeRedress::on_pushButton_clicked()
{
    emit signalDeliverParaToBasepara(this->ui->checkBox_2->isChecked(),
                                     this->ui->countLineText_2->text().toInt(),
                                     this->ui->threadCount_2->value());
}
=======
﻿#include "computeredress.h"
#include "ui_computeredress.h"

computeRedress::computeRedress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::computeRedress)
{
    ui->setupUi(this);
    initUiCon();
}

//初始化降噪控件
void computeRedress::initUiCon()
{
    this->ui->checkBox_2->setDisabled(true);
    this->ui->countLineText_2->setDisabled(true);
    this->ui->threadCount_2->setDisabled(true);
    this->ui->pushButton->setDisabled(true);
}

//控件设置可用
void computeRedress::setConAble()
{
    this->ui->checkBox_2->setDisabled(false);
    this->ui->countLineText_2->setDisabled(false);
    this->ui->threadCount_2->setDisabled(false);
    this->ui->pushButton->setDisabled(false);
}

computeRedress::~computeRedress()
{
    delete ui;
}


void computeRedress::on_ImgSelectButton_clicked()
{
    signalImgSelect(1);
}

void computeRedress::on_SaveRectButton_clicked()
{
    signalSaveRect();
}

void computeRedress::on_ComputerButton_clicked()
{
    signalComputer();
}

void computeRedress::on_ImgCropButton_clicked()
{
    signalImgCrop();
}

void computeRedress::on_mouseButton_clicked()
{
    signalImgSelect(0);
}

void computeRedress::on_reductionButton_clicked()
{
    signalReduct();
}

//控件点击开始时传递所有参数设置
void computeRedress::on_pushButton_clicked()
{
    emit signalDeliverParaToBasepara(this->ui->checkBox_2->isChecked(),
                                     this->ui->countLineText_2->text().toInt(),
                                     this->ui->threadCount_2->value());
}
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
