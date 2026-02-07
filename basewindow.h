#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QTextBrowser>
#include <QTextCursor>
#include <QTime>

#define LOGID_RELEASE 200 //release模式 0全部显示 -1全部不显示 >0表示只显示对应ID的
#define LOGID_DEBUG 200 //debug模式  0全部显示 -1全部不显示 >0表示只显示对应ID的
#define LOGID_FILE 310
#define Contro_max 10000000000000000000

#define LOGD BaseLogger(__FILE__, __LINE__).write
#define LOGFILE BaseLogger(__FILE__, __LINE__).writeFile
#define LOGINFO BaseWindow::ShowLog
#define LOGERROR BaseWindow::ShowErrorLog
#define LOGWARRING BaseWindow::ShowWaringLog

class BaseWindow : public QMainWindow
{
    Q_OBJECT
  public:
    explicit BaseWindow(QWidget *parent = nullptr);
    static void ShowLog(QString msg)
    {
        if (m_self)
        {
            QString strTimeInfo = "<font color=\"black\">[" + QTime::currentTime().toString() + "]</font> " + msg;
            m_self->showLog(strTimeInfo);
        }
    }

    static void ShowErrorLog(QString msg)
    {
        if (m_self)
        {
            QString strTimeInfo = "[" + QTime::currentTime().toString() + "]<font color=\"red\"> " + msg + "</font>";
            m_self->showLog(strTimeInfo);
        }
    }

    static void ShowWaringLog(QString msg)
    {
        if (m_self)
        {

            QString strTimeInfo = "[" + QTime::currentTime().toString() + "]<font color=\"#fd7e00\"> " + msg + "</font>";
            m_self->showLog(strTimeInfo);
        }
    }

    static void SetLogsView(QTextBrowser *v)
    {

        m_self->initLogsView(v);
    }

    static void ClearLogsView()
    {
        m_self->clearLogsView();
    }

  signals:
    void showLogsMessage(QString);
  public slots:
    void showMessage(QString);
    void setWinTitle(QString, Qt::Alignment align = Qt::AlignHCenter);
    //    void onLogicalDotsPerInchChanged(qreal dpi);

  public:
    QSize m_saveWinSize;
    int m_tagIdx = 0; //默认的tab页

  public:
    static BaseWindow *m_self;
    void initLogsView(QTextBrowser *v);
    void showLog(QString);
    void clearLogsView();
    void winTitle(QLabel *);
    void winLabIcon(QLabel *);

  protected:
    QTextBrowser *m_logText = nullptr;
    //    void findChild(const QObject &o);
    //    int scaleUI(int spec);
    virtual void read(); //读取配置
    virtual void save(); //保存配置
    virtual void resizeEvent(QResizeEvent *event) override;
    void changeTag(int idx);
    QLabel *m_winTitle;
    QLabel *m_winIcon;
    //    double m_preRate;
    //    double m_scaleFromPreRate;
};

class BaseLogger
{
  public:
    inline explicit BaseLogger(const char *file, int line)
        : m_file(file),
          m_line(line)
    {
    }
    ~BaseLogger();
    QDebug write(int id = 0);
    QDebug writeFile(int id = 0);

  private:
    int m_id = 0;
    bool isWriteToFile = false;
    const char *m_file;
    int m_line;
    QString m_message;
};

#endif // BASEWINDOW_H
