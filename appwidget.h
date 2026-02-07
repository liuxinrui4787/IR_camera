<<<<<<< HEAD
﻿#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QApplication>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMutex>
#include <QPushButton>
#include <QSettings>
#include <QTableWidgetItem>
#include <QToolButton>
#include <QVBoxLayout>

#include "components/qtmaterialdialog.h"
#include "components/qtmaterialflatbutton.h"
#include "components/qtmaterialsnackbar.h"

#define AppPath qApp->applicationDirPath()
#define AppDeskWidth qApp->desktop()->availableGeometry().width()
#define AppDeskHeight qApp->desktop()->availableGeometry().height()
#define DATETIME qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))

#ifdef Q_OS_WIN
#define NEWLINE "\r\n"
#else
#define NEWLINE "\n"
#endif
#if defined(_MSC_VER)
    #define NORETURN __declspec(noreturn)  // MSVC
#elif defined(__GNUC__) || defined(__clang__)
    #define NORETURN __attribute__((noreturn))  // GCC/Clang
#else
    #define NORETURN  // 其他编译器留空
#endif

class QUIMessageBox;
class AppWidget : public QDialog
{
    Q_OBJECT
  public:
    //将部分对象作为枚举值暴露给外部
    enum Widget
    {
        Lab_Ico = 0, //左上角图标
        BtnMenu = 1, //下拉菜单按钮
        BtnMenu_Min = 2, //最小化按钮
        BtnMenu_Max = 3, //最大化按钮
        BtnMenu_Normal = 4, //还原按钮
        BtnMenu_Close = 5 //关闭按钮
    };
    //全局静态方法集合
  public:
    //桌面宽度高度
    static int deskWidth();
    static int deskHeight();
    //程序本身文件名称
    static QString appName();
    //程序当前所在路径
    static QString appPath();

    //写入消息到额外的的消息日志文件
    static void writeInfo(const QString &info, const QString &filePath = "log");
    //设置窗体居中显示
    static void setFormInCenter(QWidget *frm);
    //设置翻译文件
    static void setTranslator(const QString &qmFile = ":/image/qt_zh_CN.qm");
    //弹出消息框
    static void showMessageBoxInfo(const QString &info, int closeSec = 0);
    //弹出错误框
    static void showMessageBoxError(const QString &info, int closeSec = 0);
    //弹出询问框
    static QUIMessageBox *showMessageBoxQuestion(const QString &info);
    //弹出输入框
    static QString showInputBox(bool &ok, const QString &title, int type = 0, int closeSec = 0,
                                QString defaultValue = QString(), bool pwd = false);

    //    static void ShowToast(QString);

    static QString HexColor(QColor);

    static void ShowToast(QString msg)
    {
        if (mToast != nullptr)
        {
            mToast->addMessage(msg);
        }
    }
    //全局静态方法集合结束
  public:
    explicit AppWidget(QWidget *parent = nullptr);
    ~AppWidget();

  protected:
    bool eventFilter(QObject *obj, QEvent *evt);

  private:
    QVBoxLayout *verticalLayout1;
    QVBoxLayout *verticalLayout2;
    //    QFrame *widgetMain;
    QToolButton *btnMenu;
    QtMaterialFlatButton *btnMenu_Min;
    QtMaterialFlatButton *btnMenu_Close;
    //    QtMaterialFlatButton *btnMenu_Min;
    //    QtMaterialFlatButton *btnMenu_Close;

    QWidget *widgetMain;
    QWidget *widget;
    QWidget *widgetTitle; //标题栏

    QLabel *labIco; //对应的小图标
    QLabel *labTitle; //标题栏的标题
  private:
    QRect location; //鼠标移动窗体后的坐标位置
    bool minHide; //最小化隐藏
    QWidget *mainWidget; //主窗体对象
    QString cssDark; //黑暗模式的CSS
    QString cssLight; //白色模式的CSS
    bool isDarkMode = false; //是否是黑夜模式
  public:
    static QtMaterialSnackbar *mToast;
    QPushButton *getBtnMenuMin() const;
    QPushButton *getBtnMenuMClose() const;
    QLabel *getWinTitle() const;
    QLabel *getLabIco() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setWidgetVisible(AppWidget::Widget widget, bool visible);
  private slots:
    void initControl(); //初始化控件
    void initForm(); //初始化窗体

