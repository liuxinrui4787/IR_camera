<<<<<<< HEAD
﻿#ifndef QGRAPHICSVIEWIFR_H
#define QGRAPHICSVIEWIFR_H

#include <QGraphicsView>
#include <QRubberBand>
#include <QWidget>
#include <opencv2/core.hpp>

namespace Ui {
class QGraphicsViewIFR;
}

class QGraphicsViewIFR : public QGraphicsView
{
    Q_OBJECT

public:
    explicit QGraphicsViewIFR(QWidget *parent = nullptr);
    QPixmap getPixmap()
    {
        return pixmap;
    }
    void showlimg(QPixmap pix);
    ~QGraphicsViewIFR();
    void setPixmap(QPixmap pix);
signals:
    void SignalSelectRect(cv::Rect);
    void SignalDesignRect(cv::Rect);
    void SignalDeleteRect(cv::Rect);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void MousePressHandle(QMouseEvent *event);
    void MouseMoveHandle(QMouseEvent *event);
    void MouseReleaseHandle(QMouseEvent *event);
    void appendPoints(QPointF p);
    void ScaleImage(float factor);
public slots:
    void slotlimg(QPixmap pix);
private slots:
//    void slotlimg(QPixmap pix);
    void slotrimg(QPixmap pix);
    void SlotCapScreen(int);
private:
    Ui::QGraphicsViewIFR *ui;
    QPoint prev_mouse_pos_;
    QGraphicsScene *scenc;
    QGraphicsPixmapItem *pixItem;
    QRubberBand *rubber;
    QPixmap pixmap;
    QList<QPointF> drawParamList;
    int isrubber = 0;
    float m_scaleFactor=1;
    cv::Rect mat_rect;


};

#endif // QGRAPHICSVIEWIFR_H
=======
﻿#ifndef QGRAPHICSVIEWIFR_H
#define QGRAPHICSVIEWIFR_H

#include <QGraphicsView>
#include <QRubberBand>
#include <QWidget>
#include <opencv2/core.hpp>

namespace Ui {
class QGraphicsViewIFR;
}

class QGraphicsViewIFR : public QGraphicsView
{
    Q_OBJECT

public:
    explicit QGraphicsViewIFR(QWidget *parent = nullptr);
    QPixmap getPixmap()
    {
        return pixmap;
    }
    void showlimg(QPixmap pix);
    ~QGraphicsViewIFR();
    void setPixmap(QPixmap pix);
signals:
    void SignalSelectRect(cv::Rect);
    void SignalDesignRect(cv::Rect);
    void SignalDeleteRect(cv::Rect);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void MousePressHandle(QMouseEvent *event);
    void MouseMoveHandle(QMouseEvent *event);
    void MouseReleaseHandle(QMouseEvent *event);
    void appendPoints(QPointF p);
    void ScaleImage(float factor);
public slots:
    void slotlimg(QPixmap pix);
private slots:
//    void slotlimg(QPixmap pix);
    void slotrimg(QPixmap pix);
    void SlotCapScreen(int);
private:
    Ui::QGraphicsViewIFR *ui;
    QPoint prev_mouse_pos_;
    QGraphicsScene *scenc;
    QGraphicsPixmapItem *pixItem;
    QRubberBand *rubber;
    QPixmap pixmap;
    QList<QPointF> drawParamList;
    int isrubber = 0;
    float m_scaleFactor=1;
    cv::Rect mat_rect;


};

#endif // QGRAPHICSVIEWIFR_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
