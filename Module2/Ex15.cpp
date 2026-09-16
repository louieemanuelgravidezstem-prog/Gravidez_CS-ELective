#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
/*
* Example 15 - glColor3ub (RGBA / Unsigned Byte Color)
*
* Concept: glColor3ub(r, g, b) sets color using the 0-255 integer
* range instead of 0.0-1.0 floats - exactly like the RGB values you
* would pick in an image editor or color picker.
*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3ub(92, 181, 100);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); glutInitWindowSize(600, 600);
    glutCreateWindow("Ex15 - glColor3ub (0-255 Range)"); glutDisplayFunc(display);
    glutMainLoop(); return 0;
}