  private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    NORETURN void on_btnMenu_Close_clicked();
  public Q_SLOTS:
    //设置部件图标
    void setIcon(AppWidget::Widget widget, QChar str, quint32 size = 9);
    //设置部件图片
    void setPixmap(AppWidget::Widget widget, const QString &file, const QSize &size = QSize(16, 16));
    //设置只有关闭按钮
    void setOnlyCloseBtn();

    //设置按钮统一宽度
    void setBtnWidth(int width);

    //设置最小化隐藏
    void setMinHide(bool minHide);

    //设置主窗体
    void setMainWidget(QWidget *mainWidget);
    void setStyleWithFile(const char *file);

    void setTitle(QString);

    /**
     * 设置两种模式下的文件
     */
    void setStyleFiles(QString light, QString dark);
    //切换黑夜/白夜
    void changeDarkMode();

  Q_SIGNALS:
    void changeStyle(const QString &qssFile);
    void closing();
};

//弹出信息框类
class QUIMessageBox : public QtMaterialDialog
{
    Q_OBJECT

  public:
    explicit QUIMessageBox(QWidget *parent = nullptr);
    ~QUIMessageBox();

    static QUIMessageBox *Instance()
    {
        static QMutex mutex;

        if (!self)
        {
            QMutexLocker locker(&mutex);

            if (!self)
            {
                self = new QUIMessageBox();
            }
        }

        return self;
    }

  private:
    static QUIMessageBox *self;

    QtMaterialFlatButton *btnOk; //确定
    QtMaterialFlatButton *btnCancel; //取消
    QLabel *labTitle; //标题
    QLabel *labInfo; //具体消息
    QVBoxLayout *verticalLayout1;
    QHBoxLayout *horizontalLayout1; /*


    QVBoxLayout *verticalLayout1;
    QWidget *widgetTitle;
    QHBoxLayout *horizontalLayout3;
    QLabel *labIco;
    QLabel *labTitle;
    QLabel *labTime;
    QWidget *widgetMenu;
    QHBoxLayout *horizontalLayout4;
    QPushButton *btnMenu_Close;
    QWidget *widgetMain;
    QVBoxLayout *verticalLayout2;
    QFrame *frame;
    QVBoxLayout *verticalLayout4;
    QHBoxLayout *horizontalLayout1;
    QLabel *labIcoMain;
    QSpacerItem *horizontalSpacer1;
    QLabel *labInfo;
    QHBoxLayout *horizontalLayout2;
    QSpacerItem *horizontalSpacer2;
    QPushButton *btnOk;
    QPushButton *btnCancel;*/
  Q_SIGNALS:
    void result(int); //点击返回或者成功的回调;

  private:
    int closeSec; //总显示时间
    int currentSec; //当前已显示时间

  private slots:
    void initControl(); //初始化控件
    void checkSec(); //校验倒计时

  private slots:
    void on_btnOk_clicked();
    void on_btnMenu_Close_clicked();

  public Q_SLOTS:
    //    void setIconMain(QChar str, quint32 size = 9);
    void setMessage(const QString &msg, int type, int closeSec = 0);
};

//弹出输入框类
class QUIInputBox : public QDialog
{
    Q_OBJECT

  public:
    explicit QUIInputBox(QWidget *parent = nullptr);
    ~QUIInputBox();

    static QUIInputBox *Instance()
    {
        static QMutex mutex;

        if (!self)
        {
            QMutexLocker locker(&mutex);

            if (!self)
            {
                self = new QUIInputBox;
            }
        }

        return self;
    }

  protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *obj, QEvent *evt);

  private:
    static QUIInputBox *self;

    QVBoxLayout *verticalLayout1;
    QWidget *widgetTitle;
    QHBoxLayout *horizontalLayout1;
    QLabel *labIco;
    QLabel *labTitle;
    QLabel *labTime;
    QWidget *widgetMenu;
    QHBoxLayout *horizontalLayout2;
    QPushButton *btnMenu_Close;
    QWidget *widgetMain;
    QVBoxLayout *verticalLayout2;
    QFrame *frame;
    QVBoxLayout *verticalLayout3;
    QLabel *labInfo;
    QLineEdit *txtValue;
    QComboBox *cboxValue;
    QHBoxLayout *lay;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;

  private:
    int closeSec; //总显示时间
    int currentSec; //当前已显示时间
    QString value; //当前值

  private slots:
    void initControl(); //初始化控件
    void initForm(); //初始化窗体
    void checkSec(); //校验倒计时

  private slots:
    void on_btnOk_clicked();
    void on_btnMenu_Close_clicked();

  public:
    QString getValue() const;

  public Q_SLOTS:
    void setIconMain(QChar str, quint32 size = 9);
    void setParameter(const QString &title, int type = 0, int closeSec = 0,
                      QString defaultValue = QString(), bool pwd = false);
};

