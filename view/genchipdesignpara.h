#ifndef GENCHIPDESIGNPARA_H
#define GENCHIPDESIGNPARA_H

#include <QWidget>

namespace Ui {
class genChipDesignPara;
}

class genChipDesignPara : public QWidget
{
    Q_OBJECT

public:
    explicit genChipDesignPara(QWidget *parent = nullptr);
    ~genChipDesignPara();
    int scale = 1500;

signals:
    void signalGenChipDesignPara(int offsetType);
private slots:
    void on_startButton_clicked();

    void on_spinBox_scale_valueChanged(const QString &arg1);

private:
    Ui::genChipDesignPara *ui;
};

#endif // GENCHIPDESIGNPARA_H
