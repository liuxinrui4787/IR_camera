<<<<<<< HEAD
#ifndef AUTOLINEFINDER_H
#define AUTOLINEFINDER_H

#include <QWidget>

namespace Ui {
class AutoLineFinder;
}

class AutoLineFinder : public QWidget
{
    Q_OBJECT

public:
    explicit AutoLineFinder(QWidget *parent = nullptr);
    ~AutoLineFinder();
signals:
    void pushAutoLine();
private slots:
    void on_pushButton_clicked();

private:
    Ui::AutoLineFinder *ui;
};

#endif // AUTOLINEFINDER_H
=======
#ifndef AUTOLINEFINDER_H
#define AUTOLINEFINDER_H

#include <QWidget>

namespace Ui {
class AutoLineFinder;
}

class AutoLineFinder : public QWidget
{
    Q_OBJECT

public:
    explicit AutoLineFinder(QWidget *parent = nullptr);
    ~AutoLineFinder();
signals:
    void pushAutoLine();
private slots:
    void on_pushButton_clicked();

private:
    Ui::AutoLineFinder *ui;
};

#endif // AUTOLINEFINDER_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
