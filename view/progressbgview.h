#ifndef PROGRESSBGVIEW_H
#define PROGRESSBGVIEW_H

#include <QPaintEvent>
#include <QPropertyAnimation>
#include <QWidget>
/******************************************************************************
 * Copyright(C),2020-2021,National University of Defense Technology .
 * All right reserved. See COPYRIGHT for detailed Information.
 * FileName:   progressbgview.h
 * Author:     guohuan<guohuan@qq.com>
 * Version:    1.0.0
 * Date:       2022/06/13
 * Description: //背景显示
 *****************************************************************************/

class ProgressBGView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int angle WRITE setAngle READ angle)

  public:
    explicit ProgressBGView(QWidget *parent = 0);

    void setAngle(int angle);
    int angle() const;
    void setIsLoading(bool isLoading);
  signals:

  public slots:

  protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

  private:
    bool m_isLoading = false;
    int m_angle;
    QPixmap m_pic;
    QColor m_bgColor;
    QColor m_borderColor;
};

#endif // PROGRESSBGVIEW_H
