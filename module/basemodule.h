#ifndef BASEMODULE_H
#define BASEMODULE_H

#include "components/qtmaterialflatbutton.h"
#include <QWidget>
namespace Ui
{
class BaseModule;
}

class BaseModule : public QWidget
{
    Q_OBJECT

  public:
    explicit BaseModule(QWidget *parent = nullptr);
    ~BaseModule();
    void init();

  protected:
    void updateButton(QtMaterialFlatButton *btn, QString iconPath);

  private:
    Ui::BaseModule *ui;
};

#endif // BASEMODULE_H
