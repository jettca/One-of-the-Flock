#include "quaternion.h"
#include <vector>

using namespace std;

class bird
{
    public:
        bird(double x, double y, double z, double speed, quaternion direction);
        void align(vector<bird> flock);
        void move(double dt);
        void bank(double v, double dt);
        void tilt(double v, double dt);

        double x();
        double y();
        double z();
        quaternion dir();

    private:
        double x, y, z, speed;
        quaternion direction;
};
