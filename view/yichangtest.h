<<<<<<< HEAD
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
=======
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
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
