#ifndef YICHANGTEST_H
#define YICHANGTEST_H

#include <QWidget>

class yichangtest : public QWidget
{
    Q_OBJECT
public:
    explicit yichangtest(QWidget *parent = nullptr);
    void init();
signals:
    void deliveryichangToMain(QString);
};

#endif // YICHANGTEST_H
