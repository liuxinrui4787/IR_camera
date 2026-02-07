#include "rotatecorrect.h"
#include "ui_rotatecorrect.h"

rotateCorrect::rotateCorrect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rotateCorrect)
{
    ui->setupUi(this);
}

rotateCorrect::~rotateCorrect()
{
    delete ui;
}
