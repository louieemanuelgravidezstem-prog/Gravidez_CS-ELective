#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;


/*
* Example 11 - GL_TRIANGLE_FAN
*
* Concept: Draws a fan of triangles that share edges AND share the
* first vertex specified. With vertices v0..v5, OpenGL renders
* triangles (v0,v1,v2), (v0,v2,v3), (v0,v3,v4), (v0,v4,v5).
*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, -0.5f); // v0 - shared center
    glVertex2f(0.5f, 0.5f);  // v1
    glVertex2f(0.0f, 0.5f);  // v2
    glVertex2f(-0.5f, 0.5f); // v3
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex11 - Triangle Fan (GL_TRIANGLE_FAN)");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}