#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

/*
 * Example 03 - GL_LINE_STRIP (Connected, Open)
 * ------------------------------------
 * Concept: OpenGL renders a segment between vertex 1-2, 2-3, 3-4, and
 * so on - a connected chain that does NOT close back to the start.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINE_STRIP);
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
    glutCreateWindow("Ex03 - Line Strip (GL_LINE_STRIP)");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}