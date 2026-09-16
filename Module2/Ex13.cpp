#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


/*
* Example 13 - GL_QUAD_STRIP
*
* Concept: Draws a sequence of quadrilaterals that share edges. With
* vertices v0..v5, OpenGL renders a quad using (v0,v1,v3,v2), then
* another using (v2,v3,v5,v4). Vertices are supplied in pairs
* (bottom, top) as you move across the strip.
*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUAD_STRIP);

    glVertex2f(-0.5f, -0.5f); // v0
    glVertex2f(-0.5f, 0.0f); // v1

    glVertex2f(0.0f, -0.5f); // v2
    glVertex2f(0.0f, 0.0f); // v3

    glVertex2f(0.5f, -0.3f); // v4
    glVertex2f(0.5f, 0.3f); // v5

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex13 - Quad Strip (GL_QUAD_STRIP)");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}