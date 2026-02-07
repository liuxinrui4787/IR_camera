<<<<<<< HEAD
#include "basefileinput.h"
#include "ui_basefileinput.h"

BaseFileInput::BaseFileInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseFileInput)
{
    ui->setupUi(this);
    inputFileObj = ui->page;
    FileSelectObj = ui->page_2;

}

BaseFileInput::~BaseFileInput()
{
    delete ui;
}
void BaseFileInput::recPageIdx(int page)
{
    this->ui->stackedWidget->setCurrentIndex(page);
}

=======
#include "basefileinput.h"
#include "ui_basefileinput.h"

BaseFileInput::BaseFileInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseFileInput)
{
    ui->setupUi(this);
    inputFileObj = ui->page;
    FileSelectObj = ui->page_2;

}

BaseFileInput::~BaseFileInput()
{
    delete ui;
}
void BaseFileInput::recPageIdx(int page)
{
    this->ui->stackedWidget->setCurrentIndex(page);
}

>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
