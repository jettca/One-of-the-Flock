#include "quaternion.h"

#include <iostream>
#include <math.h>
using namespace std;

quaternion::quaternion() :
    w(0),
    x(0),
    y(1),
    z(0)
{}

quaternion::quaternion(double w, double x, double y, double z) :
    w(w),
    x(x),
    y(y),
    z(z)
{}

double quaternion::theta()
{
    return acos(w)*360/M_PI;
}

double quaternion::vx()
{
    return x/sin(acos(w));
}

double quaternion::vy()
{
    return y/sin(acos(w));
}

double quaternion::vz()
{
    return z/sin(acos(w));
}

double quaternion::getw()
{
    return w;
}

double quaternion::getx()
{
    return x;
}

double quaternion::gety()
{
    return y;
}

double quaternion::getz()
{
    return z;
}

quaternion quaternion::compose(quaternion q)
{
    double neww, newx, newy, newz;
    neww = w*q.getw() - x*q.getx() - y*q.gety() - z*q.getz();
    newx = y*q.getz() - z*q.gety() + w*q.getx() + q.getw()*x;
    newy = z*q.getx() - x*q.getz() + w*q.gety() + q.getw()*y;
    newz = x*q.gety() - y*q.getx() + w*q.getz() + q.getw()*z;

    return quaternion(neww, newx, newy, newz);
}

void quaternion::normalize()
{
    double m = magnitude();
    x /= m;
    x /= m;
    y /= m;
    z /= m;
}

double quaternion::magnitude()
{
    return sqrt(w*w + x*x + y*y + z*z);
}

void quaternion::rotate(double &vx, double &vy, double &vz)
{
    quaternion v = quaternion(0, vx, vy, vz);
    quaternion qconj = quaternion(w, -x, -y, -z);
    quaternion p = (*this).compose(v.compose(qconj));

    vx = p.x;
    vy = p.y;
    vz = p.z;
}

void quaternion::conjugate()
{
    x = -x;
    y = -y;
    z = -z;
}

quaternion makeQuaternion(double theta, double x, double y, double z)
{
    theta = theta*M_PI/360;
    double st = sin(theta);
    quaternion q(cos(theta), st*x, st*y, st*z);
    q.normalize();
    return q;
}
