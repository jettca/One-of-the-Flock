#include "point.h"

class quaternion
{
    public:
        quaternion();
        quaternion(double w, double x, double y, double z);
        quaternion(double w, point p);
        double theta();
        double vx();
        double vy();
        double vz();

        double getw();
        double getx();
        double gety();
        double getz();

        quaternion compose(quaternion q);
        quaternion interpolate(quaternion q, double h);
        void normalize();
        double magnitude();
        point rotate(point p);
        void conjugate();

        void print();

    private:
        double w, x, y, z;
};

quaternion makeQuaternion(double theta, point p);
