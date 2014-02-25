#include "quaternion.h"
#include <vector>

using namespace std;

class bird
{
    public:
        /* Creates a bird object with given position, speed,
         * and initial rotation
         */
        bird(point pos, double speed, quaternion rotation);

        /* Aligns the bird to the rest of the flock
         * me is the bird's position in flock
         */
        void align(vector<bird>& flock, unsigned int me);

        /* Moves the bird forward by speed*dt
         */
        void move(double dt);

        /* Rotates the bird around the initial z-axis
         */
        void bank(double v, double dt);

        /* Rotates the bird around the initial x-axis
         */
        void tilt(double v, double dt);

        /* Rotates the bird around the initial y-axis
         */
        void twist(double v, double dt);

        point getpos();
        quaternion getrotation();

    private:

        /* Calculates current direction from rotation quaternion
         */
        point direction();

        /* Returns direction to center of flock
         */
        point center(vector<bird>& flock, unsigned int me);

        /* Returns force away from flock
         */
        point repulsion(vector<bird>& flock, unsigned int me, double threshold);

        point pos;
        double speed;
        quaternion rotation;
};
