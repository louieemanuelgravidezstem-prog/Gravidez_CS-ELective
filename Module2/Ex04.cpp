#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
using namespace std;

/*
 * Example 04 - GL_LINE_LOOP (Connected, Closed)
 * ------------------------------------
 * Concept: Behaves like GL_LINE_STRIP, but OpenGL adds one extra
 * closing segment between the LAST and FIRST vertices.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex04 - Line Loop (GL_LINE_LOOP)");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}