#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <QWidget>

namespace Ui {
class inputFile;
}

class inputFile : public QWidget
{
    Q_OBJECT

public:
    explicit inputFile(QWidget *parent = nullptr);
    ~inputFile();
    QString infraredFilePath;
    QString designFilePath;
    QStringList infraredPathList;
    QStringList designPathList;
signals:
    void signalinfraredFile(QStringList s);
    void signalDesignFilePath(QStringList s);
    void signalFilePath(QStringList s);
    void testCheck(int);
protected:
    bool checkImgPath(QString imgPath, bool isSave= false);
    void readFielPath();
private slots:
    void on_btnOpen_clicked();

    void on_btnOpen_2_clicked();

    void on_btnOpen_3_clicked();

    void on_checkBox_2_stateChanged(int arg1);

private:
    Ui::inputFile *ui;

    bool checkDirExists(QString m_fileDir);
};

#endif // INPUTFILE_H
