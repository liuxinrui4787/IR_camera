#include "inputfile.h"
#include "ui_inputfile.h"
#include "components/qtmaterialstyle.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

inputFile::inputFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inputFile)
{
    ui->setupUi(this);
    this->ui->btnOpen->setColor(QtMaterialStyle::instance().themeColor("primary1"));
    this->ui->btnOpen->setIcon(QIcon(":/image/btn_search.svg"));
    this->ui->btnOpen->setIconSize(QSize(30, 30));
    this->ui->btnOpen_2->setColor(QtMaterialStyle::instance().themeColor("primary1"));
    this->ui->btnOpen_2->setIcon(QIcon(":/image/btn_search.svg"));
    this->ui->btnOpen_2->setIconSize(QSize(30, 30));
    this->ui->btnOpen_3->setColor(QtMaterialStyle::instance().themeColor("primary1"));
    this->ui->btnOpen_3->setIcon(QIcon(":/image/btn_search.svg"));
    this->ui->btnOpen_3->setIconSize(QSize(30, 30));
    readFielPath();
    this->ui->btnOpen_3->hide();
    this->ui->edit_file_3->hide();
    this->ui->fixline_3->hide();
    this->ui->fix_3->hide();
    this->ui->label_3->hide();
}

inputFile::~inputFile()
{
    delete ui;
}

void inputFile::on_btnOpen_clicked()
{
    infraredPathList = QFileDialog::getOpenFileNames
            (this, "选择输入红外文件", ui->edit_file->document()->toPlainText(), "图像数据(.png *.jpg *.csv *.xls *.xlsx);;所有文件(*.*)");
    if(infraredPathList.isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, QString(u8"警告"),
                             QString(u8"当前文件不存在"));
        return;
    }
    infraredFilePath = infraredPathList.at(0);
    ui->edit_file->setText(infraredFilePath);
    signalinfraredFile(infraredPathList);

    QSettings iniFile("./config.ini", QSettings::IniFormat);
    iniFile.beginGroup("imageAlign");
    iniFile.setValue("realInputpath", this->infraredFilePath);
    iniFile.endGroup();
    iniFile.sync();
}

void inputFile::on_btnOpen_2_clicked()
{
    designPathList = QFileDialog::getOpenFileNames(this, "选择输入芯片文件", ui->edit_file_2->document()->toPlainText(), "图像数据(*.csv *.xls *.xlsx);;所有文件(*.*)");
    if (designPathList.isEmpty())
    {
        QMessageBox::warning(Q_NULLPTR, QString("警告"),QString("当前文件不存在"));
        return;
    }
    designFilePath = designPathList.at(0);
    ui->edit_file_2->setText(designFilePath);
    signalDesignFilePath(designPathList);

    QSettings iniFile("./config.ini", QSettings::IniFormat);
    iniFile.beginGroup("imageAlign");
    iniFile.setValue("designInputpath", this->designFilePath);
    iniFile.endGroup();
    iniFile.sync();
}
bool inputFile::checkImgPath(QString imgPath, bool isSave)
{
    if (imgPath == QString::fromLocal8Bit(""))
    {
        return false;
    }
    if(!QFileInfo(imgPath).isFile())
    {
        return false;
    }
    return true;
}

void inputFile::readFielPath()
{
    QSettings iniFile("./config.ini", QSettings::IniFormat);
    iniFile.beginGroup("imageAlign");
    this->infraredFilePath = iniFile.value("realInputpath", "").toString();
    this->designFilePath = iniFile.value("designInputpath", "").toString();
    if(checkImgPath(infraredFilePath))
    {
        //预加载内容
        ui->edit_file->setText(infraredFilePath);
    }
    if(checkImgPath(designFilePath))
    {
        //预加载内容
        ui->edit_file_2->setText(designFilePath);
    }
}

void inputFile::on_btnOpen_3_clicked()
{
    QString chipDesignPath = QFileDialog::getExistingDirectory(this, "选择数据输入文件夹", ui->edit_file_3->document()->toPlainText());
    if(!checkDirExists(chipDesignPath))
    {
        QMessageBox::warning(Q_NULLPTR, QString("警告"),QString("当前文件夹不存在"));
    }
    chipDesignPath = chipDesignPath+"/";
}
bool inputFile::checkDirExists(QString m_fileDir)
{
    ui->edit_file_3->setText("");
    if (m_fileDir == QString::fromLocal8Bit(""))
    {
        return false;
    }
    if (!QDir(m_fileDir).exists())
    {
        return false;
    }
    ui->edit_file_3->setText(m_fileDir);
    return true;
}

void inputFile::on_checkBox_2_stateChanged(int arg1)
{
    testCheck(arg1);
}
