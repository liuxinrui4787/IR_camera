#ifndef BINARIZATION_H
#define BINARIZATION_H

#include <QWidget>

namespace Ui {
class binarization;
}

class binarization : public QWidget
{
    Q_OBJECT

public:
    explicit binarization(QWidget *parent = nullptr);
    ~binarization();
signals:
    void signalRunBinarizaType(int);
    void signalSetThreasholdType(int);
    void signalRunThreashold(int);
private slots:
    void on_threasholdTypeButton_clicked();

    void on_workModeButton_clicked();

    void on_wallButton_clicked();

    void on_triangleButton_clicked();

    void on_otsuButton_clicked();

    void on_horizontalSlider_threashold_sliderMoved(int position);

private:
    Ui::binarization *ui;
    int binarizaType=0;
    int threasholdType = 0;
    int workMode = 0;
};

#endif // BINARIZATION_H
