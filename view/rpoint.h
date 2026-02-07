<<<<<<< HEAD
#ifndef RPOINT_H
#define RPOINT_H
#include <QDebugStateSaver>
#include <QObject>
#include <QPoint>
class RPoint {
  public:
    RPoint();
    RPoint ( long x, long y );
    void setX ( long x ) {
        this->xp = x;
    }

    void setY ( long y ) {
        this->yp = y;
    }
    long x() const {
        return xp;
    }
    long y() const {
        return yp;
    }
    friend std::istream &operator>> ( std::istream &, RPoint & );
    friend std::ostream &operator<< ( std::ostream &, const RPoint & );
    friend QDebug operator<< ( QDebug debug, const RPoint &c );
    QPoint GetFixPoint ( int moveX, int moveY, int scale );
    static QPoint GetFixPoint ( const RPoint &p, int moveX, int moveY, int scale );
    static QPoint GetFixPoint ( const RPoint &p, int moveX, int moveY );
    static QPoint GetFixPoint ( const QPoint &p, int moveX, int moveY, int scale );
    static QPoint GetFixPoint ( const QPoint &p, int moveX, int moveY );

  private:
    long xp;
    long yp;
};

#endif // RPOINT_H
=======
#ifndef RPOINT_H
#define RPOINT_H
#include <QDebugStateSaver>
#include <QObject>
#include <QPoint>
class RPoint {
  public:
    RPoint();
    RPoint ( long x, long y );
    void setX ( long x ) {
        this->xp = x;
    }

    void setY ( long y ) {
        this->yp = y;
    }
    long x() const {
        return xp;
    }
    long y() const {
        return yp;
    }
    friend std::istream &operator>> ( std::istream &, RPoint & );
    friend std::ostream &operator<< ( std::ostream &, const RPoint & );
    friend QDebug operator<< ( QDebug debug, const RPoint &c );
    QPoint GetFixPoint ( int moveX, int moveY, int scale );
    static QPoint GetFixPoint ( const RPoint &p, int moveX, int moveY, int scale );
    static QPoint GetFixPoint ( const RPoint &p, int moveX, int moveY );
    static QPoint GetFixPoint ( const QPoint &p, int moveX, int moveY, int scale );
    static QPoint GetFixPoint ( const QPoint &p, int moveX, int moveY );

  private:
    long xp;
    long yp;
};

#endif // RPOINT_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
