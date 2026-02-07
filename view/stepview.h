#ifndef STEPVIEW_H
#define STEPVIEW_H

#include "view/progressbgview.h"
#include <QDomDocument>
#include <QWidget>

namespace Ui
{
class StepView;
}

class StepView : public ProgressBGView
{
    Q_OBJECT

  public:
    explicit StepView(QWidget *parent = 0);
    ~StepView();
    void setSuccess(); //完成
    void setError(); //出错
    void setLoading(); //加载中
    void setWaiting(int nums); //等待中
    void setIsWaitForConfig(bool); //设置中
    void setIcon(QString);
    void setText(QString); //设置步骤名称
    QString hexColor(QColor);
    bool isSuccess() const
    {
        return m_status == 2;
    }

    bool isLoading() const
    {
        return m_status == 1;
    }

    bool isWatting() const
    {
        return m_status == 0;
    }

  protected:
    QPixmap svgImageWithColor(QString path, QSize size, QString strattrval);
    void SetSVGBackColor(QDomElement elem, QString strtagname, QString strattr, QString strattrval);

  private:
    QString m_iconPath;
    Ui::StepView *ui;
    int m_status = 0;
};

#endif // STEPVIEW_H
