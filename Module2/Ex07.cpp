#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

/*
 * Example 07 - Line Stippling: Dotted Line with repeatFactor
 * ------------------------------------
 * Concept: Pattern 0xAAAA alternates a single "on" bit with a single
 * "off" bit, producing fine dots. Increasing repeatFactor stretches
 * every bit in the pattern, making each dot/gap longer.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2f, 0.8f, 1.0f);

    glEnable(GL_LINE_STIPPLE);
    glLineWidth(2.0f);

    glLineStipple(1, 0xAAAA);   // fine dots 
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.7f, 0.3f);
    glVertex2f(0.7f, 0.3f);
    glEnd();

    glLineStipple(3, 0xAAAA);   // same pattern, stretched 3x 
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.7f, -0.3f);
    glVertex2f(0.7f, -0.3f);
    glEnd();

    glDisable(GL_LINE_STIPPLE);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Ex07 - Dotted Line, repeatFactor 1 vs 3");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}