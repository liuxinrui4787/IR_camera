#include "rpoint.h"
#include "math.h"
RPoint::RPoint()
{
}

RPoint::RPoint(long x, long y)
    : xp(x), yp(y)
{
}

QPoint RPoint::GetFixPoint(int moveX, int moveY, int scale)
{
    return QPoint(static_cast<int>(ceil((xp + moveX) / scale)), static_cast<int>(ceil((yp + moveY) / scale)));
}

QPoint RPoint::GetFixPoint(const RPoint &p, int moveX, int moveY, int scale)
{
    return QPoint(static_cast<int>(ceil((p.x() + moveX) / scale)), static_cast<int>(ceil((p.y() + moveY) / scale)));
    //    return p.GetFixPoint(moveX, moveY, scale);
}

QPoint RPoint::GetFixPoint(const RPoint &p, int moveX, int moveY)
{

    return QPoint(static_cast<int>(p.x()) + moveX, static_cast<int>(p.y()) + moveY);
}
QPoint RPoint::GetFixPoint(const QPoint &p, int moveX, int moveY, int scale)
{
    return QPoint(static_cast<int>(ceil((p.x() + moveX) / scale)), static_cast<int>(ceil((p.y() + moveY) / scale)));
    //    return p.GetFixPoint(moveX, moveY, scale);
}

QPoint RPoint::GetFixPoint(const QPoint &p, int moveX, int moveY)
{

    return QPoint(static_cast<int>(p.x()) + moveX, static_cast<int>(p.y()) + moveY);
}
std::istream &operator>>(std::istream &in, RPoint &m)
{
    in >> m.xp >> m.yp;
    return in;
}
std::ostream &operator<<(std::ostream &out, const RPoint &m)
{
    out << "{ " << m.xp << ", " << m.yp << "}";
    return out;
}
QDebug operator<<(QDebug debug, const RPoint &m)
{
    //  QDebugStateSaver saver(debug);
    debug.noquote() << "{" << m.xp << "," << m.yp << "}";
    return debug;
}
