#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

/*
 * Example 01 - Points with Per-Vertex Color
 * ------------------------------------
 * Concept: GL_POINTS renders one point per vertex. Calling glColor3f
 * before each glVertex2f gives each point its own color.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(6.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(-0.5f, -0.5f); // white 
    glColor3f(1.0f, 0.0f, 1.0f); glVertex2f(-0.5f, 0.5f); // magenta 
    glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.5f, 0.5f); // blue 
    glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.5f, -0.5f); // green 
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex01 - Colored Points (GL_POINTS)");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}