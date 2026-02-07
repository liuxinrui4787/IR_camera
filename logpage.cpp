#include "logpage.h"
#include "ui_logpage.h"

logPage::logPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logPage)
{
    ui->setupUi(this);
}

logPage::~logPage()
{
    delete ui;
}
