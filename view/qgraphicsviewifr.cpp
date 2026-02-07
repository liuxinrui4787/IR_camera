#include "qgraphicsviewifr.h"
#include "ui_qgraphicsviewifr.h"
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QScrollBar>
#include <QDebug>

QGraphicsViewIFR::QGraphicsViewIFR(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::QGraphicsViewIFR)
{
    ui->setupUi(this);
    scenc = new QGraphicsScene();
    pixItem = new QGraphicsPixmapItem();
    scenc->addItem(pixItem);
    this->setScene(scenc);
    this->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    this->fitInView(pixItem,Qt::KeepAspectRatioByExpanding);
    //鼠标拖动框
    rubber = new QRubberBand(QRubberBand::Rectangle,this);
}

QGraphicsViewIFR::~QGraphicsViewIFR()
{
    delete ui;
}

void QGraphicsViewIFR::mousePressEvent(QMouseEvent *event)
{
    if(isrubber > 0)
    {
        // 按键判断
        if (event->button() == Qt::RightButton) { // 鼠标右键
            MousePressHandle(event);
        } else if (event->button() == Qt::MiddleButton) { // 鼠标中键
            MousePressHandle(event);
        } else if (event->button() == Qt::LeftButton) { // 鼠标左键
            MousePressHandle(event);
        }
        prev_mouse_pos_ = event->pos();
        rubber->setGeometry(QRect(prev_mouse_pos_,QSize()));
        rubber->show();
    }
}

void QGraphicsViewIFR::mouseMoveEvent(QMouseEvent *event)
{
    if(isrubber > 0)
    {
        // 按键判断
        if (event->buttons() & Qt::RightButton) { // 鼠标右键拖动
            MouseMoveHandle(event);
        } else if (event->buttons() & Qt::MiddleButton) { // 鼠标中键拖动
            MouseMoveHandle(event);
        } else if (event->buttons() & Qt::LeftButton) { // 鼠标左键拖动
            MouseMoveHandle(event);
        } else {
            QGraphicsView::mouseMoveEvent(event);
        }
        rubber->setGeometry(QRect(prev_mouse_pos_,event->pos()).normalized());
    }
}

void QGraphicsViewIFR::mouseReleaseEvent(QMouseEvent *event)
{
    if(isrubber > 0)
    {
        // 按键判断
        if (event->button() == Qt::RightButton) { // 鼠标右键
            MouseReleaseHandle(event);
        } else if (event->button() == Qt::MiddleButton) { // 鼠标中键
            MouseReleaseHandle(event);
        } else if (event->button() == Qt::LeftButton) { // 鼠标左键
            MouseReleaseHandle(event);
        }
        //    rubber->hide();
    }
}
void QGraphicsViewIFR::wheelEvent(QWheelEvent *event)
{
    if(!pixmap.isNull())
    {
        if(event->angleDelta().y()>0)//y值正数为滚轮向下
        {
            ScaleImage(0.05);//正数为缩小
        }else
        {
            ScaleImage(-0.05);
        }
    }
    scenc->setSceneRect(0,0,m_scaleFactor*pixmap.width(),m_scaleFactor*pixmap.height());
}

void QGraphicsViewIFR::keyPressEvent(QKeyEvent *event)
{
    // Esc 键退出;
    if (event->key() == Qt::Key_Escape)
    {
        rubber->hide();
    }
    // Eeter键完成;
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if(isrubber == 1)
        {
            SignalSelectRect(mat_rect);
        }else if(isrubber == 2)
        {
            SignalDesignRect(mat_rect);
        }else if(isrubber == 3)
        {
            SignalDeleteRect(mat_rect);
        }
        rubber->hide();
    }
}
void QGraphicsViewIFR::ScaleImage(float factor)
{
    m_scaleFactor += factor;
    //QGraphicsView执行放缩
    if (m_scaleFactor < 0.07 || m_scaleFactor > 100)
            return;
    pixItem->setScale(m_scaleFactor);
}

void QGraphicsViewIFR::MousePressHandle(QMouseEvent *event)
{
    setDragMode(QGraphicsView::RubberBandDrag);
    QPointF sp = mapToScene(event->pos());// 鼠标坐标(映射到场景)
    QPointF ip = mapFromScene(sp);// 图片坐标
    int offset_x =horizontalScrollBar()->value();
    int offset_y =verticalScrollBar()->value();
    QPointF p = QPointF(ip.x()+offset_x,ip.y()+offset_y);
    appendPoints(QPointF(p/m_scaleFactor));
}

