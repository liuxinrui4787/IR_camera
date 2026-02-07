<<<<<<< HEAD
#include "logpagetest.h"
#include "ui_logpagetest.h"

LogpageTest::LogpageTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogpageTest)
{
    ui->setupUi(this);
    denTextBrowser = ui->textBrowser_4;
    disinputBrowser = ui->textBrowser_5;
    disparaBrowser = ui->textBrowser_6;
    logBrowser = ui->textBrowser_7;
}

LogpageTest::~LogpageTest()
{
    delete ui;
}
//1025
void LogpageTest::slotLog(int id, QString s)
{
    switch (id)
    {
    case 0:
        ui->textBrowser->insertPlainText(s+"\n");
        break;
    case 1:
        ui->textBrowser_2->insertPlainText(s+"\n");
        break;
    case 2:
        ui->textBrowser_3->insertPlainText(s+"\n");
        break;
    case 3:
        ui->textBrowser_4->insertPlainText(s+"\n");
        break;
    case 4:
        ui->textBrowser_5->insertPlainText(s+"\n");
        break;
    case 5:
        ui->textBrowser_6->insertPlainText(s+"\n");
        break;
    default:
        break;
    }
}
=======
#include "logpagetest.h"
#include "ui_logpagetest.h"

LogpageTest::LogpageTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogpageTest)
{
    ui->setupUi(this);
    denTextBrowser = ui->textBrowser_4;
    disinputBrowser = ui->textBrowser_5;
    disparaBrowser = ui->textBrowser_6;
    logBrowser = ui->textBrowser_7;
}

LogpageTest::~LogpageTest()
{
    delete ui;
}
//1025
void LogpageTest::slotLog(int id, QString s)
{
    switch (id)
    {
    case 0:
        ui->textBrowser->insertPlainText(s+"\n");
        break;
    case 1:
        ui->textBrowser_2->insertPlainText(s+"\n");
        break;
    case 2:
        ui->textBrowser_3->insertPlainText(s+"\n");
        break;
    case 3:
        ui->textBrowser_4->insertPlainText(s+"\n");
        break;
    case 4:
        ui->textBrowser_5->insertPlainText(s+"\n");
        break;
    case 5:
        ui->textBrowser_6->insertPlainText(s+"\n");
        break;
    default:
        break;
    }
}
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
