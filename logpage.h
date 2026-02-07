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