//图形字体处理类
class IconHelper : public QObject
{
    Q_OBJECT

  public:
    explicit IconHelper(QObject *parent = nullptr);
    static IconHelper *Instance()
    {
        static QMutex mutex;

        if (!self)
        {
            QMutexLocker locker(&mutex);

            if (!self)
            {
                self = new IconHelper;
            }
        }

        return self;
    }

    void setIcon(QLabel *lab, QChar c, quint32 size = 9);
    void setIcon(QAbstractButton *btn, QChar c, quint32 size = 9);
    void setIcon(QTableWidgetItem *btn, QChar c, quint32 size = 9);

    QPixmap getPixmap(const QString &color, QChar c, quint32 size = 9,
                      quint32 pixWidth = 10, quint32 pixHeight = 10);

    //根据按钮获取该按钮对应的图标
    QPixmap getPixmap(QToolButton *btn, bool normal);

    //指定导航面板样式,不带图标
    static void setStyle(QWidget *widget, const QString &type = "left", int borderWidth = 3,
                         const QString &borderColor = "#029FEA",
                         const QString &normalBgColor = "#292F38",
                         const QString &darkBgColor = "#1D2025",
                         const QString &normalTextColor = "#54626F",
                         const QString &darkTextColor = "#FDFDFD");

    //指定导航面板样式,带图标和效果切换
    void setStyle(QWidget *widget, QList<QToolButton *> btns, QList<int> pixChar,
                  quint32 iconSize = 9, quint32 iconWidth = 10, quint32 iconHeight = 10,
                  const QString &type = "left", int borderWidth = 3,
                  const QString &borderColor = "#029FEA",
                  const QString &normalBgColor = "#292F38",
                  const QString &darkBgColor = "#1D2025",
                  const QString &normalTextColor = "#54626F",
                  const QString &darkTextColor = "#FDFDFD");

    //指定导航按钮样式,带图标和效果切换
    void setStyle(QFrame *frame, QList<QToolButton *> btns, QList<int> pixChar,
                  quint32 iconSize = 9, quint32 iconWidth = 10, quint32 iconHeight = 10,
                  const QString &normalBgColor = "#2FC5A2",
                  const QString &darkBgColor = "#3EA7E9",
                  const QString &normalTextColor = "#EEEEEE",
                  const QString &darkTextColor = "#FFFFFF");

  protected:
    bool eventFilter(QObject *watched, QEvent *event);

  private:
    static IconHelper *self; //对象自身
    QFont iconFont; //图形字体
    QList<QToolButton *> btns; //按钮队列
    QList<QPixmap> pixNormal; //正常图片队列
    QList<QPixmap> pixDark; //加深图片队列
};

//全局变量控制
class QUIConfig
{
  public:
    //全局图标
    static QChar IconMain; //标题栏左上角图标
    static QChar IconMenu; //下拉菜单图标
    static QChar IconMin; //最小化图标
    static QChar IconMax; //最大化图标
    static QChar IconNormal; //还原图标
    static QChar IconClose; //关闭图标

    static QString FontName; //全局字体名称
    static int FontSize; //全局字体大小
    static QString ConfigFile; //配置文件文件路径及名称

    //样式表颜色值
    static QString TextColor; //文字颜色
    static QString PanelColor; //面板颜色
    static QString BorderColor; //边框颜色
    static QString NormalColorStart; //正常状态开始颜色
    static QString NormalColorEnd; //正常状态结束颜色
    static QString DarkColorStart; //加深状态开始颜色
    static QString DarkColorEnd; //加深状态结束颜色
    static QString HighColor; //高亮颜色

    static void ReadConfig(); //读取配置文件,在main函数最开始加载程序载入
    static void WriteConfig(); //写入配置文件,在更改配置文件程序关闭时调用
    static void NewConfig(); //以初始值新建配置文件
    static bool CheckConfig(); //校验配置文件
    static QSettings *GetCofnig();
};

