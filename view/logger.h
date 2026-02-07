<<<<<<< HEAD
#ifndef LOGGER_H
#define LOGGER_H
#include "qdatetime.h"
#include <QDebug>
#include <QString>

class Logger;

#define LOGD Logger(__FILE__, __LINE__).write

class Logger
{
  public:
    inline explicit Logger(const char *file, int line)
        : m_file(file),
          m_line(line)
    {
    }
    ~Logger();
    QDebug write(int id = 0);

  private:
    int m_id = 0;
    const char *m_file;
    int m_line;
    QString m_message;
};

#endif // LOGGER_H
=======
#ifndef LOGGER_H
#define LOGGER_H
#include "qdatetime.h"
#include <QDebug>
#include <QString>

class Logger;

#define LOGD Logger(__FILE__, __LINE__).write

class Logger
{
  public:
    inline explicit Logger(const char *file, int line)
        : m_file(file),
          m_line(line)
    {
    }
    ~Logger();
    QDebug write(int id = 0);

  private:
    int m_id = 0;
    const char *m_file;
    int m_line;
    QString m_message;
};

#endif // LOGGER_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
