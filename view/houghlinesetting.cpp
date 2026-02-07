#include "houghlinesetting.h"
#include "ui_houghlinesetting.h"

HoughLineSetting::HoughLineSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HoughLineSetting)
{
    ui->setupUi(this);
}

HoughLineSetting::~HoughLineSetting()
{
    delete ui;
}
void HoughLineSetting::on_Slider_MinLength_sliderMoved(int position)
{
    ui->label_MinLength->setNum(position);
    MinLength(position);
}

void HoughLineSetting::on_Slider_MaxGap_sliderMoved(int position)
{
    ui->label_MaxGap->setNum(position);
    MaxGap(position);
}

void HoughLineSetting::on_Slider_MinVote_sliderMoved(int position)
{
    ui->label_MinVote->setNum(position);
    MinVote(position);
}
