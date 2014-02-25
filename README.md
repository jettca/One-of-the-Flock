One of the Flock
==========
A simple flocking simulator for a school of fish.  You control the leader!

Contents:
I.      Directions
II.     Features
III.    Implementation
IV.     Sources

I. Directions
Press "s" to start
Use left and right arrow keys to make the leader bank left or right
Use up and down arrow keys to make the leader tilt up or down


II. Features
A school of 20 fish are generated at the start of the program.  You control one
of them.  All fish move at a constant speed in the direction they begin facing.

The fish are guided by two desires: to move towards the center of
the school, and to maintain distance from each other.  Your movements influence
the rest of the school since you are the only one able to significantly change
the center of the school.


III. Implementation
The primary implementation details of note regard the automatic rotation of the
school of fish, which was by far the most challenging part of the project.  I
implemented quaternions for rotation, taking advantage of their convenience for
interpolation and composition.  In order to get the fish to rotate relatively
smoothly towards their target, I had to interpolate the quaternion for their
target rotation with that for their current rotation.

Also of interest are the mechanics for flocking, though as of yet they are
fairly simple.  Attraction and repulsion vectors are calculated, added, and set
as the target direction for each fish.  These are calculated on a fish-by-fish
basis, making this a "swarm" algorithm, and a mild case of emergent complexity.
The attraction vector for a given fish is the average of the positions of all
other fish, minus the fish's current position.  The repulsion vector is
calculated as the sum over all other fish of 1/e^x, where x is the distance
between the two fish.


IV. Sources
Ideas for how to implement flocking came from
http://en.wikipedia.org/wiki/Flocking\_(behavior).

I spent a lot of time with the online OpenGL Red Book, found here
http://www.glprogramming.com/red/index.html.

The OpenGL discussion boards, StackOverflow, and gamedev.net forums were all
incredibly useful for learning and debugging OpenGL techniques.
