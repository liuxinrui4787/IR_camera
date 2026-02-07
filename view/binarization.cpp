<<<<<<< HEAD
﻿#include "binarization.h"
#include "ui_binarization.h"
#include "components/qtmaterialstyle.h"

binarization::binarization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::binarization)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
//    ui->threasholdTypeButton->setOrientation(Qt::Horizontal);
    ui->threasholdTypeButton->setIcon(QIcon(":/image/switch-on.svg"));
    ui->workModeButton->setIcon(QIcon(":/image/switch-on.svg"));
    ui->threasholdTypeButton->setIconSize(QSize(30, 30));
    ui->workModeButton->setIconSize(QSize(30, 30));
}

binarization::~binarization()
{
    delete ui;
}
void binarization::on_threasholdTypeButton_clicked()
{
    if(threasholdType == 0)
    {
        threasholdType = 1;
        ui->threasholdTypeButton->setText(u8"反选");
        ui->threasholdTypeButton->setIcon(QIcon(":/image/switch-off.svg"));
        ui->threasholdTypeButton->setIconSize(QSize(30, 30));
    }else
    {
        threasholdType = 0;
        ui->threasholdTypeButton->setText(u8"正选");
        ui->threasholdTypeButton->setIcon(QIcon(":/image/switch-on.svg"));
        ui->threasholdTypeButton->setIconSize(QSize(30, 30));
    }
    signalSetThreasholdType(threasholdType);
}

void binarization::on_workModeButton_clicked()
{
    if(workMode == 0)
    {
        workMode = 1;
        ui->workModeButton->setText("非自动");
        ui->stackedWidget->setCurrentIndex(1);
        ui->workModeButton->setIcon(QIcon(":/image/switch-off.svg"));
        ui->workModeButton->setIconSize(QSize(30, 30));
    }else
    {
        workMode = 0;
        ui->workModeButton->setText("自动");
        ui->stackedWidget->setCurrentIndex(0);
        ui->workModeButton->setIcon(QIcon(":/image/switch-on.svg"));
        ui->workModeButton->setIconSize(QSize(30, 30));
    }
}

void binarization::on_wallButton_clicked()
{
    signalRunBinarizaType(1);
}

void binarization::on_triangleButton_clicked()
{
    signalRunBinarizaType(2);
}

void binarization::on_otsuButton_clicked()
{
    signalRunBinarizaType(3);
}

void binarization::on_horizontalSlider_threashold_sliderMoved(int position)
{
    ui->label_line_threashold->setNum(position);
    signalRunThreashold(position);
}
=======
﻿#include "binarization.h"
#include "ui_binarization.h"
#include "components/qtmaterialstyle.h"

binarization::binarization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::binarization)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
//    ui->threasholdTypeButton->setOrientation(Qt::Horizontal);
    ui->threasholdTypeButton->setIcon(QIcon(":/image/switch-on.svg"));
    ui->workModeButton->setIcon(QIcon(":/image/switch-on.svg"));
    ui->threasholdTypeButton->setIconSize(QSize(30, 30));
    ui->workModeButton->setIconSize(QSize(30, 30));
}

binarization::~binarization()
{
    delete ui;
}
void binarization::on_threasholdTypeButton_clicked()
{
    if(threasholdType == 0)
    {
        threasholdType = 1;
        ui->threasholdTypeButton->setText(u8"反选");
        ui->threasholdTypeButton->setIcon(QIcon(":/image/switch-off.svg"));
        ui->threasholdTypeButton->setIconSize(QSize(30, 30));
    }else
    {
        threasholdType = 0;
        ui->threasholdTypeButton->setText(u8"正选");
        ui->threasholdTypeButton->setIcon(QIcon(":/image/switch-on.svg"));
        ui->threasholdTypeButton->setIconSize(QSize(30, 30));
    }
    signalSetThreasholdType(threasholdType);
}

void binarization::on_workModeButton_clicked()
{
    if(workMode == 0)
    {
        workMode = 1;
        ui->workModeButton->setText("非自动");
        ui->stackedWidget->setCurrentIndex(1);
        ui->workModeButton->setIcon(QIcon(":/image/switch-off.svg"));
        ui->workModeButton->setIconSize(QSize(30, 30));
    }else
    {
        workMode = 0;
        ui->workModeButton->setText("自动");
        ui->stackedWidget->setCurrentIndex(0);
        ui->workModeButton->setIcon(QIcon(":/image/switch-on.svg"));
        ui->workModeButton->setIconSize(QSize(30, 30));
    }
}

void binarization::on_wallButton_clicked()
{
    signalRunBinarizaType(1);
}

void binarization::on_triangleButton_clicked()
{
    signalRunBinarizaType(2);
}

void binarization::on_otsuButton_clicked()
{
    signalRunBinarizaType(3);
}

void binarization::on_horizontalSlider_threashold_sliderMoved(int position)
{
    ui->label_line_threashold->setNum(position);
    signalRunThreashold(position);
}
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
