#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
using namespace std;

/*
* Example 17 - Per-Vertex Color Interpolation on a Line
*
* Concept: When two endpoints of a line have DIFFERENT colors,
* OpenGL smoothly interpolates the color along the line during
* rasterization.
*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(10.0f);

    glBegin(GL_LINES);

    // Green starting point
    glColor3ub(92, 181, 100);
    glVertex2f(-0.8f, 0.0f);

    // Red ending point
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.8f, 0.0f);

    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Ex17 - Gradient Line (Per-Vertex Color)");
    glutDisplayFunc(display);
    glutMainLoop(); return 0;
}
