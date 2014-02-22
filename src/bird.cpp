#include "bird.h"

bird::bird(double x, double y, double z, double speed, quaternion direction) :
    x(x),
    y(y),
    z(z),
    speed(speed),
    direction(direction)
{}

void bird::align(vector<bird>& flock)
{
    double avg_x, avg_y, avg_z;
    avg_x = 0;
    avg_y = 0;
    avg_z = 0;

    unsigned int size = flock.size();
    for(int i = 0; i < size; i++)
    {
        avg_x += flock.at(i).getx();
        avg_y += flock.at(i).gety();
        avg_z += flock.at(i).getz();
    }
    avg_x /= size;
    avg_y /= size;
    avg_z /= size;

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

double bird::getx()
{
    return x;
}

double bird::gety()
{
    return y;
}

double bird::getz()
{
    return z;
}

quaternion bird::getdir()
{
    return direction;
}
