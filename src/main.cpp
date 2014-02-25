#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include "fish.h"
#include "bubble.h"

#include <stdlib.h>
#include <iostream>
#include <deque>


using namespace std;

#define BUFFER_LENGTH 64
#define NUM_FISH 20
#define BUBBLE_RATE 500
#define BUBBLE_TIME 50
#define BUBBLE_SPEED .1

GLfloat camRotX, camRotY, camPosX, camPosY, camPosZ;
GLuint fishDrawList;
GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];

GLuint pickedObj = -1;
char titleString[150];

// Lights & Materials
GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {50.0};

// Fish and bubbles
int me = 0;
vector<fish> school;
deque<bubble> bubbles;

// For changing initial behavior
bool start = false;
bool keyhit = false;

// Keys held down
bool UDLR[4] = {false, false, false, false};

void initLights()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL) ;
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

double randDouble()
{
    return (double)random()/RAND_MAX;
}

void initSchool()
{
    // generate school in large grid, initially
    srandom(time(NULL));
    for(int i = 0; i < NUM_FISH; i++)
    {
        quaternion q = makeQuaternion(360.*random()/RAND_MAX, point(0, 1, 0));

        double x = 10*(i%5);
        double y = 0;
        double z = 10*(i/5);
        double speed = 2.5;

        point color(randDouble(), randDouble(), randDouble());
        fish f(point(x, y, z), color, speed, q);
        school.push_back(f);
    }
}

void setupRC()
{
    // Place Camera
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = -30.0f;
    
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    initLights();
}

void setCamera()
{
    glTranslatef(-camPosX, -camPosY, -camPosZ);
}

point getFaceNormal(point a, point b, point c)
{
    return a.plus(b.times(-1)).cross(c.plus(b.times(-1))).normalize();
}

void vertex(point p)
{
    glVertex3f(p.getx(), p.gety(), p.getz());
}

void normal(point p)
{
    glNormal3f(p.getx(), p.gety(), p.getz());
}

// Draw triangle with normal (a, b, c go clockwise)
void triangle(point a, point b, point c)
{
    point norm = getFaceNormal(a, b, c);
    normal(norm);
    vertex(a);
    normal(norm);
    vertex(b);
    normal(norm);
    vertex(c);
}

// Precompile fish drawing
void loadFish()
{
    fishDrawList = glGenLists(1);
    glNewList(fishDrawList, GL_COMPILE);
    {
        glBegin(GL_TRIANGLES);
        {
            // Torso
            point tFront(1, 0.3, 0);
            point tBottom(0, -0.3, 0);
            point tRight(0, 0.3, 0.3);
            point tLeft(0, 0.3, -0.3);

            triangle(tFront, tRight, tLeft);
            triangle(tFront, tBottom, tRight);
            triangle(tFront, tLeft, tBottom);
            triangle(tRight, tBottom, tLeft);

            // Tail fin
            point tfFront(0, 0, 0);
            point tfTop(-0.5, 0.3, 0);
            point tfRight(-0.5, -0.2, 0.2);
            point tfLeft(-0.5, -0.2, -0.2);

            triangle(tfFront, tfTop, tfLeft);
            triangle(tfFront, tfRight, tfTop);
            triangle(tfFront, tfLeft, tfRight);
            triangle(tfTop, tfRight, tfLeft);

            // Back fin
            point bfFront(0.6, 0.3, 0);
            point bfTop(0.35, 0.6, 0);
            point bfRight(0.2, 0.3, 0.1);
            point bfLeft(0.2, 0.3, -0.1);

            triangle(bfFront, bfTop, bfLeft);
            triangle(bfFront, bfRight, bfTop);
            triangle(bfTop, bfRight, bfLeft);
        }
        glEnd();
    }
    glEndList();
}

