/* Simple opengl demo program. 
 */

//#elif defined(__linux)
#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include "bird.h"

using namespace std;

#define BUFFER_LENGTH 64
#define NUM_THINGS 20

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

// Birds
int me = 0;
vector<bird> flock;

bool start = false;

// Keys held down
bool UDLR[4] = {false, false, false, false};

void initLights()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void initFlock()
{
    srandom(time(NULL));
    for(int i = 0; i < NUM_THINGS; i++)
    {
        quaternion q = makeQuaternion(180, point(0, 1, 0));

        double x = 3*(i%5);
        double y = 0;
        double z = 3*(i/5);
        double speed = 2.5;

        bird b(point(x, y, z), speed, q);
        flock.push_back(b);
    }
}

void setupRC()
{
    // Place Camera
    camRotX = 90.0f;
    camRotY = 0.0f;
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
    //glRotatef(camRotX, 1, 0, 0);
    //glRotatef(camRotY, 0, 1, 0);
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

void drawThings()
{
    for(int i = 0; i < NUM_THINGS; i++)
    {
        bird b = flock.at(i);
        glPushMatrix();
        {
            glTranslatef(b.getpos().getx(), b.getpos().gety(), b.getpos().getz());
            quaternion rot = b.getrot();
            glRotatef(rot.theta(), rot.vx(), rot.vy(), rot.vz());
            glCallList(fishDrawList);
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
        drawThings();

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

void special(int key, int x, int y)
{
    quaternion q;
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

void updateMe()
{
    if(UDLR[0])
    {
        flock.at(me).tilt(6, 1);
    }
    if(UDLR[1])
    {
        flock.at(me).tilt(-6, 1);
    }
    if(UDLR[2])
    {
        flock.at(me).bank(-6, 1);
    }
    if(UDLR[3])
    {
        flock.at(me).bank(6, 1);
    }
}

void updateFlock()
{
    for(int i = 0; i < NUM_THINGS; i++)
    {
        flock.at(i).move(.1);
        if(i != me)
            flock.at(i).align(flock, i);
    }
}

void updateCamera()
{
    point mypos = flock.at(me).getpos();
    camPosX = mypos.getx();
    camPosY = mypos.gety();
    camPosZ = mypos.getz() + 30.0f;
}

void update()
{
    if(start)
    {
        updateMe();
        updateFlock();
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
    initFlock();
    loadFish();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutIdleFunc(update);

    glutMainLoop();
}
