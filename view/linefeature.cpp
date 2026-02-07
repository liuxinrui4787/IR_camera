#include "linefeature.h"
#include "ui_linefeature.h"

lineFeature::lineFeature(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lineFeature)
{
    ui->setupUi(this);
    ui->autoLine_widget->setVisible(true);
    ui->hough_widget->setVisible(false);
    ui->saveLineButton->hide();
    connect(ui->hough_widget,SIGNAL(MinLength(int)),this,SLOT(slotMinLength(int)));
    connect(ui->hough_widget,SIGNAL(MaxGap(int)),this,SLOT(slotMaxGap(int)));
    connect(ui->hough_widget,SIGNAL(MinVote(int)),this,SLOT(slotMinVote(int)));
    connect(ui->autoLine_widget,SIGNAL(pushAutoLine()),this,SLOT(AutoLine()));

}

lineFeature::~lineFeature()
{
    delete ui;
}

void lineFeature::on_workModeButton_clicked()
{
    if(workMode == 0)
    {
        workMode = 1;
        ui->workModeButton->setText(u8"非自动");
        ui->autoLine_widget->setVisible(false);
        ui->hough_widget->setVisible(true);
    }else
    {
        workMode = 0;
        ui->workModeButton->setText(u8"自动");
        ui->autoLine_widget->setVisible(true);
        ui->hough_widget->setVisible(false);
    }
}

void lineFeature::slotMinLength(int position)
{
    signalMinLength(position);
    ui->saveLineButton->show();
}

void lineFeature::slotMaxGap(int position)
{
    signalMaxGap(position);
    ui->saveLineButton->show();
}

void lineFeature::slotMinVote(int position)
{
    signalMinVote(position);
    ui->saveLineButton->show();
}

void lineFeature::AutoLine()
{
    signalAutoLine();
    ui->saveLineButton->show();
}

void lineFeature::on_deleteLineButton_clicked()
{
    signalImgSelect(3);
}

void lineFeature::on_saveLineButton_clicked()
{
    signalSaveLine();
    ui->saveLineButton->hide();
}

void lineFeature::on_mouseButton_clicked()
{
    signalImgSelect(0);
}

void lineFeature::on_AggregateLineButton_clicked()
{
    signalAggregateLine();
}
