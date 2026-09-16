#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    // First quadrilateral
    glVertex2f(-0.2f, 0.2f);
    glVertex2f(-0.5f, -0.2f);
    glVertex2f(0.5f, -0.2f);
    glVertex2f(0.2f, 0.2f);

    // Second quadrilateral
    glVertex2f(0.2f, 0.4f);
    glVertex2f(0.6f, 0.4f);
    glVertex2f(0.6f, 0.6f);
    glVertex2f(0.2f, 0.6f);

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Ex12 - Two Independent Quads");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}