#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

/*
* Example 09 - GL_POLYGON
*
* Concept: GL_POLYGON draws a single filled convex n-gon. OpenGL
* renders an n-sided polygon where n is the number of vertices
* specified; if n < 3, OpenGL renders nothing.
*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT); glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POLYGON); glVertex2f(-0.4f, 0.1f); glVertex2f(0.1f, 0.1f); glVertex2f(0.2f, 0.3f); glVertex2f(0.1f, 0.6f); glVertex2f(-0.4f, 0.6f);
    glVertex2f(-0.5f, 0.3f); glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); glutInitWindowSize(600, 600);
    glutCreateWindow("Ex09 - Filled Hexagon (GL_POLYGON)"); glutDisplayFunc(display);
    glutMainLoop(); return 0;
}