void QGraphicsViewIFR::MouseMoveHandle(QMouseEvent *event)
{
//    QPointF sp = mapToScene(event->pos());// 鼠标坐标(映射到场景)
//    QPointF ip = mapFromScene(sp);// 图片坐标
//    int offset_x =horizontalScrollBar()->value();
//    int offset_y =verticalScrollBar()->value();
//    QPointF p = QPointF(ip.x()+offset_x,ip.y()+offset_y);
//    appendPoints(QPointF(p/m_scaleFactor));
}

void QGraphicsViewIFR::MouseReleaseHandle(QMouseEvent *event)
{
    QPointF sp = mapToScene(event->pos());// 鼠标坐标(映射到场景)
    QPointF ip = mapFromScene(sp);// 图片坐标
    int offset_x =horizontalScrollBar()->value();
    int offset_y =verticalScrollBar()->value();
    QPointF p = QPointF(ip.x()+offset_x,ip.y()+offset_y);
    appendPoints(QPointF(p/m_scaleFactor));

    if (drawParamList.count() >= 2) {
        int x = (drawParamList[0].x() < drawParamList[1].x()) ? drawParamList[0].x() : drawParamList[1].x();
        int y = (drawParamList[0].y() < drawParamList[1].y()) ? drawParamList[0].y() : drawParamList[1].y();
        int w = qAbs(drawParamList[0].x() - drawParamList[1].x()) + 1;
        int h = qAbs(drawParamList[0].y() - drawParamList[1].y()) + 1;
        //回传选区
        mat_rect = cv::Rect(x,y,w,h);
    }
    drawParamList.clear();
    if (rubberBandRect().isValid())
    {
        // 局部调整(选区确认后)

    }
}

void QGraphicsViewIFR::appendPoints(QPointF p)
{
    if ( drawParamList.count() == 2 ) {
        drawParamList.removeLast(); // 移除最后一个点
    }
    if(p.x() >= pixmap.width()-1)
    {
        p = QPointF(pixmap.width()-1,p.y());
    }
    if(p.y() >= pixmap.height()-1)
    {
        p = QPointF(p.x(),pixmap.height()-1);
    }
    if(p.x() < 0)
    {
        p = QPointF(0,p.y());
    }
    if(p.y() < 0)
    {
        p = QPointF(p.x(),0);
    }
    drawParamList.append(p);
}
void QGraphicsViewIFR::slotlimg(QPixmap pix)
{
    pixmap = pix;
    pixItem->setPixmap(pixmap);
    float w,h;
    w = pixmap.width();
    h = pixmap.height();
    if(w > h)
    {
        m_scaleFactor = (this->width()-10) / w;
    }else
    {
        m_scaleFactor = (this->height()-10) / h;
    }
    pixItem->setScale(m_scaleFactor);
    scenc->setSceneRect(0,0,m_scaleFactor*w,m_scaleFactor*h);
}

void QGraphicsViewIFR::showlimg(QPixmap pix)
{
    pixmap = pix;
    pixItem->setPixmap(pixmap);
    float w,h;
    w = pixmap.width();
    h = pixmap.height();
    if(w > h)
    {
        m_scaleFactor = (this->width()-10) / w;
    }else
    {
        m_scaleFactor = (this->height()-10) / h;
    }
    pixItem->setScale(m_scaleFactor);
    scenc->setSceneRect(0,0,m_scaleFactor*w,m_scaleFactor*h);
}

void QGraphicsViewIFR::setPixmap(QPixmap pix){
    pixItem->setPixmap(pixmap);
}

void QGraphicsViewIFR::slotrimg(QPixmap pix)
{
    pixmap = pix;
    pixItem->setPixmap(pixmap);
    float w,h;
    w = pixmap.width();
    h = pixmap.height();
    if(w > h)
    {
        m_scaleFactor = (this->width()-10) / w;
    }else
    {
        m_scaleFactor = (this->height()-10) / h;
    }
    pixItem->setScale(m_scaleFactor);
    scenc->setSceneRect(0,0,m_scaleFactor*pixmap.width(),m_scaleFactor*pixmap.height());
}
void QGraphicsViewIFR::SlotCapScreen(int i)
{
    isrubber = i;
    if(i <= 0)
    {
        rubber->hide();
    }
}
