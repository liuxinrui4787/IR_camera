#include "appwidget.h"
#include <QDate>
#include <QDesktopWidget>
#include <QFile>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QTextStream>
#include <QTranslator>
#include <QtCore>

#include "basewindow.h"
#include <QGraphicsDropShadowEffect>

QtMaterialSnackbar *AppWidget::mToast = nullptr;
//AppWidget::mToast = 0;
AppWidget::AppWidget(QWidget *parent)
    : QDialog(parent)
{
    this->initControl();
    this->initForm();
}
AppWidget::~AppWidget()
{
    delete widgetMain;
}

int AppWidget::deskWidth()
{

    return QGuiApplication::primaryScreen()->size().width();
    //    return qApp->desktop()->availableGeometry().width();
}

int AppWidget::deskHeight()
{
    return QGuiApplication::primaryScreen()->size().height();
    //    return qApp->desktop()->availableGeometry().height();
}

//程序本身文件名称
QString AppWidget::appName()
{
    QString name = qApp->applicationFilePath();
    QStringList list = name.split("/");
    name = list.at(list.count() - 1).split(".").at(0);
    return name;
}
//程序当前所在路径
QString AppWidget::appPath()
{
#ifdef __arm__
    return "/sdcard/Android/lys";
    //return QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
    return qApp->applicationDirPath();
#endif
}

//void AppWidget::ShowToast(QString msg)
//{
//    AppWidget::mToast->addMessage(msg);
//}

//写入消息到额外的的消息日志文件
void AppWidget::writeInfo(const QString &info, const QString &filePath)
{
    QString fileName = QString("%1/%2/%3_runinfo_%4.txt").arg(AppWidget::appPath()).arg(filePath).arg(AppWidget::appName()).arg(QDate::currentDate().year());

    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    QTextStream stream(&file);
    stream << DATETIME << info << NEWLINE;
    file.close();
}

//设置窗体居中显示
void AppWidget::setFormInCenter(QWidget *frm)
{
    int frmX = frm->width();
    int frmY = frm->height();
    QDesktopWidget w;

    int deskWidth = QGuiApplication::primaryScreen()->size().width();
    int deskHeight = QGuiApplication::primaryScreen()->size().height();
    //    int deskWidth = w.availableGeometry().width();
    //    int deskHeight = w.availableGeometry().height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    frm->move(movePoint);
}
//设置翻译文件
void AppWidget::setTranslator(const QString &qmFile)
{
    QTranslator *translator = new QTranslator(qApp);
    translator->load(qmFile);
    qApp->installTranslator(translator);
}
//弹出消息框

void AppWidget::showMessageBoxInfo(const QString &info, int closeSec)
{
    QUIMessageBox::Instance()->setMessage(info, 0, closeSec);
    QUIMessageBox::Instance()->show();
}
//弹出错误框
void AppWidget::showMessageBoxError(const QString &info, int closeSec)
{
    QUIMessageBox::Instance()->setMessage(info, 2, closeSec);
    QUIMessageBox::Instance()->show();
}

//弹出询问框
QUIMessageBox *AppWidget::showMessageBoxQuestion(const QString &info)
{
    QUIMessageBox::Instance()->setMessage(info, 1);
    QUIMessageBox::Instance()->showDialog();
    return QUIMessageBox::Instance();
}

QString AppWidget::showInputBox(bool &ok, const QString &title, int type, int closeSec,
                                QString defaultValue, bool pwd)
{
    QUIInputBox input;
    input.setParameter(title, type, closeSec, defaultValue, pwd);
    ok = input.exec();
    return input.getValue();
}