void drawSchool()
{
    for(int i = 0; i < NUM_FISH; i++)
    {
        glPushMatrix();
        {
            fish f = school.at(i);
            point fpos = f.getpos();
            glTranslatef(fpos.getx(), fpos.gety(), fpos.getz());
            quaternion rot = f.getrotation();
            glRotatef(rot.theta(), rot.vx(), rot.vy(), rot.vz());
            
            point col = f.getcolor();
            glColor4f(col.getx(), col.gety(), col.getz(), 1.0f);
            glCallList(fishDrawList);
        }
        glPopMatrix();
    }
}

void drawBubbles()
{
    int bsize = bubbles.size();
    glColor4f(0.0f, 0.5f, 1.0f, 0.5f);
    for(int i = 0; i < bsize; i++)
    {
        glPushMatrix();
        {
            point bpos = bubbles.at(i).getpos();
            glTranslatef(bpos.getx(), bpos.gety(), bpos.getz());
            glutSolidSphere(.1, 10, 10);
        }
        glPopMatrix();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    {
        setCamera();
        drawSchool();
        drawBubbles();

        // Retrieve current matrices before they are popped
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);        // Retrieve The Modelview Matrix
        glGetDoublev(GL_PROJECTION_MATRIX, projection);      // Retrieve The Projection Matrix
        glGetIntegerv(GL_VIEWPORT, viewport);                // Retrieve The Viewport Values
    }
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set the clipping volume
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: // Escape key
            exit(0);
            break;
        case 's':
            start = true;
            break;
    }
}

// Handle arrow keys
void special(int key, int x, int y)
{
    if(start)
        keyhit = true;
    switch(key)
    {
        case GLUT_KEY_UP:
            UDLR[0] = true;
            break;
        case GLUT_KEY_DOWN:
            UDLR[1] = true;
            break;
        case GLUT_KEY_LEFT:
            UDLR[2] = true;
            break;
        case GLUT_KEY_RIGHT:
            UDLR[3] = true;
            break;
    }
}

// Handle arrow key release
void specialUp(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            UDLR[0] = false;
            break;
        case GLUT_KEY_DOWN:
            UDLR[1] = false;
            break;
        case GLUT_KEY_LEFT:
            UDLR[2] = false;
            break;
        case GLUT_KEY_RIGHT:
            UDLR[3] = false;
            break;
    }
}

// Update motion based on arrow key presses
void updateMe()
{
    if(UDLR[0])
    {
        school.at(me).tilt(6, 1);
    }
    if(UDLR[1])
    {
        school.at(me).tilt(-6, 1);
    }
    if(UDLR[2])
    {
        school.at(me).bank(-6, 1);
    }
    if(UDLR[3])
    {
        school.at(me).bank(6, 1);
    }
}

void updateSchool()
{
    for(int i = 0; i < NUM_FISH; i++)
    {
        school.at(i).move(.1);
        if(!keyhit || i != me)
            school.at(i).align(school, i);
    }
}

void updateBubbles()
{
    if(randDouble() > (BUBBLE_RATE - 1.0)/BUBBLE_RATE);
    {
        double bx = camPosX + 40.0*random()/RAND_MAX - 20;
        double by = camPosY + 40.0*random()/RAND_MAX - 20;
        double bz = camPosZ - 40.0*random()/RAND_MAX;

        point pos(bx, by, bz);
        bubble b(pos, BUBBLE_SPEED, (1 + randDouble())*BUBBLE_TIME);
        bubbles.push_back(b);
    }

    int bsize = bubbles.size();
    for(int i = 0; i < bsize; i++)
    {
        if(bubbles.at(i).move())
        {
            bubbles.pop_front();
            i--;
            bsize--;
        }
    }
}

void updateCamera()
{
    point mypos = school.at(me).getpos();
    camPosX = mypos.getx();
    camPosY = mypos.gety();
    camPosZ = mypos.getz() + 30.0f;
}

void update()
{
    if(start)
    {
        updateMe();
        updateSchool();
        updateBubbles();
        updateCamera();
        glutPostRedisplay();
    }
}

int main(int argc, char *argv[])
{
    int win_width = 960;
    int win_height = 540;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);

    glutCreateWindow("jett has a thing");

    setupRC();
    initSchool();
    loadFish();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutIdleFunc(update);

    glutMainLoop();
}
