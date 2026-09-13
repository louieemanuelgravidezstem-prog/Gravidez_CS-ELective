#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(0.0f, 0.7f);
    glVertex2f(-0.7f, -0.6f);
    glVertex2f(0.7f, -0.6f);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q03 - Blue Triangle Outline");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}