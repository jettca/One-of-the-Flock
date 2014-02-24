#include "quaternion.h"
#include <vector>

using namespace std;

class bird
{
    public:
        bird(point pos, double speed, quaternion rot);
        void align(vector<bird>& flock, unsigned int me);
        void move(double dt);
        void bank(double v, double dt);
        void tilt(double v, double dt);
        void twist(double v, double dt);

        point getpos();
        quaternion getrot();

    private:
        point direction();
        point center(vector<bird>& flock, unsigned int me);
        point repulsion(vector<bird>& flock, unsigned int me, double threshold);

        point pos;
        double speed;
        quaternion rot;
};
