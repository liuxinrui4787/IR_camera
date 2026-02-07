#ifndef LOGPAGETEST_H
#define LOGPAGETEST_H

#include <QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>

namespace Ui {
class LogpageTest;
}

class LogpageTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogpageTest(QWidget *parent = nullptr);
    ~LogpageTest();
    Ui::LogpageTest *ui;
    QTextBrowser * denTextBrowser;
    QTextBrowser * disinputBrowser;
    QTextBrowser * disparaBrowser;
    QTextBrowser * logBrowser;

private slots:
    void slotLog(int,QString);

private:


};

#endif // LOGPAGETEST_H
