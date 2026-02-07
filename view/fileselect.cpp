#include "fileselect.h"
#include "components/qtmaterialstyle.h"
#include "qfiledialog.h"
#include "ui_fileselect.h"
#include <QFileDialog>
#include <QMessageBox>

FileSelect::FileSelect(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::FileSelect)
{
    ui->setupUi(this);

    this->ui->btnChooseChip->setColor(QtMaterialStyle::instance().themeColor("primary1"));
    this->ui->btnChooseChip->setIcon(QIcon(":/image/btn_search.svg"));
    this->ui->btnChooseChip->setIconSize(QSize(30, 30));

    this->ui->btnChooseInfra->setColor(QtMaterialStyle::instance().themeColor("primary1"));
    this->ui->btnChooseInfra->setIcon(QIcon(":/image/btn_search.svg"));
    this->ui->btnChooseInfra->setIconSize(QSize(30, 30));
}

FileSelect::~FileSelect()
{
    delete ui;
}

void FileSelect::ReadConfig(const QSettings &conf)
{

//    this->ui->edit_file->setText(conf.value("vfile", "").toString());
}

bool FileSelect::checkFileExists(const QString &filePath)
{
    if (filePath == QString::fromLocal8Bit(""))
    {
        return false;
    }
    QFileInfo info(filePath);
    if (!info.exists())
    {
        return false;
    }
    return true;
}

/**
 * @brief 设置读取的文件
 * @param file
 */
void FileSelect::SetFilePath(const QString &file)
{
    this->chipDataPath = file;
//    this->ui->edit_file->setText(file);
//    this->ui->edit_file->setAlignment(Qt::AlignBottom);
//    this->ui->edit_file->moveCursor(QTextCursor::End);
    QSettings set("./ifconfig.ini", QSettings::IniFormat);
    set.setValue("chipPath", file);
    set.endGroup();
}

void FileSelect::setInfraPath(const QString &path)
{
    this->infraDataPath = path;
    QSettings set("./ifconfig.ini", QSettings::IniFormat);
    set.setValue("infraPath", path);
    set.endGroup();
}

//选择参考图像路径
void FileSelect::on_btnChooseChip_clicked()
{
    QString chipDesignPath = QFileDialog::getExistingDirectory(this, "选择保存降噪结果文件夹", this->chipDataPath);


    if (!checkActualFile(chipDesignPath))
    {
        QMessageBox::warning(Q_NULLPTR, QString("警告"),
                             QString("当前文件夹不存在"));
    }


}

//检测选择的路径是否存在
bool FileSelect::checkActualFile(QString filePath, bool isSave)
{

    this->chipDataPath = filePath;
    this->ui->chipDesignPath->setText(filePath);
    return true;
}

bool FileSelect::checkChipFile(QString filePath, bool isSave)
{
    this->infraDataPath = filePath;
    this->ui->infraDataPath->setText(filePath);
    return true;
}

//选择红外数据路径
void FileSelect::on_btnChooseInfra_clicked()
{
    QString infraData = QFileDialog::getExistingDirectory(this, "选择保存降噪结果文件夹", this->infraDataPath);


    if (!checkChipFile(infraData))
    {
        QMessageBox::warning(Q_NULLPTR, QString("警告"),
                             QString("当前文件夹不存在"));
    }
}

