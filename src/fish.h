#include "quaternion.h"
#include <vector>

using namespace std;

class fish
{
    public:
        /* Creates a fish object with given position, speed,
         * and initial rotation
         */
        fish(point pos, point color, double speed, quaternion rotation);

        /* Aligns the fish to the rest of the school
         * me is the fish's position in school
         */
        void align(vector<fish>& school, unsigned int me);

        /* Moves the fish forward by speed*dt
         */
        void move(double dt);

        /* Rotates the fish around the initial z-axis
         */
        void bank(double v, double dt);

        /* Rotates the fish around the initial x-axis
         */
        void tilt(double v, double dt);

        /* Rotates the fish around the initial y-axis
         */
        void twist(double v, double dt);

        point getpos();
        point getcolor();
        quaternion getrotation();

    private:

        /* Calculates current direction from rotation quaternion
         */
        point direction();

        /* Returns direction to center of school
         */
        point center(vector<fish>& school, unsigned int me);

        /* Returns force away from school
         */
        point repulsion(vector<fish>& school, unsigned int me, double threshold);

        point pos;
        point color;
        double speed;
        quaternion rotation;
};
