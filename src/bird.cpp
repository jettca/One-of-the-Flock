#include "bird.h"

bird::bird(double x, double y, double z, double speed, quaternion direction) :
    x(x),
    y(y),
    z(z),
    speed(speed),
    direction(direction)
{}

void bird::align(vector<bird> flock)
{
}

void bird::move(double dt)
{
    double dx, dy, dz;

    dx = speed*dt;
    dy = 0;
    dz = 0;
    direction.rotate(dx, dy, dz);

    x += dx;
    y += dy;
    z += dz;
}

void bird::bank(double v, double dt)
{
    direction = direction.compose(makeQuaternion(v*dt, 1, 0, 0));
}

void bird::tilt(double v, double dt)
{
    direction = direction.compose(makeQuaternion(v*dt, 0, 0, 1));
}

double bird::x()
{
    return x;
}

double bird::y()
{
    return y;
}

double bird::z()
{
    return z;
}

quaternion bird::dir()
{
    return direction;
}
