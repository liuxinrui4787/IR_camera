<<<<<<< HEAD
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
=======
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
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
