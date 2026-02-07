<<<<<<< HEAD
#ifndef LOGPAGE_H
#define LOGPAGE_H

#include <QWidget>

namespace Ui {
class logPage;
}

class logPage : public QWidget
{
    Q_OBJECT

public:
    explicit logPage(QWidget *parent = nullptr);
    ~logPage();

private:
    Ui::logPage *ui;
};

#endif // LOGPAGE_H
=======
#ifndef LOGPAGE_H
#define LOGPAGE_H

#include <QWidget>

namespace Ui {
class logPage;
}

class logPage : public QWidget
{
    Q_OBJECT

public:
    explicit logPage(QWidget *parent = nullptr);
    ~logPage();

private:
    Ui::logPage *ui;
};

#endif // LOGPAGE_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
