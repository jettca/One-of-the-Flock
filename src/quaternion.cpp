#include "quaternion.h"

#include <math.h>

quaternion::quaternion(double w, double x, double y, double z) :
    w(w),
    x(x),
    y(y),
    z(z)
{}

float quaternion::theta()
{
    return acos(w);
}

float quaternion::vx()
{
    return x/sin(theta());
}

float quaternion::vy()
{
    return y/sin(theta());
}

float quaternion::vz()
{
    return z/sin(theta());
}

float quaternion::getw()
{
    return w;
}

float quaternion::getx()
{
    return x;
}

float quaternion::gety()
{
    return y;
}

float quaternion::getz()
{
    return z;
}

quaternion quaternion::compose(quaternion q)
{
    float neww, newx, newy, newz;
    neww = w*q.getw() - x*q.getx() - y*q.gety() - z*q.getz();
    newx = y*q.getz() - z*q.gety() + w*q.getz() + q.getw()*x;
    newy = z*q.getx() - x*q.getz() + w*q.gety() + q.getw()*y;
    newz = x*q.gety() - y*q.getx() + w*q.getz() + q.getw()*z;

    return quaternion(neww, newx, newy, newz);
}