#endif // APPWIDGET_H
=======
﻿#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QApplication>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMutex>
#include <QPushButton>
#include <QSettings>
#include <QTableWidgetItem>
#include <QToolButton>
#include <QVBoxLayout>

#include "components/qtmaterialdialog.h"
#include "components/qtmaterialflatbutton.h"
#include "components/qtmaterialsnackbar.h"

#define AppPath qApp->applicationDirPath()
#define AppDeskWidth qApp->desktop()->availableGeometry().width()
#define AppDeskHeight qApp->desktop()->availableGeometry().height()
#define DATETIME qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))

#ifdef Q_OS_WIN
#define NEWLINE "\r\n"
#else
#define NEWLINE "\n"
#endif
#if defined(_MSC_VER)
    #define NORETURN __declspec(noreturn)  // MSVC
#elif defined(__GNUC__) || defined(__clang__)
    #define NORETURN __attribute__((noreturn))  // GCC/Clang
#else
    #define NORETURN  // 其他编译器留空
#endif

class QUIMessageBox;
class AppWidget : public QDialog
{
    Q_OBJECT
  public:
    //将部分对象作为枚举值暴露给外部
    enum Widget
    {
        Lab_Ico = 0, //左上角图标
        BtnMenu = 1, //下拉菜单按钮
        BtnMenu_Min = 2, //最小化按钮
        BtnMenu_Max = 3, //最大化按钮
        BtnMenu_Normal = 4, //还原按钮
        BtnMenu_Close = 5 //关闭按钮
    };
    //全局静态方法集合
  public:
    //桌面宽度高度
    static int deskWidth();
    static int deskHeight();
    //程序本身文件名称
    static QString appName();
    //程序当前所在路径
    static QString appPath();

    //写入消息到额外的的消息日志文件
    static void writeInfo(const QString &info, const QString &filePath = "log");
    //设置窗体居中显示
    static void setFormInCenter(QWidget *frm);
    //设置翻译文件
    static void setTranslator(const QString &qmFile = ":/image/qt_zh_CN.qm");
    //弹出消息框
    static void showMessageBoxInfo(const QString &info, int closeSec = 0);
    //弹出错误框
    static void showMessageBoxError(const QString &info, int closeSec = 0);
    //弹出询问框
    static QUIMessageBox *showMessageBoxQuestion(const QString &info);
    //弹出输入框
    static QString showInputBox(bool &ok, const QString &title, int type = 0, int closeSec = 0,
                                QString defaultValue = QString(), bool pwd = false);

    //    static void ShowToast(QString);

    static QString HexColor(QColor);

    static void ShowToast(QString msg)
    {
        if (mToast != nullptr)
        {
            mToast->addMessage(msg);
        }
    }
    //全局静态方法集合结束
  public:
    explicit AppWidget(QWidget *parent = nullptr);
    ~AppWidget();

  protected:
    bool eventFilter(QObject *obj, QEvent *evt);

  private:
    QVBoxLayout *verticalLayout1;
    QVBoxLayout *verticalLayout2;
    //    QFrame *widgetMain;
    QToolButton *btnMenu;
    QtMaterialFlatButton *btnMenu_Min;
    QtMaterialFlatButton *btnMenu_Close;
    //    QtMaterialFlatButton *btnMenu_Min;
    //    QtMaterialFlatButton *btnMenu_Close;

    QWidget *widgetMain;
    QWidget *widget;
    QWidget *widgetTitle; //标题栏

    QLabel *labIco; //对应的小图标
    QLabel *labTitle; //标题栏的标题
  private:
    QRect location; //鼠标移动窗体后的坐标位置
    bool minHide; //最小化隐藏
    QWidget *mainWidget; //主窗体对象
    QString cssDark; //黑暗模式的CSS
    QString cssLight; //白色模式的CSS
    bool isDarkMode = false; //是否是黑夜模式
  public:
    static QtMaterialSnackbar *mToast;
    QPushButton *getBtnMenuMin() const;
    QPushButton *getBtnMenuMClose() const;
    QLabel *getWinTitle() const;
    QLabel *getLabIco() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setWidgetVisible(AppWidget::Widget widget, bool visible);
  private slots:
    void initControl(); //初始化控件
    void initForm(); //初始化窗体

