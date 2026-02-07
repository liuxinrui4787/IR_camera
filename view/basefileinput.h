<<<<<<< HEAD
#ifndef BASEFILEINPUT_H
#define BASEFILEINPUT_H

#include "fileselect.h"
#include "inputfile.h"

#include <QWidget>

namespace Ui {
class BaseFileInput;
}

class BaseFileInput : public QWidget
{
    Q_OBJECT

public:
    explicit BaseFileInput(QWidget *parent = nullptr);
    ~BaseFileInput();
    void recPageIdx(int page);
    inputFile* inputFileObj;
    FileSelect* FileSelectObj;

private:
    Ui::BaseFileInput *ui;
};

#endif // BASEFILEINPUT_H
=======
#ifndef BASEFILEINPUT_H
#define BASEFILEINPUT_H

#include "fileselect.h"
#include "inputfile.h"

#include <QWidget>

namespace Ui {
class BaseFileInput;
}

class BaseFileInput : public QWidget
{
    Q_OBJECT

public:
    explicit BaseFileInput(QWidget *parent = nullptr);
    ~BaseFileInput();
    void recPageIdx(int page);
    inputFile* inputFileObj;
    FileSelect* FileSelectObj;

private:
    Ui::BaseFileInput *ui;
};

#endif // BASEFILEINPUT_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
