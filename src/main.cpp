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

struct rotation {
    GLfloat theta;
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct thing {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    struct rotation r;
};

// Things
struct thing things[NUM_THINGS];
unsigned int me = 0;

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
        struct rotation r;
        r.theta = 360*random()/RAND_MAX;
        r.x = 0;
        r.y = 1;
        r.z = 0;
        
        struct thing t;
        t.x = 3*(i%5) - 3*5/2;
        t.y = 3*(i/5);
        t.z = 0;
        t.r = r;
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
    camPosZ = -20.0f;
    
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
            glRotatef(pot.r.theta, pot.r.x, pot.r.y, pot.r.z);
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

int main (int argc, char *argv[])
{
    int win_width = 512;
    int win_height = 512;

    initThings();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);

    glutCreateWindow("jett has a game");
    setupRC();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}
