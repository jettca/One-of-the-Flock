#include "bird.h"

#include <cmath>
#include <iostream>

using namespace std;

bird::bird(point pos, double speed, quaternion rot) :
    pos(pos),
    speed(speed),
    rot(rot)
{}

void bird::align(vector<bird>& flock, unsigned int me)
{
    point c = center(flock, me);
    point r = repulsion(flock, me, 1).times(2);

    point target = c.plus(r).normalize();
    point dir = direction().normalize();
    point normal = dir.cross(target);


    double theta = 360*acos(target.dot(dir))/M_PI;

    quaternion final = makeQuaternion(theta, normal).compose(rot);
    final.normalize();
    rot.normalize();
    rot = rot.interpolate(final, .05);
}

point bird::center(vector<bird>& flock, unsigned int me)
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

point bird::repulsion(vector<bird>& flock, unsigned int me, double threshold)
{
    point repulsion(0, 0, 0);

    unsigned int size = flock.size();
    for(int i = 0; i < size; i++)
    {
        if(i != me)
        {
            point themToMe = pos.plus(flock.at(i).getpos().times(-1)).normalize();
            double dist = themToMe.magnitude();
            repulsion = repulsion.plus(themToMe.times(1/exp(dist)));
        }
    }
    return repulsion;
}

void bird::move(double dt)
{
    pos = pos.plus(direction().times(speed*dt));
}

void bird::bank(double v, double dt)
{
    rot = rot.compose(makeQuaternion(v*dt, point(1, 0, 0)));
}

void bird::tilt(double v, double dt)
{
    rot = rot.compose(makeQuaternion(v*dt, point(0, 0, 1)));
}

point bird::getpos()
{
    return pos;
}

quaternion bird::getrot()
{
    return rot;
}

point bird::direction()
{
    point dir(1, 0, 0);
    return rot.rotate(dir);
}
