#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream> 
using namespace std;

/*
 * Example 06 - Line Stippling: Dashed Line
 * ------------------------------------
 * Concept: glLineStipple(repeatFactor, pattern) breaks a solid line
 * into a repeating pattern of dots/dashes. Pattern 0x00FF is 8 bits
 * "on" followed by 8 bits "off". You must glEnable(GL_LINE_STIPPLE)
 * before drawing, and it's good practice to glDisable it afterward.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_LINE_STIPPLE);
    glLineWidth(2.0f);
    glLineStipple(1, 0x00FF);   // 8-on / 8-off, repeated once per bit 

    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.7f, 0.0f);
    glVertex2f(0.7f, 0.0f);
    glEnd();

    glDisable(GL_LINE_STIPPLE);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Ex06 - Dashed Line (0x00FF)");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}