#include "tipsview.h"
#include "ui_tipsview.h"
#include <QDebug>

TipsView::TipsView(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::TipsView), m_isSetGloblPos(false)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_Hover, true);
    this->setMouseTracking(true);

    this->m_tipsWords = "";

    //    public:
    //      void setTipsWords(QString);
    //    signals:
    //      void showTipsWords(QString, QPoint);
    //      void hideTipsWords();
}

TipsView::~TipsView()
{
    delete ui;
}

void TipsView::setTipsWords(QString words)
{
    this->m_tipsWords = words;
}

void TipsView::setGloblPos(QPoint pos)
{
    this->m_isSetGloblPos = true;
    this->m_globlPos = pos;
}

bool TipsView::event(QEvent *event)
{
    if (event->type() == QEvent::HoverEnter)
    {

        if (this->m_isSetGloblPos)
        {
            emit showTipsWords(this->m_tipsWords, this->m_globlPos);
        }
        else
        {
            emit showTipsWords(this->m_tipsWords, QPoint(x(), y()));
        }
        //        emit showTipsWords(this->m_tipsWords, mapToParent(QPoint(x(), y())));
    }
    else if (event->type() == QEvent::HoverLeave)
    {
        emit hideTipsWords();
    }
    return QWidget::event(event);
}
