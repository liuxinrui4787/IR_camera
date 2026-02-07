#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basewindow.h"
#include <QThread>
#include "opencv2/core.hpp"
#include "components/qtmaterialdialog.h"
#include "view/qgraphicsviewifr.h"
#include "view/stepview.h"
#include "logpagetest.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public BaseWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    LogpageTest* log =new LogpageTest();

  protected:
    void init();

  signals:
    void currentShowPage(int);
    void logStart();

    void signalLog(int,QString);


  private slots:
    void setDebug(bool);
    void onModuleSelect(int);
    void on_stepDebug1_clicked();

    void on_stepDebug2_clicked();

    void on_stepDebug3_clicked();


    void showWindowL(int viewId);
    void showWindowR(int viewId);
    void startProgressViewId(int id);
    void completeViewId(int id);
    void jumpViewId(int id);
    void actualParaImg(QPixmap);
    void dismapImg(QPixmap);
    void slotSetLog(int,QString);

    void on_btnExit_2_clicked();

    void receiveDenFromcol(QString);
    void receivedirectStrFrom(QString);
    void receivePathFromConMain(QString);
    void receiveYichangFromLog(QString);

private:
    Ui::MainWindow *ui;
    QList<QGraphicsView*> GraphicsListL;
    QList<QGraphicsView*> GraphicsListR;
    QList<StepView *> stepList;

};
#endif // MAINWINDOW_H