  private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    NORETURN void on_btnMenu_Close_clicked();
  public Q_SLOTS:
    //设置部件图标
    void setIcon(AppWidget::Widget widget, QChar str, quint32 size = 9);
    //设置部件图片
    void setPixmap(AppWidget::Widget widget, const QString &file, const QSize &size = QSize(16, 16));
    //设置只有关闭按钮
    void setOnlyCloseBtn();

    //设置按钮统一宽度
    void setBtnWidth(int width);

    //设置最小化隐藏
    void setMinHide(bool minHide);

    //设置主窗体
    void setMainWidget(QWidget *mainWidget);
    void setStyleWithFile(const char *file);

    void setTitle(QString);

    /**
     * 设置两种模式下的文件
     */
    void setStyleFiles(QString light, QString dark);
    //切换黑夜/白夜
    void changeDarkMode();

  Q_SIGNALS:
    void changeStyle(const QString &qssFile);
    void closing();
};

//弹出信息框类
class QUIMessageBox : public QtMaterialDialog
{
    Q_OBJECT

  public:
    explicit QUIMessageBox(QWidget *parent = nullptr);
    ~QUIMessageBox();

    static QUIMessageBox *Instance()
    {
        static QMutex mutex;

        if (!self)
        {
            QMutexLocker locker(&mutex);

            if (!self)
            {
                self = new QUIMessageBox();
            }
        }

        return self;
    }

  private:
    static QUIMessageBox *self;

    QtMaterialFlatButton *btnOk; //确定
    QtMaterialFlatButton *btnCancel; //取消
    QLabel *labTitle; //标题
    QLabel *labInfo; //具体消息
    QVBoxLayout *verticalLayout1;
    QHBoxLayout *horizontalLayout1; /*


    QVBoxLayout *verticalLayout1;
    QWidget *widgetTitle;
    QHBoxLayout *horizontalLayout3;
    QLabel *labIco;
    QLabel *labTitle;
    QLabel *labTime;
    QWidget *widgetMenu;
    QHBoxLayout *horizontalLayout4;
    QPushButton *btnMenu_Close;
    QWidget *widgetMain;
    QVBoxLayout *verticalLayout2;
    QFrame *frame;
    QVBoxLayout *verticalLayout4;
    QHBoxLayout *horizontalLayout1;
    QLabel *labIcoMain;
    QSpacerItem *horizontalSpacer1;
    QLabel *labInfo;
    QHBoxLayout *horizontalLayout2;
    QSpacerItem *horizontalSpacer2;
    QPushButton *btnOk;
    QPushButton *btnCancel;*/
  Q_SIGNALS:
    void result(int); //点击返回或者成功的回调;

  private:
    int closeSec; //总显示时间
    int currentSec; //当前已显示时间

  private slots:
    void initControl(); //初始化控件
    void checkSec(); //校验倒计时

  private slots:
    void on_btnOk_clicked();
    void on_btnMenu_Close_clicked();

  public Q_SLOTS:
    //    void setIconMain(QChar str, quint32 size = 9);
    void setMessage(const QString &msg, int type, int closeSec = 0);
};

//弹出输入框类
class QUIInputBox : public QDialog
{
    Q_OBJECT

  public:
    explicit QUIInputBox(QWidget *parent = nullptr);
    ~QUIInputBox();

    static QUIInputBox *Instance()
    {
        static QMutex mutex;

        if (!self)
        {
            QMutexLocker locker(&mutex);

            if (!self)
            {
                self = new QUIInputBox;
            }
        }

        return self;
    }

  protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *obj, QEvent *evt);

  private:
    static QUIInputBox *self;

    QVBoxLayout *verticalLayout1;
    QWidget *widgetTitle;
    QHBoxLayout *horizontalLayout1;
    QLabel *labIco;
    QLabel *labTitle;
    QLabel *labTime;
    QWidget *widgetMenu;
    QHBoxLayout *horizontalLayout2;
    QPushButton *btnMenu_Close;
    QWidget *widgetMain;
    QVBoxLayout *verticalLayout2;
    QFrame *frame;
    QVBoxLayout *verticalLayout3;
    QLabel *labInfo;
    QLineEdit *txtValue;
    QComboBox *cboxValue;
    QHBoxLayout *lay;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;

  private:
    int closeSec; //总显示时间
    int currentSec; //当前已显示时间
    QString value; //当前值

  private slots:
    void initControl(); //初始化控件
    void initForm(); //初始化窗体
    void checkSec(); //校验倒计时

  private slots:
    void on_btnOk_clicked();
    void on_btnMenu_Close_clicked();

  public:
    QString getValue() const;

  public Q_SLOTS:
    void setIconMain(QChar str, quint32 size = 9);
    void setParameter(const QString &title, int type = 0, int closeSec = 0,
                      QString defaultValue = QString(), bool pwd = false);
};

