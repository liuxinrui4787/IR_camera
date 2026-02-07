#ifndef GENERATEREFERENCEMAP_H
#define GENERATEREFERENCEMAP_H

#include "basemodule.h"
#include <QWidget>
#include "view/basepara.h"
#include <view/basefileinput.h>
#include "view/randomdelete.h"

using namespace std;

namespace Ui
{
class GenerateReferenceMap;
}

class GenerateReferenceMap : public BaseModule
{
    Q_OBJECT

  public:
    explicit GenerateReferenceMap(QWidget *parent = nullptr);
    ~GenerateReferenceMap();

    void init();
    void setButtonState(int);
    void setButtonTrueCss(int);
    void setButtonFalseCss(int);
    basePara* baseParaObj;
    BaseFileInput* BaseFileInputObj;
    randomDelete* randomDeleteObj;
signals:
    void pageIdx(int);
    void signalrimg(QPixmap);
    void signalReferenceMat(Mat);
    void DirPath(QString);
    void startProgressViewId(int);
    void completeViewId(int);
    void jumpViewId(int);
    void referenceResultDate(QString path,Mat design);

  private slots:
    void on_btnFile_clicked();

    void on_btnProportion_clicked();

    void on_btnProportion_2_clicked();

    void on_btnProportion_3_clicked();

    void on_btnProportion_4_clicked();


    void currentShowIdx(int);

    void slotComplete();

    void slotRDImg(QPixmap pix);

private:
    Ui::GenerateReferenceMap *ui;

    int pageIndix;
    bool buttonIdxArr[6] = {false,false,false,false,false,false};
    int ret;
};

#endif // GENERATEREFERENCEMAP_H
