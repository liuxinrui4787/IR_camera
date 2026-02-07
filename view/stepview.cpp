#include "stepview.h"
#include "components/qtmaterialstyle.h"
#include "ui_stepview.h"
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>

StepView::StepView(QWidget *parent)
    : ProgressBGView(parent),
      ui(new Ui::StepView)
{
    ui->setupUi(this);

    this->ui->icon->setAttribute(Qt::WA_TranslucentBackground);
    this->ui->name->setAttribute(Qt::WA_TranslucentBackground);
    this->ui->status->setAttribute(Qt::WA_TranslucentBackground);
    this->ui->numsWieget->setAttribute(Qt::WA_TranslucentBackground);
    this->ui->bg->setAttribute(Qt::WA_TranslucentBackground);
    this->ui->nums->setAttribute(Qt::WA_TranslucentBackground);

    this->ui->icon_status->setAttribute(Qt::WA_TranslucentBackground);

    this->ui->numsWieget->setVisible(false);

    //    this->ui->icon->setPixmap(QPixmap(":/image/puzzle.svg"));

    //    QImage *image = new QImage(":/image/puzzle.svg");
    //    this->ui->icon->setPixmap(QPixmap::fromImage(image));

    // setIcon(":/image/puzzle.svg");

    setSuccess();
    //    setWatting(2);
    //    setLoading();
}

void StepView::setSuccess() //完成
{
    this->m_status = 2;
    this->setIsLoading(false);
    this->ui->numsWieget->setVisible(false);
    this->ui->status->setText(u8"完成");
    this->ui->icon_widget->setVisible(true);
    this->ui->icon_status->setPixmap(QPixmap(":/image/icon_success.png"));
}

void StepView::setError() //出错
{
    this->m_status = -1;
    this->setIsLoading(false);
    this->ui->numsWieget->setVisible(false);
    this->ui->status->setText(u8"出错");
    this->ui->icon_widget->setVisible(true);
    this->ui->icon_status->setPixmap(QPixmap(":/image/icon_error.png"));
}

void StepView::setLoading() //加载中
{
    qDebug()<<"loading start";
    this->m_status = 1;
    this->setIsLoading(true);
    this->ui->numsWieget->setVisible(false);
    this->ui->status->setText(u8"分析中");
    //    this->ui->icon->setPixmap(svgImageWithColor(m_iconPath, this->ui->icon->size(), "grey"));
    this->ui->icon_widget->setVisible(true);
    this->ui->icon_status->setPixmap(QPixmap(":/image/icon_waitting.png"));
    setIcon(this->m_iconPath);
}

void StepView::setWaiting(int nums) //等待中
{
    this->m_status = 0;
    this->ui->numsWieget->setVisible(true);
    this->ui->icon_widget->setVisible(false);
    this->ui->nums->setText(QString::number(nums));
    this->ui->bg->setPixmap(svgImageWithColor(":/image/bg_nums.svg", QSize(25, 25), "grey"));
    this->ui->status->setText(u8"等待中");
    this->ui->icon->setPixmap(svgImageWithColor(m_iconPath, this->ui->icon->size(), "grey"));
}

void StepView::setIsWaitForConfig(bool isConfig)
{
    if (isConfig)
    {
        if (this->m_status == 2)//Success
        {
            this->ui->status->setText(u8"完成");
            this->ui->icon_status->setPixmap(QPixmap(":/image/icon_success.png"));
        }else
        {
            this->ui->icon->setPixmap(svgImageWithColor(m_iconPath, this->ui->icon->size(), hexColor(QtMaterialStyle::instance().themeColor("primary1"))));
            this->ui->status->setText(u8"等待配置");
            this->ui->icon_widget->setVisible(true);
            this->ui->icon_status->setPixmap(QPixmap(":/image/icon_waitting.png"));
            this->ui->numsWieget->setVisible(false);
            this->ui->nums->setVisible(false);
        }
    }
    else
    {
        if (this->m_status == 0)//waitting
        {
            this->ui->icon->setPixmap(svgImageWithColor(m_iconPath, this->ui->icon->size(), "grey"));
            this->ui->status->setText(u8"等待中");
            this->ui->icon_widget->setVisible(false);
            this->ui->numsWieget->setVisible(true);
            this->ui->nums->setVisible(true);
        }
        else if (this->m_status == 1)//loading
        {
            this->ui->status->setText(u8"分析中");
        }else if (this->m_status == 2)//Success
        {
            this->ui->status->setText(u8"完成");
            this->ui->icon_status->setPixmap(QPixmap(":/image/icon_success.png"));
        }
    }
}

void StepView::setIcon(QString icon)
{
    m_iconPath = icon;

    this->ui->icon->setPixmap(svgImageWithColor(m_iconPath, this->ui->icon->size(), hexColor(QtMaterialStyle::instance().themeColor("primary1"))));
}

void StepView::setText(QString name) //设置步骤名称
{
    this->ui->name->setText(name);
}

QString StepView::hexColor(QColor color)
{
    //    QColor color = QtMaterialStyle::instance().themeColor("primary1");
    //    QRgb mRgb = qRgb(color.red(), color.green(), color.blue());
    QString r1 = QString::number(color.red(), 16);
    if (r1.length() != 2)
    {
        r1 = "0" + r1;
    }

    QString g1 = QString::number(color.green(), 16);
    if (g1.length() != 2)
    {
        g1 = "0" + r1;
    }

    QString b1 = QString::number(color.blue(), 16);
    if (b1.length() != 2)
    {
        b1 = "0" + r1;
    }
    return "#" + r1 + g1 + b1;
}

void StepView::SetSVGBackColor(QDomElement elem, QString strtagname, QString strattr, QString strattrval)
{
    if (elem.tagName().compare(strtagname) == 0)
    {
        QString before_color = elem.attribute(strattr);
        if (before_color == "none" || before_color.isEmpty())
            return;
        elem.setAttribute(strattr, strattrval);
    }
    for (int i = 0; i < elem.childNodes().count(); i++)
    {
        if (!elem.childNodes().at(i).isElement())
        {
            continue;
        }
        SetSVGBackColor(elem.childNodes().at(i).toElement(), strtagname, strattr, strattrval);
    }
}

QPixmap StepView::svgImageWithColor(QString path, QSize size, QString color)
{

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    QDomDocument doc;
    doc.setContent(data);
    file.close();

    SetSVGBackColor(doc.documentElement(), "path", "fill", color);
    SetSVGBackColor(doc.documentElement(), "path", "stroke", color);
    SetSVGBackColor(doc.documentElement(), "circle", "stroke", color);
    SetSVGBackColor(doc.documentElement(), "line", "stroke", color);
    SetSVGBackColor(doc.documentElement(), "polygon", "stroke", color);
    SetSVGBackColor(doc.documentElement(), "polygon", "fill", color);

    QByteArray svg_content = doc.toByteArray();
    QSvgRenderer *svg_render = new QSvgRenderer(svg_content);
    QPixmap *pixmap = new QPixmap(size);
    pixmap->fill(Qt::transparent);
    QPainter painter(pixmap);
    svg_render->render(&painter);
    return *pixmap;
}

StepView::~StepView()
{
    delete ui;
}