//图形字体处理类
class IconHelper : public QObject
{
    Q_OBJECT

  public:
    explicit IconHelper(QObject *parent = nullptr);
    static IconHelper *Instance()
    {
        static QMutex mutex;

        if (!self)
        {
            QMutexLocker locker(&mutex);

            if (!self)
            {
                self = new IconHelper;
            }
        }

        return self;
    }

    void setIcon(QLabel *lab, QChar c, quint32 size = 9);
    void setIcon(QAbstractButton *btn, QChar c, quint32 size = 9);
    void setIcon(QTableWidgetItem *btn, QChar c, quint32 size = 9);

    QPixmap getPixmap(const QString &color, QChar c, quint32 size = 9,
                      quint32 pixWidth = 10, quint32 pixHeight = 10);

    //根据按钮获取该按钮对应的图标
    QPixmap getPixmap(QToolButton *btn, bool normal);

    //指定导航面板样式,不带图标
    static void setStyle(QWidget *widget, const QString &type = "left", int borderWidth = 3,
                         const QString &borderColor = "#029FEA",
                         const QString &normalBgColor = "#292F38",
                         const QString &darkBgColor = "#1D2025",
                         const QString &normalTextColor = "#54626F",
                         const QString &darkTextColor = "#FDFDFD");

    //指定导航面板样式,带图标和效果切换
    void setStyle(QWidget *widget, QList<QToolButton *> btns, QList<int> pixChar,
                  quint32 iconSize = 9, quint32 iconWidth = 10, quint32 iconHeight = 10,
                  const QString &type = "left", int borderWidth = 3,
                  const QString &borderColor = "#029FEA",
                  const QString &normalBgColor = "#292F38",
                  const QString &darkBgColor = "#1D2025",
                  const QString &normalTextColor = "#54626F",
                  const QString &darkTextColor = "#FDFDFD");

    //指定导航按钮样式,带图标和效果切换
    void setStyle(QFrame *frame, QList<QToolButton *> btns, QList<int> pixChar,
                  quint32 iconSize = 9, quint32 iconWidth = 10, quint32 iconHeight = 10,
                  const QString &normalBgColor = "#2FC5A2",
                  const QString &darkBgColor = "#3EA7E9",
                  const QString &normalTextColor = "#EEEEEE",
                  const QString &darkTextColor = "#FFFFFF");

  protected:
    bool eventFilter(QObject *watched, QEvent *event);

  private:
    static IconHelper *self; //对象自身
    QFont iconFont; //图形字体
    QList<QToolButton *> btns; //按钮队列
    QList<QPixmap> pixNormal; //正常图片队列
    QList<QPixmap> pixDark; //加深图片队列
};

//全局变量控制
class QUIConfig
{
  public:
    //全局图标
    static QChar IconMain; //标题栏左上角图标
    static QChar IconMenu; //下拉菜单图标
    static QChar IconMin; //最小化图标
    static QChar IconMax; //最大化图标
    static QChar IconNormal; //还原图标
    static QChar IconClose; //关闭图标

    static QString FontName; //全局字体名称
    static int FontSize; //全局字体大小
    static QString ConfigFile; //配置文件文件路径及名称

    //样式表颜色值
    static QString TextColor; //文字颜色
    static QString PanelColor; //面板颜色
    static QString BorderColor; //边框颜色
    static QString NormalColorStart; //正常状态开始颜色
    static QString NormalColorEnd; //正常状态结束颜色
    static QString DarkColorStart; //加深状态开始颜色
    static QString DarkColorEnd; //加深状态结束颜色
    static QString HighColor; //高亮颜色

    static void ReadConfig(); //读取配置文件,在main函数最开始加载程序载入
    static void WriteConfig(); //写入配置文件,在更改配置文件程序关闭时调用
    static void NewConfig(); //以初始值新建配置文件
    static bool CheckConfig(); //校验配置文件
    static QSettings *GetCofnig();
};

#endif // APPWIDGET_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
