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
