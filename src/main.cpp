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
#include "GLScreenCapturer.h"
#include "trackball.h"
#include "quaternion.h"

using namespace std;

#define BUFFER_LENGTH 64
#define NUM_THINGS 10

GLfloat camRotX, camRotY, camPosX, camPosY, camPosZ;
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

struct thing {
    GLfloat x;
    GLfloat y;
    GLfloat z;

    GLfloat speed;

    quaternion q;
};

// Things
struct thing things[NUM_THINGS];
unsigned int me = 0;

// Keys held down
bool UDLR[4] = {false, false, false, false};

static GLScreenCapturer screenshot("screenshot-%d.ppm");

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

void initThings()
{
    srandom(time(NULL));
    int i;
    for(i = 0; i < NUM_THINGS; i++)
    {
        struct thing t;
        t.x = 3*(i%5) - 3*5/2;
        t.y = 0;
        t.z = 3*(i/5);

        t.speed = 2;

        t.q = makeQuaternion(360*random()/RAND_MAX, 0, 1, 0);
        things[i] = t;
    }
}

void setupRC()
{
    tbInit(GLUT_RIGHT_BUTTON);
    tbAnimate(GL_TRUE);
    
    // Place Camera
    camRotX = 0.0f;
    camRotY = 0.0f;
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = -30.0f;
    
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initLights();
}

void setCamera()
{
    glTranslatef(0, 0, camPosZ);
    glRotatef(camRotX, 1, 0, 0);
    glRotatef(camRotY, 0, 1, 0);
}

void drawThings()
{
    int i;
    for(i = 0; i < NUM_THINGS; i++)
    {
        struct thing pot = things[i];
        glPushMatrix();
        {
            glTranslatef(pot.x, pot.y, pot.z);
            glRotatef(pot.q.theta(), pot.q.vx(), pot.q.vy(), pot.q.vz());
            glutSolidTeapot(1);
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
        tbMatrix();
        
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
    tbReshape(w, h);

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
        case 'r':
            printf("save current screen\n");
            screenshot.capture();
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
    quaternion q;
    if(UDLR[0])
    {
        q = things[me].q.compose(makeQuaternion(6, 0, 0, 1));
        things[me].q = quaternion(q.getw(), q.getx(), q.gety(), q.getz());
    }
    if(UDLR[1])
    {
        q = things[me].q.compose(makeQuaternion(-6, 0, 0, 1));
        things[me].q = quaternion(q.getw(), q.getx(), q.gety(), q.getz());
    }
    if(UDLR[2])
    {
        q = things[me].q.compose(makeQuaternion(-6, 1, 0, 0));
        things[me].q = quaternion(q.getw(), q.getx(), q.gety(), q.getz());
    }
    if(UDLR[3])
    {
        q = things[me].q.compose(makeQuaternion(6, 1, 0, 0));
        things[me].q = quaternion(q.getw(), q.getx(), q.gety(), q.getz());
    }
}

void updateThings()
{
    int i;
    double dx, dy, dz;
    for(i = 0; i < NUM_THINGS; i++)
    {
        dx = .1;
        dy = 0;
        dz = 0;

        things[i].q.rotate(dx, dy, dz);
        things[i].x += dx;
        things[i].y += dy;
        things[i].z += dz;
    }
}

void update()
{
    updateMe();
    updateThings();
    glutPostRedisplay();
}

int main (int argc, char *argv[])
{
    int win_width = 800;
    int win_height = 680;

    initThings();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);

    glutCreateWindow("jett has a game");
    setupRC();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutIdleFunc(update);

    glutMainLoop();
}
