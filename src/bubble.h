#ifndef __POINT_H__
#   define __POINT_H__
#   include "point.h"
#endif

class bubble
{
    public:
        bubble(point pos, double speed, int lifetime);

        /* moves bubble up by speed
         * returns true if bubble has outlived its lifetime
         */
        bool move();

        point getpos();

    private:
        point pos;
        double speed;
        int lifetime, age;
};
