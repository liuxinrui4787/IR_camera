<<<<<<< HEAD
﻿#ifndef FILESELECT_H
#define FILESELECT_H

#include <QSettings>
#include <QWidget>
namespace Ui
{
class FileSelect;
}

class FileSelect : public QWidget
{
    Q_OBJECT

  public:
    explicit FileSelect(QWidget *parent = nullptr);
    ~FileSelect();

    void ReadConfig(const QSettings &conf);
    void SetFilePath(const QString &path);
    void setInfraPath(const QString &path);
    bool checkActualFile(QString filePath, bool isSave=false);
    bool checkChipFile(QString filePath, bool isSave=false);


  private slots:
    bool checkFileExists(const QString &path);

    void on_btnChooseChip_clicked();

    void on_btnChooseInfra_clicked();


private:
    Ui::FileSelect *ui;
    QString m_inputPath;
    QString chipDataPath;
    QString infraDataPath;
};

#endif // FILESELECT_H
=======
﻿#ifndef FILESELECT_H
#define FILESELECT_H

#include <QSettings>
#include <QWidget>
namespace Ui
{
class FileSelect;
}

class FileSelect : public QWidget
{
    Q_OBJECT

  public:
    explicit FileSelect(QWidget *parent = nullptr);
    ~FileSelect();

    void ReadConfig(const QSettings &conf);
    void SetFilePath(const QString &path);
    void setInfraPath(const QString &path);
    bool checkActualFile(QString filePath, bool isSave=false);
    bool checkChipFile(QString filePath, bool isSave=false);


  private slots:
    bool checkFileExists(const QString &path);

    void on_btnChooseChip_clicked();

    void on_btnChooseInfra_clicked();


private:
    Ui::FileSelect *ui;
    QString m_inputPath;
    QString chipDataPath;
    QString infraDataPath;
};

#endif // FILESELECT_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
