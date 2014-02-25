#include "bubble.h"

bubble::bubble(point pos, double speed, int lifetime) :
    pos(pos),
    speed(speed),
    lifetime(lifetime),
    age(0)
{}

bool bubble::move()
{
    bool isDead = false;

    if(age >= lifetime)
        isDead = true;
    else
    {
        pos = pos.plus(point(0, speed, 0));
        age++;
    }

    return isDead;
}

point bubble::getpos()
{
    return pos;
}
