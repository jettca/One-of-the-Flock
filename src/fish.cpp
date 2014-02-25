#include "fish.h"

#include <cmath>
#include <iostream>

using namespace std;

fish::fish(point pos, point color, double speed, quaternion rotation) :
    pos(pos),
    speed(speed),
    rotation(rotation),
    color(color)
{}

void fish::align(vector<fish>& flock, unsigned int me)
{
    point c = center(flock, me);
    point r = repulsion(flock, me, 1).times(2);

    point target = c.plus(r).normalize();
    point dir = direction().normalize();
    point normal = dir.cross(target);


    double theta = 360*acos(target.dot(dir))/M_PI;
    quaternion final = makeQuaternion(theta, normal).compose(rotation);
    final.normalize();
    rotation.normalize();
    rotation = rotation.interpolate(final, .05);
}

point fish::center(vector<fish>& flock, unsigned int me)
{
    point avg(0, 0, 0);

    unsigned int size = flock.size();
    for(int i = 0; i < size; i++)
    {
        if(i != me)
            avg = avg.plus(flock.at(i).getpos());
    }
    avg = avg.times(1./(size - 1));
    return avg.plus(pos.times(-1));
}

point fish::repulsion(vector<fish>& flock, unsigned int me, double threshold)
{
    point repulsion(0, 0, 0);

    unsigned int size = flock.size();
    for(int i = 0; i < size; i++)
    {
        if(i != me)
        {
            point themToMe = pos.plus(flock.at(i).getpos().times(-1));
            double dist = themToMe.magnitude();
            repulsion = repulsion.plus(themToMe.times(5/(exp(dist))));
        }
    }
    return repulsion;
}

void fish::move(double dt)
{
    pos = pos.plus(direction().times(speed*dt));
}

void fish::bank(double v, double dt)
{
    rotation = rotation.compose(makeQuaternion(v*dt, point(1, 0, 0)));
}

void fish::tilt(double v, double dt)
{
    rotation = rotation.compose(makeQuaternion(v*dt, point(0, 0, 1)));
}

void fish::twist(double v, double dt)
{
    rotation = rotation.compose(makeQuaternion(v*dt, point(0, 1, 0)));
}

point fish::getpos()
{
    return pos;
}

point fish::getcolor()
{
    return color;
}

quaternion fish::getrotation()
{
    return rotation;
}

point fish::direction()
{
    point dir(1, 0, 0);
    return rotation.rotate(dir);
}
