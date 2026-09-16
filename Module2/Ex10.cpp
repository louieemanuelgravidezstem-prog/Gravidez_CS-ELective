#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

/*
* Example 10 - GL_TRIANGLE_STRIP
*
* Concept: Draws a sequence of triangles that share edges. OpenGL
* renders a triangle using vertices 1-2-3, then another using
* 2-3-4, then 3-4-5, and so on.
*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT); glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f); glVertex2f(0.0f, 0.5f); glVertex2f(0.7f, 0.5f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); glutInitWindowSize(600, 600);
    glutCreateWindow("Ex10 - Triangle Strip (GL_TRIANGLE_STRIP)"); glutDisplayFunc(display);
    glutMainLoop(); return 0;
}
