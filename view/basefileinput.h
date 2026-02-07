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
