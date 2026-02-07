#include "autolinefinder.h"
#include "ui_autolinefinder.h"

AutoLineFinder::AutoLineFinder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoLineFinder)
{
    ui->setupUi(this);
}

AutoLineFinder::~AutoLineFinder()
{
    delete ui;
}

void AutoLineFinder::on_pushButton_clicked()
{
    pushAutoLine();
}
