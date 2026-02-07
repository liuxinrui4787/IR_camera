 #include "basewindow.h"
//#include "log4qt/logger.h"
#include "qdebug.h"
#include "qevent.h"
#include "qstring.h"
#include <QDateTime>
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>

//QTextBrowser *BaseWindow::m_logText = nullptr;
BaseWindow *BaseWindow::m_self = nullptr;
BaseWindow::BaseWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_winTitle = nullptr;
    m_winIcon = nullptr;
    m_self = this;
    connect(this, SIGNAL(showLogsMessage(QString)), this,
            SLOT(showMessage(QString)));

    //    QList<QScreen *> screens = QGuiApplication::screens();
    //    if (screens.size() > 0)
    //    {
    //        QScreen *screen = screens[0];
    //        connect(screen, &QScreen::logicalDotsPerInchChanged, this, &BaseWindow::onLogicalDotsPerInchChanged);
    //    }
}

void BaseWindow::read()
{
    QSettings iniFile("./config.ini", QSettings::IniFormat);
    this->m_saveWinSize = iniFile.value("winsize", QSize(0, 0)).toSize();
    if (this->m_saveWinSize.width() > 0 && this->m_saveWinSize.height() > 0)
    {
        this->resize(this->m_saveWinSize.width(), this->m_saveWinSize.height());
    }
    this->m_tagIdx = (iniFile.value("tag", 0)).toInt();
}

void BaseWindow::initLogsView(QTextBrowser *v)
{
    if (m_logText)
    {
        m_logText->clear();
    }
    m_logText = v;
}

void BaseWindow::clearLogsView()
{
    if (m_logText)
    {
        m_logText->clear();
    }
    m_logText = nullptr;
}

void BaseWindow::showLog(QString msg)
{
    emit showLogsMessage(msg);
}

void BaseWindow::winTitle(QLabel *label)
{
    this->m_winTitle = label;
}

void BaseWindow::winLabIcon(QLabel *label)
{
    this->m_winIcon = label;
}

void BaseWindow::showMessage(QString msg)
{
    if (this->m_logText)
    {
        this->m_logText->append(msg);
    }
}

void BaseWindow::setWinTitle(QString title, Qt::Alignment align)
{
    if (this->m_winTitle != nullptr)
    {
        this->m_winTitle->setText(title);
        this->m_winTitle->setAlignment(align | Qt::AlignVCenter);
        if (m_winIcon != nullptr)
        {
            if (align == Qt::AlignHCenter)
            {
                m_winIcon->setVisible(true);
            }
            else
            {
                m_winIcon->setVisible(false);
            }
        }
    }
}

void BaseWindow::save()
{
    QSettings iniFile("./config.ini", QSettings::IniFormat);
    iniFile.setValue("winsize", this->m_saveWinSize);
}

void BaseWindow::changeTag(int idx)
{
    this->m_tagIdx = idx;
    QSettings iniFile("./config.ini", QSettings::IniFormat);
    iniFile.setValue("tag", this->m_tagIdx);
}

//cpp文件
void BaseWindow::resizeEvent(QResizeEvent *event)
{
    if (event->size() == this->m_saveWinSize)
        return;
    if (qAbs(event->size().width() - this->m_saveWinSize.width()) > 10 || qAbs(event->size().height() - this->m_saveWinSize.height()) > 10)
    {
        this->m_saveWinSize.setWidth(event->size().width());
        this->m_saveWinSize.setHeight(event->size().height());
        this->save();
    }
}

BaseLogger::~BaseLogger()
{
    if (this->m_id == LOGID_FILE)
    {
        //        Log4Qt::Logger::rootLogger()->debug((this->m_message + "\n").toStdString().c_str());
        return;
    }

#ifdef QT_NO_DEBUG
    if (LOGID_RELEASE == 0 || LOGID_RELEASE == this->m_id)
    {
        if (!this->isWriteToFile)
        {
            qDebug() << QDateTime::currentDateTime().toString(QLatin1String("HH:mm:ss.zzz")).toUtf8().data() << m_file << ":" << m_line << this->m_message.toStdString().c_str();
            return;
        }
        //        Log4Qt::Logger::rootLogger()->debug((this->m_message + "\n").toStdString().c_str());
    }
#else
    if (LOGID_DEBUG == 0 || LOGID_DEBUG == this->m_id)
    {
        if (!this->isWriteToFile)
        {

            qDebug() << QString("%0 %1[%2]: %3").arg(QDateTime::currentDateTime().toString(QLatin1String("HH:mm:ss.zzz")).toUtf8().data()).arg(m_file).arg(m_line).arg(this->m_message).toStdString().c_str();
            //            qDebug() << QDateTime::currentDateTime().toString(QLatin1String("HH:mm:ss.zzz")).toUtf8().data() << m_file << "[" << m_line << "]:" << this->m_message.toStdString().c_str();
            m_file = nullptr;
            return;
        }
        //        Log4Qt::Logger::rootLogger()->debug((this->m_message + "\n").toStdString().c_str());
    }
#endif
}

QDebug BaseLogger::write(int id)
{

    this->m_id = id;
    this->isWriteToFile = false;
    QDebug d(&m_message);
    return d;
}

QDebug BaseLogger::writeFile(int id)
{
    this->m_id = id;
    this->isWriteToFile = true;
    QDebug d(&m_message);
    return d;
}
