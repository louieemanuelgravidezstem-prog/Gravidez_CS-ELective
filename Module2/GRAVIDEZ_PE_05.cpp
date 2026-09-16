#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);

    glBegin(GL_LINES);

    glVertex2f(-0.8f, 0.0f);
    glVertex2f(0.8f, 0.0f);

    glEnd();

    glDisable(GL_LINE_STIPPLE);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Q05 - Dashed Line");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}