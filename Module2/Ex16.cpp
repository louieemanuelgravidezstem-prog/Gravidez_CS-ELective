#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
using namespace std;

/*
 * Example 16 - glColor4f and Alpha Blending
 *
 * Alpha controls opacity:
 * 1.0 = fully opaque
 * 0.0 = fully transparent
 */

void display() {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 60% opaque red rectangle
    glColor4f(1.0f, 0.0f, 0.0f, 0.6f);

    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.3f);
    glVertex2f(0.1f, -0.3f);
    glVertex2f(0.1f, 0.3f);
    glVertex2f(-0.5f, 0.3f);
    glEnd();

    // 60% opaque blue rectangle
    glColor4f(0.0f, 0.4f, 1.0f, 0.6f);

    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.3f);
    glVertex2f(0.5f, -0.3f);
    glVertex2f(0.5f, 0.3f);
    glVertex2f(-0.1f, 0.3f);
    glEnd();

    glDisable(GL_BLEND);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Ex16 - Alpha Blending (glColor4f)");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}