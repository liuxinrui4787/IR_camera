#ifndef LINEFEATURE_H
#define LINEFEATURE_H

#include "autolinefinder.h"
#include "houghlinesetting.h"

#include <QWidget>

namespace Ui {
class lineFeature;
}

class lineFeature : public QWidget
{
    Q_OBJECT

public:
    explicit lineFeature(QWidget *parent = nullptr);
    ~lineFeature();
signals:
    void signalMinLength(int);
    void signalMaxGap(int);
    void signalMinVote(int);
    void signalAutoLine();
    void signalDeleteLine();
    void signalAggregateLine();
    void signalSaveLine();
    void signalImgSelect(int);
private slots:
    void on_workModeButton_clicked();

    void slotMinLength(int);
    void slotMaxGap(int);
    void slotMinVote(int);

    void AutoLine();

    void on_deleteLineButton_clicked();

    void on_AggregateLineButton_clicked();

    void on_saveLineButton_clicked();

    void on_mouseButton_clicked();

private:
    Ui::lineFeature *ui;
    int workMode = 0;
};

#endif // LINEFEATURE_H
