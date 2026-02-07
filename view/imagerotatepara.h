<<<<<<< HEAD
#ifndef IMAGEROTATEPARA_H
#define IMAGEROTATEPARA_H

#include "binarization.h"
#include "linefeature.h"

#include <QToolBox>
#include <QWidget>

namespace Ui {
class imageRotatePara;
}

class imageRotatePara : public QWidget
{
    Q_OBJECT

public:
    explicit imageRotatePara(QWidget *parent = nullptr);
    ~imageRotatePara();
    binarization *binarizationObj;
    lineFeature *lineFeatureObj;
    QToolBox *toolBoxObj;

signals:
    void signalImgSelect(int);
    void signalReduct();
    void signalDesignSelect(int);
    void signalMatch();
private slots:
    void on_ImgSelectButton_clicked();

    void on_mouseButton_clicked();

    void on_reductionButton_clicked();

    void on_MouseButton_2_clicked();

    void on_ImgSelectButton_2_clicked();

    void on_MatchButton_clicked();

private:
    Ui::imageRotatePara *ui;

};

#endif // IMAGEROTATEPARA_H
=======
#ifndef IMAGEROTATEPARA_H
#define IMAGEROTATEPARA_H

#include "binarization.h"
#include "linefeature.h"

#include <QToolBox>
#include <QWidget>

namespace Ui {
class imageRotatePara;
}

class imageRotatePara : public QWidget
{
    Q_OBJECT

public:
    explicit imageRotatePara(QWidget *parent = nullptr);
    ~imageRotatePara();
    binarization *binarizationObj;
    lineFeature *lineFeatureObj;
    QToolBox *toolBoxObj;

signals:
    void signalImgSelect(int);
    void signalReduct();
    void signalDesignSelect(int);
    void signalMatch();
private slots:
    void on_ImgSelectButton_clicked();

    void on_mouseButton_clicked();

    void on_reductionButton_clicked();

    void on_MouseButton_2_clicked();

    void on_ImgSelectButton_2_clicked();

    void on_MatchButton_clicked();

private:
    Ui::imageRotatePara *ui;

};

#endif // IMAGEROTATEPARA_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
