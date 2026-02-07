#ifndef HOUGHLINESETTING_H
#define HOUGHLINESETTING_H

#include <QWidget>

namespace Ui {
class HoughLineSetting;
}

class HoughLineSetting : public QWidget
{
    Q_OBJECT

public:
    explicit HoughLineSetting(QWidget *parent = nullptr);
    ~HoughLineSetting();
signals:
    void MinLength(int);
    void MaxGap(int);
    void MinVote(int);
private slots:
    void on_Slider_MinLength_sliderMoved(int position);

    void on_Slider_MaxGap_sliderMoved(int position);

    void on_Slider_MinVote_sliderMoved(int position);
private:
    Ui::HoughLineSetting *ui;
};

#endif // HOUGHLINESETTING_H
