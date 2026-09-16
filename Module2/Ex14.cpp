#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


/*
* Example 14 - glColor3f (RGBA / Float Color)
*
* Concept: In RGBA mode, glColor3f(r, g, b) maps available red,
* green, and blue intensities onto the range [0.0, 1.0], where 0.0
* means the component is absent and 1.0 is fully saturated.
*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 1.0f); // pure blue glLineWidth(2.0f);
    glBegin(GL_LINES); glVertex2f(-0.8f, 0.0f); glVertex2f(0.8f, 0.0f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); glutInitWindowSize(600, 400); glutCreateWindow("Ex14 - glColor3f"); glutDisplayFunc(display); glutMainLoop();
    return 0;
}
