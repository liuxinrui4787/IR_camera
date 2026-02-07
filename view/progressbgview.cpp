#include "progressbgview.h"
#include <QConicalGradient>
#include <QPainter>
#include <QStyleOption>

ProgressBGView::ProgressBGView(QWidget *parent)
    : QWidget(parent)
{
    m_angle = 0;

    m_bgColor = QColor("#f8fafb");
    m_borderColor = QColor("#a6a6a6");

    QPropertyAnimation *animation = new QPropertyAnimation(this);
    animation->setPropertyName("angle");
    animation->setLoopCount(-1);
    animation->setTargetObject(this);
    animation->setStartValue(0);
    animation->setEndValue(360);
    animation->setDuration(3600);

    animation->start();

    m_pic = QPixmap(":/image/loading.jpg");
    //    setIsLoading(true);
}

void ProgressBGView::setAngle(int angle)
{
    m_angle = angle;
    update();
}

int ProgressBGView::angle() const
{
    return m_angle;
}

void ProgressBGView::setIsLoading(bool isLoading)
{
    m_isLoading = isLoading;
    update();
}

void ProgressBGView::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event)

    QPainter painter(this);
    //    painter.setRenderHint(QPainter::Antialiasing);
    //    QPainterPath path;
    //    path.addRect(0,0,this->width(),this->height());
    painter.fillRect(this->rect(), m_borderColor);

    if (m_isLoading)
    {
        painter.save();
        painter.translate(this->width() / 2, this->height() / 2);
        painter.scale(2, 2);
        painter.rotate(m_angle);
        painter.translate(-this->width() / 2, -this->height() / 2);

        //    QPixmap pic;
        //    pic.load(":/image/icon.png");
        //    = new QPixmap(":/image/icon.png");
        painter.drawPixmap(QRect(0, 0, this->width(), this->height()), m_pic);
        painter.restore();
    }

    painter.fillRect(QRect(3, 3, this->width() - 6, this->height() - 6), m_bgColor);

    QWidget::paintEvent(event);
    //    const qreal cr = d->cornerRadius;

    //    if (cr > 0)
    //    {
    //        QPainterPath path;
    //        path.addRoundedRect(rect(), cr, cr);

    //        painter.setClipPath(path);
    //        painter.setClipping(true);
    //    }

    //    paintBackground(&painter);
    //    paintHalo(&painter);

    //    painter.setOpacity(1);
    //    painter.setClipping(false);

    //    paintForeground(&painter);
}
