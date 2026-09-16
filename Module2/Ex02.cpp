#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

/*
 * Example 02 - GL_LINES (Unconnected Segments)
 * ------------------------------------
 * Concept: OpenGL draws a line segment for each group of TWO vertices.
 * With n vertices, OpenGL renders n/2 segments; an odd leftover vertex
 * is ignored.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, 0.5f);   // segment 1 (vertical, left) 
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);   // segment 2 (vertical, right) 
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex02 - Unconnected Lines (GL_LINES)");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}