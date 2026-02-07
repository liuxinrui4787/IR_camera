#ifndef COMPUTEREDRESS_H
#define COMPUTEREDRESS_H

#include <QWidget>

namespace Ui {
class computeRedress;
}

class computeRedress : public QWidget
{
    Q_OBJECT

public:
    explicit computeRedress(QWidget *parent = nullptr);
    ~computeRedress();

    void initUiCon();
    void setConAble();
signals:
    void signalImgSelect(int);
    void signalReduct();
    void signalSaveRect();
    void signalComputer();
    void signalImgCrop();
    void signalDeliverParaToBasepara(bool,int,int);
private slots:
    void on_ImgSelectButton_clicked();

    void on_SaveRectButton_clicked();

    void on_ComputerButton_clicked();

    void on_ImgCropButton_clicked();

    void on_mouseButton_clicked();

    void on_reductionButton_clicked();
    void on_pushButton_clicked();

private:
    Ui::computeRedress *ui;
};

#endif // COMPUTEREDRESS_H
