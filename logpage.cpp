<<<<<<< HEAD
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
=======
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
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
