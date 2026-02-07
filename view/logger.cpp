<<<<<<< HEAD
﻿#include "logger.h"
Logger::~Logger()
{
    //    if (!AppWidget::ShowLog(this->m_id))
    //        return;

    qDebug() << QDateTime::currentDateTime().toString(QLatin1String("HH:mm:ss.zzz")).toUtf8().data() << m_file << ":" << m_line << this->m_message.toStdString().c_str();
}

QDebug Logger::write(int id)
{
    this->m_id = id;
    QDebug d(&m_message);
    return d;
}
=======
﻿#include "logger.h"
Logger::~Logger()
{
    //    if (!AppWidget::ShowLog(this->m_id))
    //        return;

    qDebug() << QDateTime::currentDateTime().toString(QLatin1String("HH:mm:ss.zzz")).toUtf8().data() << m_file << ":" << m_line << this->m_message.toStdString().c_str();
}

QDebug Logger::write(int id)
{
    this->m_id = id;
    QDebug d(&m_message);
    return d;
}
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
