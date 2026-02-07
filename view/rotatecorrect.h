#ifndef ROTATECORRECT_H
#define ROTATECORRECT_H

#include <QWidget>

namespace Ui {
class rotateCorrect;
}

class rotateCorrect : public QWidget
{
    Q_OBJECT

public:
    explicit rotateCorrect(QWidget *parent = nullptr);
    ~rotateCorrect();

private:
    Ui::rotateCorrect *ui;
};

#endif // ROTATECORRECT_H
