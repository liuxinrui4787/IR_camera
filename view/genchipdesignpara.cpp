<<<<<<< HEAD
#include "genchipdesignpara.h"
#include "ui_genchipdesignpara.h"

genChipDesignPara::genChipDesignPara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::genChipDesignPara)
{
    ui->setupUi(this);
}

genChipDesignPara::~genChipDesignPara()
{
    delete ui;
}

void genChipDesignPara::on_startButton_clicked()
{
    scale = ui->spinBox_scale->text().toInt();
    int offsetType = ui->spinBox_offsetType->text().toInt();
    signalGenChipDesignPara(offsetType);
}

void genChipDesignPara::on_spinBox_scale_valueChanged(const QString &arg1)
{
    scale = arg1.toInt();
}
=======
#include "genchipdesignpara.h"
#include "ui_genchipdesignpara.h"

genChipDesignPara::genChipDesignPara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::genChipDesignPara)
{
    ui->setupUi(this);
}

genChipDesignPara::~genChipDesignPara()
{
    delete ui;
}

void genChipDesignPara::on_startButton_clicked()
{
    scale = ui->spinBox_scale->text().toInt();
    int offsetType = ui->spinBox_offsetType->text().toInt();
    signalGenChipDesignPara(offsetType);
}

void genChipDesignPara::on_spinBox_scale_valueChanged(const QString &arg1)
{
    scale = arg1.toInt();
}
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