QString AppWidget::HexColor(QColor color)
{
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

bool AppWidget::eventFilter(QObject *obj, QEvent *evt)
{
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (event->button() == Qt::LeftButton)
        {
            mousePressed = true;
            mousePoint = event->globalPos() - this->pos();
            return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        mousePressed = false;
        return true;
    }
    else if (event->type() == QEvent::MouseMove)
    {
        if (mousePressed && (event->buttons() == Qt::LeftButton))
        {
            this->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(obj, evt);
}

QPushButton *AppWidget::getBtnMenuMin() const
{
    return this->btnMenu_Min;
}

QLabel *AppWidget::getWinTitle() const
{
    return this->labTitle;
}

QLabel *AppWidget::getLabIco() const
{
    return this->labIco;
}

QPushButton *AppWidget::getBtnMenuMClose() const
{
    return this->btnMenu_Close;
}

QSize AppWidget::sizeHint() const
{
    return QSize(600, 450);
}

QSize AppWidget::minimumSizeHint() const
{
    return QSize(200, 150);
}

void AppWidget::initControl()
{
    this->setObjectName(QString::fromUtf8("QUIWidget"));
    this->resize(900, 750);
    this->setSizeGripEnabled(false);

    //修改成对应的内容
    this->setWindowFlags((Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint));

    //带title的
    verticalLayout1 = new QVBoxLayout(this);
    verticalLayout1->setSpacing(0);
    verticalLayout1->setContentsMargins(11, 11, 11, 11);
    verticalLayout1->setObjectName(QString::fromUtf8("verticalLayout1"));
    verticalLayout1->setContentsMargins(1, 1, 1, 1);

    widgetMain = new QWidget(this);
    widgetMain->setObjectName(QString::fromUtf8("widgetMain"));
    widgetMain->setStyleSheet(QString::fromUtf8(""));
    verticalLayout2 = new QVBoxLayout(widgetMain);
    verticalLayout2->setSpacing(0);
    verticalLayout2->setContentsMargins(11, 11, 11, 11);
    verticalLayout2->setObjectName(QString::fromUtf8("verticalLayout2"));
    verticalLayout2->setContentsMargins(0, 0, 0, 0);

    widgetTitle = new QWidget(widgetMain);
    widgetTitle->setObjectName(QString::fromUtf8("widgetTitle"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(widgetTitle->sizePolicy().hasHeightForWidth());
    widgetTitle->setSizePolicy(sizePolicy);
    widgetTitle->setMinimumSize(QSize(0, 30));
    widgetTitle->setStyleSheet("background:#0E1A32");

    QHBoxLayout *horizontalLayout4 = new QHBoxLayout(widgetTitle);
    horizontalLayout4->setSpacing(0);
    horizontalLayout4->setObjectName(QString::fromUtf8("horizontalLayout4"));
    horizontalLayout4->setContentsMargins(5, 0, 0, 0);

    labIco = new QLabel(widgetTitle);
    labIco->setObjectName(QString::fromUtf8("labIco"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(labIco->sizePolicy().hasHeightForWidth());
    labIco->setSizePolicy(sizePolicy1);
    labIco->setMinimumSize(QSize(60, 0));
    labIco->setAlignment(Qt::AlignCenter);

    horizontalLayout4->addWidget(labIco);

    labTitle = new QLabel(widgetTitle);
    labTitle->setObjectName(QString::fromUtf8("labTitle"));
    QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy2.setHorizontalStretch(1);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(labTitle->sizePolicy().hasHeightForWidth());
    labTitle->setSizePolicy(sizePolicy2);
    labTitle->setAlignment(Qt::AlignLeading | Qt::AlignHCenter | Qt::AlignVCenter);
    labTitle->setStyleSheet("color:#FFFFFF;\nfont:20px \"微软雅黑\"");

    horizontalLayout4->addWidget(labTitle);

    QWidget *widgetMenu = new QWidget(widgetTitle);
    widgetMenu->setObjectName(QString::fromUtf8("widgetMenu"));
    sizePolicy1.setHeightForWidth(widgetMenu->sizePolicy().hasHeightForWidth());
    widgetMenu->setSizePolicy(sizePolicy1);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(widgetMenu);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    btnMenu = new QToolButton(widgetMenu);
    btnMenu->setObjectName(QString::fromUtf8("btnMenu"));
    QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(btnMenu->sizePolicy().hasHeightForWidth());
    btnMenu->setSizePolicy(sizePolicy3);
    btnMenu->setMinimumSize(QSize(30, 0));
    btnMenu->setMaximumSize(QSize(30, 16777215));
    btnMenu->setFocusPolicy(Qt::NoFocus);
    btnMenu->setPopupMode(QToolButton::InstantPopup);

    horizontalLayout->addWidget(btnMenu);

    //    m_button(new QtMaterialIconButton(QtMaterialTheme::icon("toggle", "star")))

    btnMenu_Min = new QtMaterialFlatButton(widgetMenu);
    btnMenu_Min->setBackgroundMode(Qt::TransparentMode);

    QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(btnMenu_Min->sizePolicy().hasHeightForWidth());
    btnMenu_Min->setSizePolicy(sizePolicy4);
    btnMenu_Min->setMinimumSize(QSize(30, 0));
    btnMenu_Min->setMaximumSize(QSize(30, 16777215));
    btnMenu_Min->setCursor(QCursor(Qt::ArrowCursor));
    btnMenu_Min->setFocusPolicy(Qt::NoFocus);
    btnMenu_Min->setIcon(QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_TitleBarMinButton));
    btnMenu_Min->setTextAlignment(Qt::AlignHCenter);
    btnMenu_Min->setText("");
    btnMenu_Min->setForegroundColor(Qt::white);
    btnMenu_Min->setOverlayColor(QColor(51, 127, 209, 230));

    horizontalLayout->addWidget(btnMenu_Min);

    btnMenu_Close = new QtMaterialFlatButton(widgetMenu);
    btnMenu_Close->setBackgroundMode(Qt::TransparentMode);
    //    btnMenu_Close = new QPushButton(widgetMenu);
    btnMenu_Close->setObjectName(QString::fromUtf8("btnMenu_Close"));
    sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
    btnMenu_Close->setSizePolicy(sizePolicy3);
    btnMenu_Close->setMinimumSize(QSize(30, 0));
    btnMenu_Close->setMaximumSize(QSize(30, 16777215));
    btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
    btnMenu_Close->setFocusPolicy(Qt::NoFocus);
    btnMenu_Close->setTextAlignment(Qt::AlignHCenter);
    btnMenu_Close->setIcon(QIcon(":/image/icon_close.svg"));
    btnMenu_Close->setText("");
    btnMenu_Close->setForegroundColor(Qt::white);
    btnMenu_Close->setOverlayColor(QColor(238, 0, 0, 128));

    horizontalLayout->addWidget(btnMenu_Close);
    horizontalLayout4->addWidget(widgetMenu);
    verticalLayout2->addWidget(widgetTitle);

    widget = new QWidget(widgetMain);
    widget->setObjectName(QString::fromUtf8("widget"));

    QVBoxLayout *verticalLayout3 = new QVBoxLayout(widget);
    verticalLayout3->setSpacing(0);
    verticalLayout3->setContentsMargins(11, 11, 11, 11);
    verticalLayout3->setObjectName(QString::fromUtf8("verticalLayout3"));
    verticalLayout3->setContentsMargins(0, 0, 0, 0);

    verticalLayout2->addWidget(widget);
    verticalLayout1->addWidget(widgetMain);

    connect(this->btnMenu_Min, SIGNAL(clicked(bool)), this, SLOT(on_btnMenu_Min_clicked()));
    connect(this->btnMenu_Close, SIGNAL(clicked(bool)), this, SLOT(on_btnMenu_Close_clicked()));

    AppWidget::mToast = new QtMaterialSnackbar(this);
    AppWidget::mToast->setParent(this);
    QUIMessageBox::Instance()->setParent(this);

    //不带title标题栏的

    //只有一个主体
    //    widgetMain = new QFrame(this);
    //    widgetMain->setObjectName(QString::fromUtf8("frame"));

    //    btnMenu = new QToolButton(widgetMain);
    //    btnMenu->setObjectName(QString::fromUtf8("btnMenu"));
    //    QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
    //    sizePolicy3.setHorizontalStretch(0);
    //    sizePolicy3.setVerticalStretch(0);
    //    sizePolicy3.setHeightForWidth(btnMenu->sizePolicy().hasHeightForWidth());
    //    btnMenu->setSizePolicy(sizePolicy3);
    //    btnMenu->setMinimumSize(QSize(30, 0));
    //    btnMenu->setMaximumSize(QSize(30, 16777215));
    //    btnMenu->setFocusPolicy(Qt::NoFocus);
    //    btnMenu->setPopupMode(QToolButton::InstantPopup);
    //    btnMenu->setIcon(QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_TitleBarMenuButton));
    //    btnMenu->setVisible(false);

    //    btnMenu_Min = new QPushButton(widgetMain);
    //    btnMenu_Min->setObjectName(QString::fromUtf8("btnMenu_Min"));
    //    QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Expanding);
    //    sizePolicy4.setHorizontalStretch(0);
    //    sizePolicy4.setVerticalStretch(0);
    //    sizePolicy4.setHeightForWidth(btnMenu_Min->sizePolicy().hasHeightForWidth());
    //    btnMenu_Min->setSizePolicy(sizePolicy4);
    //    btnMenu_Min->setMinimumSize(QSize(30, 0));
    //    btnMenu_Min->setMaximumSize(QSize(30, 16777215));
    //    btnMenu_Min->setCursor(QCursor(Qt::ArrowCursor));
    //    btnMenu_Min->setFocusPolicy(Qt::NoFocus);
    //    btnMenu_Min->setIcon(QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_TitleBarMinButton));

    //    btnMenu_Close = new QPushButton(widgetMain);
    //    btnMenu_Close->setObjectName(QString::fromUtf8("btnMenu_Close"));
    //    btnMenu_Close->setSizePolicy(sizePolicy4);
    //    btnMenu_Close->setMinimumSize(QSize(30, 0));
    //    btnMenu_Close->setMaximumSize(QSize(30, 16777215));
    //    btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
    //    btnMenu_Close->setFocusPolicy(Qt::NoFocus);
    //    btnMenu_Close->setIcon(QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_TitleBarCloseButton));

    //    btnMenu->move(this->geometry().width() - 90, 0);
    //    btnMenu_Min->move(this->geometry().width() - 60, 0);
    //    btnMenu_Close->move(this->geometry().width() - 30, 0);

    //    btnMenu->raise();
    //    btnMenu_Min->raise();
    //    btnMenu_Close->raise();

    //    widget = new QWidget(widgetMain);
    //    verticalLayout1 = new QVBoxLayout(widget);
    //    verticalLayout1->setSpacing(0);
    //    verticalLayout1->setContentsMargins(11, 11, 11, 11);
    //    verticalLayout1->setObjectName(QString::fromUtf8("verticalLayout1"));
    //    verticalLayout1->setContentsMargins(0, 0, 0, 0);
    //    widget->lower();

    //    AppWidget::mToast = new QtMaterialSnackbar(this);
    //    AppWidget::mToast->setParent(this);
    //    QUIMessageBox::Instance()->setParent(this);

    //    connect(this->btnMenu_Min, SIGNAL(clicked(bool)), this, SLOT(on_btnMenu_Min_clicked()));
    //    connect(this->btnMenu_Close, SIGNAL(clicked(bool)), this, SLOT(on_btnMenu_Close_clicked()));
}

void AppWidget::setWidgetVisible(AppWidget::Widget widget, bool visible)
{
    if (widget == AppWidget::Lab_Ico)
    {
        //        this->labIco->setVisible(visible);
    }
    else if (widget == AppWidget::BtnMenu)
    {
        this->btnMenu->setVisible(visible);
    }
    else if (widget == AppWidget::BtnMenu_Min)
    {
        this->btnMenu_Min->setVisible(visible);
    }
    else if (widget == AppWidget::BtnMenu_Max)
    {
        //        this->btnMenu_Max->setVisible(visible);
    }
    else if (widget == AppWidget::BtnMenu_Close)
    {
        this->btnMenu_Close->setVisible(visible);
    }
}

void AppWidget::initForm()
{

    //    //    setIcon(QUIConfig::Lab_Ico, QUIConfig::IconMain, 11);
    //    //    setIcon(QUIConfig::BtnMenu, QUIConfig::IconMenu);
    //    setIcon(AppWidget::BtnMenu_Min, QUIConfig::IconMin);
    //    //    setIcon(QUIConfig::BtnMenu_Normal, QUIConfig::IconNormal);
    //    setIcon(AppWidget::BtnMenu_Close, QUIConfig::IconClose);

    this->location = this->geometry();
    this->setProperty("form", true);
    this->setWindowFlags((Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint));

    minHide = false;
    mainWidget = nullptr;

    setWidgetVisible(AppWidget::BtnMenu, false);

    //绑定事件过滤器监听鼠标移动
    this->installEventFilter(this);
}

void AppWidget::setIcon(AppWidget::Widget widget, QChar str, quint32 size)
{
    if (widget == AppWidget::Lab_Ico)
    {
        //        setIconMain(str, size);
    }
    else if (widget == AppWidget::BtnMenu)
    {
        QUIConfig::IconMenu = str;
        IconHelper::Instance()->setIcon(this->btnMenu, str, size);
    }
    else if (widget == AppWidget::BtnMenu_Min)
    {
        QUIConfig::IconMin = str;
        IconHelper::Instance()->setIcon(this->btnMenu_Min, str, size);
    }
    else if (widget == AppWidget::BtnMenu_Max)
    {
        //        QUIConfig::IconMax = str;
        //        IconHelper::Instance()->setIcon(this->btnMenu_Max, str, size);
    }
    else if (widget == AppWidget::BtnMenu_Normal)
    {
        //        QUIConfig::IconNormal = str;
        //        IconHelper::Instance()->setIcon(this->btnMenu_Max, str, size);
    }
    else if (widget == AppWidget::BtnMenu_Close)
    {
        QUIConfig::IconClose = str;
        IconHelper::Instance()->setIcon(this->btnMenu_Close, str, size);
    }
}

void AppWidget::setPixmap(AppWidget::Widget widget, const QString &file, const QSize &size)
{
    QPixmap pix = QPixmap(file);
    //按照宽高比自动缩放
    pix = pix.scaled(size, Qt::KeepAspectRatio);

    if (widget == AppWidget::Lab_Ico)
    {
        //        this->labIco->setPixmap(pix);
    }
    else if (widget == AppWidget::BtnMenu)
    {
        this->btnMenu->setIcon(QIcon(file));
    }
    else if (widget == AppWidget::BtnMenu_Min)
    {
        this->btnMenu_Min->setIcon(QIcon(file));
    }
    else if (widget == AppWidget::BtnMenu_Max)
    {
        //        this->btnMenu_Max->setIcon(QIcon(file));
    }
    else if (widget == AppWidget::BtnMenu_Close)
    {
        this->btnMenu_Close->setIcon(QIcon(file));
    }
}
void AppWidget::setOnlyCloseBtn()
{
    this->btnMenu->setVisible(false);
    this->btnMenu_Min->setVisible(false);
}

void AppWidget::setBtnWidth(int width)
{
    this->btnMenu->setFixedWidth(width);
    this->btnMenu_Min->setFixedWidth(width);
    this->btnMenu_Close->setFixedWidth(width);
}

void AppWidget::setMinHide(bool minHide)
{
    if (this->minHide != minHide)
    {
        this->minHide = minHide;
    }
}

void AppWidget::setTitle(QString title)
{
    if (this->labTitle != nullptr)
    {
        this->labTitle->setText(title);
    }
}

void AppWidget::setMainWidget(QWidget *mainWidget)
{
    //一个QUI窗体对象只能设置一个主窗体
    if (this->mainWidget == nullptr)
    {
        //将子窗体添加到布局
        this->widget->layout()->addWidget(mainWidget);
        //自动设置大小
        this->widget->resize(mainWidget->width(), mainWidget->height());

        this->mainWidget = mainWidget;

        //带TITLE的
        resize(mainWidget->width(), mainWidget->height() + 30);

        //不带TITLE的

        //        resize(mainWidget->width(), mainWidget->height());

        //        btnMenu->move(this->geometry().width() - 90, 0);
        //        btnMenu_Min->move(this->geometry().width() - 60, 0);
        //        btnMenu_Close->move(this->geometry().width() - 30, 0);
    }
}

void AppWidget::setStyleWithFile(const char *qssFile)
{
    QFile file(qssFile);

    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

void AppWidget::setStyleFiles(QString light, QString dark)
{
    this->cssDark = dark;
    this->cssLight = light;
    //    this->setWidgetVisible(AppWidget::BtnMenu, true);
    //    connect(T_Program, SIGNAL(WaitingReadFile(uint)), this,
    //            SLOT(progress_value_change(uint)));
    //    connect(this->getBtnMenu(), SIGNAL(clicked()), this, SLOT(changeDarkMode()));
    QSettings set(QUIConfig::ConfigFile, QSettings::IniFormat);
    set.beginGroup("BaseConfig");
    this->isDarkMode = set.value("dark", false).toBool();
    this->isDarkMode = !this->isDarkMode;
    this->changeDarkMode();
}

//切换黑夜/白夜
void AppWidget::changeDarkMode()
{
    if (cssDark.isEmpty() || cssLight.isEmpty())
        return;
    //    QString cssDark; //黑暗模式的CSS
    //    QString cssLight; //白色模式的CSS
    if (this->isDarkMode)
    {
        //换成白夜模式
        this->setStyleWithFile(cssLight.toStdString().c_str());
        this->isDarkMode = false;
        setIcon(AppWidget::BtnMenu, QChar(0xf186));
        QSettings set(QUIConfig::ConfigFile, QSettings::IniFormat);
        set.beginGroup("BaseConfig");
        set.setValue("dark", false);
        set.endGroup();
    }
    else
    {
        //换成黑夜模式
        this->setStyleWithFile(cssDark.toStdString().c_str());
        this->isDarkMode = true;
        setIcon(AppWidget::BtnMenu, QChar(0xf185));
        QSettings set(QUIConfig::ConfigFile, QSettings::IniFormat);
        set.beginGroup("BaseConfig");
        set.setValue("dark", true);
        set.endGroup();
    }
}

void AppWidget::on_btnMenu_Min_clicked()
{
    if (minHide)
    {
        hide();
    }
    else
    {
        showMinimized();
    }
}

void AppWidget::on_btnMenu_Max_clicked()
{
    //    if (max)
    //    {
    //        this->setGeometry(location);
    //        setIcon(AppWidget::BtnMenu_Normal, QUIConfig::IconNormal);
    //    }
    //    else
    //    {
    //        location = this->geometry();
    //        this->setGeometry(qApp->desktop()->contentsRect());
    //        setIcon(AppWidget::BtnMenu_Max, QUIConfig::IconMax);
    //    }

    //    max = !max;
}

void AppWidget::on_btnMenu_Close_clicked()
{
    emit closing();
    exit(0);
}

QUIMessageBox *QUIMessageBox::self = nullptr;
QUIMessageBox::QUIMessageBox(QWidget *parent)
    : QtMaterialDialog(parent)
{
    this->initControl();
}

QUIMessageBox::~QUIMessageBox()
{
    //    delete widgetMain;
}

void QUIMessageBox::initControl()
{
    this->setObjectName(QString::fromUtf8("QUIMessageBox"));

    //#ifdef __arm__
    //    this->resize(300, 400);
    //#else
    //    this->resize(380, 400);
    //#endif

    QWidget *dialogWidget = new QWidget;
    dialogWidget->setMinimumHeight(100);
    verticalLayout1 = new QVBoxLayout(dialogWidget);
    verticalLayout1->setObjectName(QString::fromUtf8("verticalLayout1"));
    verticalLayout1->setContentsMargins(10, 10, 10, 10);

    QVBoxLayout *dialogLayout = new QVBoxLayout;
    this->setWindowLayout(dialogLayout);
    dialogLayout->addWidget(dialogWidget);

    labTitle = new QLabel(this);
    labTitle->setObjectName(QString::fromUtf8("labTitle"));
    labTitle->setStyleSheet(QString::fromUtf8(""));
    labTitle->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);
    verticalLayout1->addStretch();
    verticalLayout1->addWidget(labTitle);

    labInfo = new QLabel(this);
    labInfo->setObjectName(QString::fromUtf8("labInfo"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(labInfo->sizePolicy().hasHeightForWidth());
    labInfo->setSizePolicy(sizePolicy1);
    labInfo->setMinimumSize(QSize(0, 33));
    labInfo->setScaledContents(false);
    labInfo->setWordWrap(true);

    QFont f(font());
    f.setPointSizeF(20);
    setFont(f);
    labInfo->setFont(f);

    verticalLayout1->addWidget(labInfo);

    verticalLayout1->addStretch(1);

    horizontalLayout1 = new QHBoxLayout();
    horizontalLayout1->setSpacing(0);
    horizontalLayout1->setObjectName(QString::fromUtf8("horizontalLayout1"));
    horizontalLayout1->setContentsMargins(0, 0, 0, 0);
    verticalLayout1->addLayout(horizontalLayout1);

    this->btnCancel = new QtMaterialFlatButton(this);
    this->btnCancel->setHaloVisible(false);
    this->btnCancel->setBackgroundMode(Qt::TransparentMode);
    this->btnCancel->setText("取消");
    this->btnCancel->setMinimumSize(QSize(80, 33));
    this->btnCancel->setMaximumSize(QSize(80, 33));
    this->btnCancel->setFontSize(25);
    this->btnCancel->setRole(Material::Secondary);

    this->btnOk = new QtMaterialFlatButton(this);
    this->btnOk->setHaloVisible(false);
    this->btnOk->setBackgroundMode(Qt::TransparentMode);
    this->btnOk->setText("确定");
    this->btnOk->setMinimumSize(QSize(80, 33));
    this->btnOk->setMaximumSize(QSize(80, 33));
    this->btnOk->setFontSize(25);
    this->btnOk->setRole(Material::Primary);

    horizontalLayout1->addStretch(0);
    horizontalLayout1->addWidget(this->btnCancel);
    horizontalLayout1->addWidget(this->btnOk);

    connect(this->btnOk, SIGNAL(clicked()), this, SLOT(on_btnOk_clicked()));
    connect(this->btnCancel, SIGNAL(clicked()), this, SLOT(on_btnMenu_Close_clicked()));
}

void QUIMessageBox::checkSec()
{
    if (closeSec == 0)
    {
        return;
    }

    if (currentSec < closeSec)
    {
        currentSec++;
    }
    else
    {
        this->close();
    }

    QString str = QString("关闭倒计时 %1 s").arg(closeSec - currentSec + 1);
    //    this->labTime->setText(str);
}

void QUIMessageBox::setMessage(const QString &msg, int type, int closeSec)
{
    Q_UNUSED(type);
    this->closeSec = closeSec;
    this->currentSec = 0;
    //    this->labTime->clear();

    checkSec();

    //    if (type == 0)
    //    {
    //        this->labIcoMain->setStyleSheet("border-image: url(:/image/msg_info.png);");
    //        this->btnCancel->setVisible(false);
    //        this->labTitle->setText("提示");
    //    }
    //    else if (type == 1)
    //    {
    //        this->labIcoMain->setStyleSheet("border-image: url(:/image/msg_question.png);");
    //        this->labTitle->setText("询问");
    //    }
    //    else if (type == 2)
    //    {
    //        this->labIcoMain->setStyleSheet("border-image: url(:/image/msg_error.png);");
    //        this->btnCancel->setVisible(false);
    //        this->labTitle->setText("错误");
    //    }

    this->labInfo->setText(msg);
    this->setWindowTitle(this->labTitle->text());
}

//void QUIMessageBox::closeEvent(QCloseEvent *)
//{
//    closeSec = 0;
//    currentSec = 0;
//}

//bool QUIMessageBox::eventFilter(QObject *obj, QEvent *evt)
//{
//    static QPoint mousePoint;
//    static bool mousePressed = false;

//    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
//    if (event->type() == QEvent::MouseButtonPress)
//    {
//        if (event->button() == Qt::LeftButton)
//        {
//            mousePressed = true;
//            mousePoint = event->globalPos() - this->pos();
//            return true;
//        }
//    }
//    else if (event->type() == QEvent::MouseButtonRelease)
//    {
//        mousePressed = false;
//        return true;
//    }
//    else if (event->type() == QEvent::MouseMove)
//    {
//        if (mousePressed && (event->buttons() == Qt::LeftButton))
//        {
//            this->move(event->globalPos() - mousePoint);
//            return true;
//        }
//    }

//    return QWidget::eventFilter(obj, evt);
//}

void QUIMessageBox::on_btnOk_clicked()
{
    //    done(QMessageBox::Yes);
    //    close();
    emit result(QMessageBox::Yes); //点击返回或者成功的回调;
    this->hideDialog();
}

void QUIMessageBox::on_btnMenu_Close_clicked()
{
    //    done(QMessageBox::No);
    emit result(QMessageBox::No);
    this->hideDialog();
    //    close();
}

//void QUIMessageBox::setIconMain(QChar str, quint32 size)
//{
//    IconHelper::Instance()->setIcon(this->labIco, str, size);
//}

QUIInputBox *QUIInputBox::self = nullptr;
QUIInputBox::QUIInputBox(QWidget *parent)
    : QDialog(parent)
{
    this->initControl();
    this->initForm();
}

QUIInputBox::~QUIInputBox()
{
    delete widgetMain;
}

void QUIInputBox::initControl()
{
    this->setObjectName(QString::fromUtf8("QUIInputBox"));

#ifdef __arm__
    this->resize(300, 160);
#else
    this->resize(280, 150);
#endif

    verticalLayout1 = new QVBoxLayout(this);
    verticalLayout1->setSpacing(0);
    verticalLayout1->setObjectName(QString::fromUtf8("verticalLayout1"));
    verticalLayout1->setContentsMargins(1, 1, 1, 1);
    widgetTitle = new QWidget(this);
    widgetTitle->setObjectName(QString::fromUtf8("widgetTitle"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(widgetTitle->sizePolicy().hasHeightForWidth());
    widgetTitle->setSizePolicy(sizePolicy);
    widgetTitle->setMinimumSize(QSize(0, 30));
    horizontalLayout1 = new QHBoxLayout(widgetTitle);
    horizontalLayout1->setSpacing(0);
    horizontalLayout1->setObjectName(QString::fromUtf8("horizontalLayout1"));
    horizontalLayout1->setContentsMargins(0, 0, 0, 0);
    labIco = new QLabel(widgetTitle);
    labIco->setObjectName(QString::fromUtf8("labIco"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(labIco->sizePolicy().hasHeightForWidth());
    labIco->setSizePolicy(sizePolicy1);
    labIco->setMinimumSize(QSize(30, 0));
    labIco->setAlignment(Qt::AlignCenter);

    horizontalLayout1->addWidget(labIco);

    labTitle = new QLabel(widgetTitle);
    labTitle->setObjectName(QString::fromUtf8("labTitle"));
    labTitle->setStyleSheet(QString::fromUtf8(""));
    labTitle->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);

    horizontalLayout1->addWidget(labTitle);

    labTime = new QLabel(widgetTitle);
    labTime->setObjectName(QString::fromUtf8("labTime"));
    QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(labTime->sizePolicy().hasHeightForWidth());
    labTime->setSizePolicy(sizePolicy2);
    labTime->setAlignment(Qt::AlignCenter);

    horizontalLayout1->addWidget(labTime);

    widgetMenu = new QWidget(widgetTitle);
    widgetMenu->setObjectName(QString::fromUtf8("widgetMenu"));
    sizePolicy1.setHeightForWidth(widgetMenu->sizePolicy().hasHeightForWidth());
    widgetMenu->setSizePolicy(sizePolicy1);
    horizontalLayout2 = new QHBoxLayout(widgetMenu);
    horizontalLayout2->setSpacing(0);
    horizontalLayout2->setObjectName(QString::fromUtf8("horizontalLayout2"));
    horizontalLayout2->setContentsMargins(0, 0, 0, 0);
    btnMenu_Close = new QPushButton(widgetMenu);
    btnMenu_Close->setObjectName(QString::fromUtf8("btnMenu_Close"));
    QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
    btnMenu_Close->setSizePolicy(sizePolicy3);
    btnMenu_Close->setMinimumSize(QSize(30, 0));
    btnMenu_Close->setMaximumSize(QSize(30, 16777215));
    btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
    btnMenu_Close->setFocusPolicy(Qt::NoFocus);
    btnMenu_Close->setFlat(true);

    horizontalLayout2->addWidget(btnMenu_Close);
    horizontalLayout1->addWidget(widgetMenu);
    verticalLayout1->addWidget(widgetTitle);

    widgetMain = new QWidget(this);
    widgetMain->setObjectName(QString::fromUtf8("widgetMain"));
    widgetMain->setStyleSheet(QString::fromUtf8(""));
    verticalLayout2 = new QVBoxLayout(widgetMain);
    verticalLayout2->setSpacing(5);
    verticalLayout2->setObjectName(QString::fromUtf8("verticalLayout2"));
    verticalLayout2->setContentsMargins(5, 5, 5, 5);
    frame = new QFrame(widgetMain);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Sunken);
    verticalLayout3 = new QVBoxLayout(frame);
    verticalLayout3->setObjectName(QString::fromUtf8("verticalLayout3"));
    labInfo = new QLabel(frame);
    labInfo->setObjectName(QString::fromUtf8("labInfo"));
    labInfo->setScaledContents(false);
    labInfo->setWordWrap(true);

    verticalLayout3->addWidget(labInfo);

    txtValue = new QLineEdit(frame);
    txtValue->setObjectName(QString::fromUtf8("txtValue"));

    verticalLayout3->addWidget(txtValue);

    cboxValue = new QComboBox(frame);
    cboxValue->setObjectName(QString::fromUtf8("cboxValue"));

    verticalLayout3->addWidget(cboxValue);

    lay = new QHBoxLayout();
    lay->setObjectName(QString::fromUtf8("lay"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    lay->addItem(horizontalSpacer);

    btnOk = new QPushButton(frame);
    btnOk->setObjectName(QString::fromUtf8("btnOk"));
    btnOk->setMinimumSize(QSize(80, 0));
    btnOk->setStyleSheet(QString::fromUtf8(""));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/image/btn_ok.png"), QSize(), QIcon::Normal, QIcon::Off);
    btnOk->setIcon(icon);

    lay->addWidget(btnOk);

    btnCancel = new QPushButton(frame);
    btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
    btnCancel->setMinimumSize(QSize(80, 0));
    btnCancel->setStyleSheet(QString::fromUtf8(""));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/image/btn_close.png"), QSize(), QIcon::Normal, QIcon::Off);
    btnCancel->setIcon(icon1);

    lay->addWidget(btnCancel);
    verticalLayout3->addLayout(lay);
    verticalLayout2->addWidget(frame);
    verticalLayout1->addWidget(widgetMain);

    setTabOrder(txtValue, btnOk);
    setTabOrder(btnOk, btnCancel);

    this->labTitle->setText("输入框");
    this->btnOk->setText("确定");
    this->btnCancel->setText("取消");

    connect(this->btnOk, SIGNAL(clicked()), this, SLOT(on_btnOk_clicked()));
    connect(this->btnMenu_Close, SIGNAL(clicked(bool)), this, SLOT(on_btnMenu_Close_clicked()));
    connect(this->btnCancel, SIGNAL(clicked()), this, SLOT(on_btnMenu_Close_clicked()));
}

void QUIInputBox::initForm()
{

    IconHelper::Instance()->setIcon(labIco, QUIConfig::IconMain, static_cast<uint>(QUIConfig::FontSize + 2));
    IconHelper::Instance()->setIcon(btnMenu_Close, QUIConfig::IconClose, static_cast<uint>(QUIConfig::FontSize));

    this->setProperty("form", true);
    this->widgetTitle->setProperty("form", "title");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint);
    this->setFixedSize(280, 150);
    this->setWindowTitle(this->labTitle->text());

    int width = 80;
    int iconWidth = 18;
    int iconHeight = 18;

    QList<QPushButton *> btns = this->frame->findChildren<QPushButton *>();

    foreach (QPushButton *btn, btns)
    {
        btn->setMinimumWidth(width);
        btn->setIconSize(QSize(iconWidth, iconHeight));
    }

    closeSec = 0;
    currentSec = 0;

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkSec()));
    timer->start();

    this->installEventFilter(this);
}

void QUIInputBox::checkSec()
{
    if (closeSec == 0)
    {
        return;
    }

    if (currentSec < closeSec)
    {
        currentSec++;
    }
    else
    {
        this->close();
    }

    QString str = QString("关闭倒计时 %1 s").arg(closeSec - currentSec + 1);
    this->labTime->setText(str);
}

void QUIInputBox::setParameter(const QString &title, int type, int closeSec,
                               QString defaultValue, bool pwd)
{
    this->closeSec = closeSec;
    this->currentSec = 0;
    this->labTime->clear();
    this->labTitle->setText(title);
    this->labInfo->setText(title);

    checkSec();

    if (type == 0)
    {
        this->cboxValue->setVisible(false);
        this->txtValue->setPlaceholderText(defaultValue);

        if (pwd)
        {
            this->txtValue->setEchoMode(QLineEdit::Password);
        }
    }
    else if (type == 1)
    {
        this->txtValue->setVisible(false);
        this->cboxValue->addItems(defaultValue.split("|"));
    }
}

QString QUIInputBox::getValue() const
{
    return this->value;
}

void QUIInputBox::closeEvent(QCloseEvent *)
{
    closeSec = 0;
    currentSec = 0;
}

bool QUIInputBox::eventFilter(QObject *obj, QEvent *evt)
{
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (event->button() == Qt::LeftButton)
        {
            mousePressed = true;
            mousePoint = event->globalPos() - this->pos();
            return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        mousePressed = false;
        return true;
    }
    else if (event->type() == QEvent::MouseMove)
    {
        if (mousePressed && (event->buttons() == Qt::LeftButton))
        {
            this->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(obj, evt);
}

void QUIInputBox::on_btnOk_clicked()
{
    if (this->txtValue->isVisible())
    {
        value = this->txtValue->text();
    }
    else if (this->cboxValue->isVisible())
    {
        value = this->cboxValue->currentText();
    }

    done(QMessageBox::Ok);
    close();
}

void QUIInputBox::on_btnMenu_Close_clicked()
{
    done(QMessageBox::Cancel);
    close();
}

void QUIInputBox::setIconMain(QChar str, quint32 size)
{
    IconHelper::Instance()->setIcon(this->labIco, str, size);
}

IconHelper *IconHelper::self = nullptr;
IconHelper::IconHelper(QObject *)
    : QObject(qApp)
{
    int fontId = QFontDatabase::addApplicationFont(":/image/fontawesome-webfont.ttf");
    QStringList fontName = QFontDatabase::applicationFontFamilies(fontId);

    if (fontName.count() > 0)
    {
        iconFont = QFont(fontName.at(0));
    }
    else
    {
        qDebug() << "load fontawesome-webfont.ttf error";
    }
}

void IconHelper::setIcon(QLabel *lab, QChar c, quint32 size)
{
    iconFont.setPointSize(static_cast<int>(size));
    lab->setFont(iconFont);
    lab->setText(c);
}

void IconHelper::setIcon(QAbstractButton *btn, QChar c, quint32 size)
{
    iconFont.setPointSize(static_cast<int>(size));
    btn->setFont(iconFont);
    btn->setText(c);
}

void IconHelper::setIcon(QTableWidgetItem *item, QChar c, quint32 size)
{
    iconFont.setPointSize(static_cast<int>(size));
    item->setFont(iconFont);
    item->setText(c);
}

QPixmap IconHelper::getPixmap(const QString &color, QChar c, quint32 size,
                              quint32 pixWidth, quint32 pixHeight)
{

    QPixmap pix(static_cast<int>(pixWidth), static_cast<int>(pixHeight));
    pix.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(QColor(color));
    painter.setBrush(QColor(color));
    iconFont.setPointSize(static_cast<int>(size));
    painter.setFont(iconFont);
    painter.drawText(pix.rect(), Qt::AlignCenter, c);
    painter.end();

    return pix;
}

QPixmap IconHelper::getPixmap(QToolButton *btn, bool normal)
{
    QPixmap pix;
    int index = btns.indexOf(btn);

    if (index >= 0)
    {
        if (normal)
        {
            pix = pixNormal.at(index);
        }
        else
        {
            pix = pixDark.at(index);
        }
    }

    return pix;
}

void IconHelper::setStyle(QWidget *widget, const QString &type, int borderWidth, const QString &borderColor,
                          const QString &normalBgColor, const QString &darkBgColor,
                          const QString &normalTextColor, const QString &darkTextColor)
{
    QString strBorder;
    if (type == "top")
    {
        strBorder = QString("border-width:%1px 0px 0px 0px;padding:%1px %2px %2px %2px;")
                        .arg(borderWidth)
                        .arg(borderWidth * 2);
    }
    else if (type == "right")
    {
        strBorder = QString("border-width:0px %1px 0px 0px;padding:%2px %1px %2px %2px;")
                        .arg(borderWidth)
                        .arg(borderWidth * 2);
    }
    else if (type == "bottom")
    {
        strBorder = QString("border-width:0px 0px %1px 0px;padding:%2px %2px %1px %2px;")
                        .arg(borderWidth)
                        .arg(borderWidth * 2);
    }
    else if (type == "left")
    {
        strBorder = QString("border-width:0px 0px 0px %1px;padding:%2px %2px %2px %1px;")
                        .arg(borderWidth)
                        .arg(borderWidth * 2);
    }

    QStringList qss;
    qss.append(QString("QWidget[flag=\"%1\"] QAbstractButton{border-style:none;border-radius:0px;padding:5px;color:%2;background:%3;}")
                   .arg(type)
                   .arg(normalTextColor)
                   .arg(normalBgColor));

    qss.append(QString("QWidget[flag=\"%1\"] QAbstractButton:hover,"
                       "QWidget[flag=\"%1\"] QAbstractButton:pressed,"
                       "QWidget[flag=\"%1\"] QAbstractButton:checked{"
                       "border-style:solid;%2border-color:%3;color:%4;background:%5;}")
                   .arg(type)
                   .arg(strBorder)
                   .arg(borderColor)
                   .arg(darkTextColor)
                   .arg(darkBgColor));

    widget->setStyleSheet(qss.join(""));
}

void IconHelper::setStyle(QWidget *widget, QList<QToolButton *> btns, QList<int> pixChar,
                          quint32 iconSize, quint32 iconWidth, quint32 iconHeight,
                          const QString &type, int borderWidth, const QString &borderColor,
                          const QString &normalBgColor, const QString &darkBgColor,
                          const QString &normalTextColor, const QString &darkTextColor)
{
    int btnCount = btns.count();
    int charCount = pixChar.count();
    if (btnCount <= 0 || charCount <= 0 || btnCount != charCount)
    {
        return;
    }

    QString strBorder;
    if (type == "top")
    {
        strBorder = QString("border-width:%1px 0px 0px 0px;padding:%1px %2px %2px %2px;")
                        .arg(borderWidth)
                        .arg(borderWidth * 2);
    }
    else if (type == "right")
    {
        strBorder = QString("border-width:0px %1px 0px 0px;padding:%2px %1px %2px %2px;")
                        .arg(borderWidth)
                        .arg(borderWidth * 2);
    }
    else if (type == "bottom")
    {
        strBorder = QString("border-width:0px 0px %1px 0px;padding:%2px %2px %1px %2px;")
                        .arg(borderWidth)
                        .arg(borderWidth * 2);
    }
    else if (type == "left")
    {
        strBorder = QString("border-width:0px 0px 0px %1px;padding:%2px %2px %2px %1px;")
                        .arg(borderWidth)
                        .arg(borderWidth * 2);
    }

    QStringList qss;
    qss.append(QString("QWidget[flag=\"%1\"] QAbstractButton{border-style:none;border-radius:0px;padding:5px;color:%2;background:%3;}")
                   .arg(type)
                   .arg(normalTextColor)
                   .arg(normalBgColor));

    qss.append(QString("QWidget[flag=\"%1\"] QAbstractButton:hover,"
                       "QWidget[flag=\"%1\"] QAbstractButton:pressed,"
                       "QWidget[flag=\"%1\"] QAbstractButton:checked{"
                       "border-style:solid;%2border-color:%3;color:%4;background:%5;}")
                   .arg(type)
                   .arg(strBorder)
                   .arg(borderColor)
                   .arg(darkTextColor)
                   .arg(darkBgColor));

    qss.append(QString("QWidget#%1{background:%2;}").arg(widget->objectName()).arg(normalBgColor));

    qss.append(QString("QToolButton{border-width:0px;}"));
    qss.append(QString("QToolButton{background-color:%1;color:%2;}")
                   .arg(normalBgColor)
                   .arg(normalTextColor));
    qss.append(QString("QToolButton:hover,QToolButton:pressed,QToolButton:checked{background-color:%1;color:%2;}")
                   .arg(darkBgColor)
                   .arg(darkTextColor));

    widget->setStyleSheet(qss.join(""));

    for (int i = 0; i < btnCount; i++)
    {
        //存储对应按钮对象,方便鼠标移上去的时候切换图片
        QPixmap pixNormal = getPixmap(normalTextColor, QChar(pixChar.at(i)), iconSize, iconWidth, iconHeight);
        QPixmap pixDark = getPixmap(darkTextColor, QChar(pixChar.at(i)), iconSize, iconWidth, iconHeight);

        btns.at(i)->setIcon(QIcon(pixNormal));
        btns.at(i)->setIconSize(QSize(static_cast<int>(iconWidth), static_cast<int>(iconHeight)));
        btns.at(i)->installEventFilter(this);

        this->btns.append(btns.at(i));
        this->pixNormal.append(pixNormal);
        this->pixDark.append(pixDark);
    }
}

void IconHelper::setStyle(QFrame *frame, QList<QToolButton *> btns, QList<int> pixChar,
                          quint32 iconSize, quint32 iconWidth, quint32 iconHeight,
                          const QString &normalBgColor, const QString &darkBgColor,
                          const QString &normalTextColor, const QString &darkTextColor)
{
    int btnCount = btns.count();
    int charCount = pixChar.count();
    if (btnCount <= 0 || charCount <= 0 || btnCount != charCount)
    {
        return;
    }

    QStringList qss;
    qss.append(QString("QToolButton{border-width:0px;}"));
    qss.append(QString("QToolButton{background-color:%1;color:%2;}")
                   .arg(normalBgColor)
                   .arg(normalTextColor));
    qss.append(QString("QToolButton:hover,QToolButton:pressed,QToolButton:checked{background-color:%1;color:%2;}")
                   .arg(darkBgColor)
                   .arg(darkTextColor));

    frame->setStyleSheet(qss.join(""));

    for (int i = 0; i < btnCount; i++)
    {
        //存储对应按钮对象,方便鼠标移上去的时候切换图片
        QPixmap pixNormal = getPixmap(normalTextColor, QChar(pixChar.at(i)), iconSize, iconWidth, iconHeight);
        QPixmap pixDark = getPixmap(darkTextColor, QChar(pixChar.at(i)), iconSize, iconWidth, iconHeight);

        btns.at(i)->setIcon(QIcon(pixNormal));
        btns.at(i)->setIconSize(QSize(static_cast<int>(iconWidth), static_cast<int>(iconHeight)));
        btns.at(i)->installEventFilter(this);

        this->btns.append(btns.at(i));
        this->pixNormal.append(pixNormal);
        this->pixDark.append(pixDark);
    }
}

bool IconHelper::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->inherits("QToolButton"))
    {
        QToolButton *btn = static_cast<QToolButton *>(watched);
        int index = btns.indexOf(btn);
        if (index >= 0)
        {
            if (event->type() == QEvent::Enter)
            {
                btn->setIcon(QIcon(pixDark.at(index)));
            }
            else if (event->type() == QEvent::Leave)
            {
                if (btn->isChecked())
                {
                    btn->setIcon(QIcon(pixDark.at(index)));
                }
                else
                {
                    btn->setIcon(QIcon(pixNormal.at(index)));
                }
            }
        }
    }

    return QObject::eventFilter(watched, event);
}

QChar QUIConfig::IconMain = QChar(0xf2db);
QChar QUIConfig::IconMenu = QChar(0xf0d7);
QChar QUIConfig::IconMin = QChar(0xf068);
QChar QUIConfig::IconMax = QChar(0xf2d2);
QChar QUIConfig::IconNormal = QChar(0xf2d0);
QChar QUIConfig::IconClose = QChar(0xf00d);

#ifdef __arm__
QString QUIConfig::FontName = "Droid Sans Fallback";
int QUIConfig::FontSize = 13;
#else
QString QUIConfig::FontName = "Microsoft Yahei";
int QUIConfig::FontSize = 9;
#endif

QString QUIConfig::ConfigFile = "config.ini";
QString QUIConfig::TextColor = "#F0F0F0";
QString QUIConfig::PanelColor = "#F0F0F0";
QString QUIConfig::BorderColor = "#F0F0F0";
QString QUIConfig::NormalColorStart = "#F0F0F0";
QString QUIConfig::NormalColorEnd = "#F0F0F0";
QString QUIConfig::DarkColorStart = "#F0F0F0";
QString QUIConfig::DarkColorEnd = "#F0F0F0";
QString QUIConfig::HighColor = "#F0F0F0";

QSettings *QUIConfig::GetCofnig()
{
    return new QSettings(QUIConfig::ConfigFile, QSettings::IniFormat);
}

void QUIConfig::ReadConfig()
{
    if (!CheckConfig())
    {
        return;
    }

    QSettings set(QUIConfig::ConfigFile, QSettings::IniFormat);
    set.beginGroup("BaseConfig");
    set.endGroup();
}

void QUIConfig::WriteConfig()
{
    QSettings set(QUIConfig::ConfigFile, QSettings::IniFormat);

    set.beginGroup("BaseConfig");
    set.endGroup();
}

void QUIConfig::NewConfig()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
#endif
    WriteConfig();
}

bool QUIConfig::CheckConfig()
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(QUIConfig::ConfigFile);

    if (file.size() == 0)
    {
        NewConfig();
        return false;
    }

    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly))
    {
        bool ok = true;

        while (!file.atEnd())
        {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2)
            {
                if (list.at(1) == "")
                {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok)
        {
            NewConfig();
            return false;
        }
    }
    else
    {
        NewConfig();
        return false;
    }

    return true;
}
