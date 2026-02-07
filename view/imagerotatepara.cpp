<<<<<<< HEAD
#include "imagerotatepara.h"
#include "ui_imagerotatepara.h"

imageRotatePara::imageRotatePara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageRotatePara)
{
    ui->setupUi(this);
    binarizationObj = ui->page_2;
    lineFeatureObj = ui->page_3;
    toolBoxObj = ui->toolBox;
    ui->toolBox->setCurrentIndex(0);
}

imageRotatePara::~imageRotatePara()
{
    delete ui;
}


void imageRotatePara::on_ImgSelectButton_clicked()
{
    signalImgSelect(1);
}

void imageRotatePara::on_mouseButton_clicked()
{
    signalImgSelect(0);
}

void imageRotatePara::on_reductionButton_clicked()
{
    signalReduct();
}

void imageRotatePara::on_MouseButton_2_clicked()
{
    signalDesignSelect(0);
}

void imageRotatePara::on_ImgSelectButton_2_clicked()
{
    signalDesignSelect(2);
}

void imageRotatePara::on_MatchButton_clicked()
{
    signalMatch();
}
=======
#include "imagerotatepara.h"
#include "ui_imagerotatepara.h"

imageRotatePara::imageRotatePara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageRotatePara)
{
    ui->setupUi(this);
    binarizationObj = ui->page_2;
    lineFeatureObj = ui->page_3;
    toolBoxObj = ui->toolBox;
    ui->toolBox->setCurrentIndex(0);
}

imageRotatePara::~imageRotatePara()
{
    delete ui;
}


void imageRotatePara::on_ImgSelectButton_clicked()
{
    signalImgSelect(1);
}

void imageRotatePara::on_mouseButton_clicked()
{
    signalImgSelect(0);
}

void imageRotatePara::on_reductionButton_clicked()
{
    signalReduct();
}

void imageRotatePara::on_MouseButton_2_clicked()
{
    signalDesignSelect(0);
}

void imageRotatePara::on_ImgSelectButton_2_clicked()
{
    signalDesignSelect(2);
}

void imageRotatePara::on_MatchButton_clicked()
{
    signalMatch();
}
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
