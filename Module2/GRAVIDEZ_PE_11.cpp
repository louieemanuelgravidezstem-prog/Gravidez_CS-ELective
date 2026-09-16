#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.2f, 0.2f);
    glVertex2f(-0.8f, -0.4f);
    glVertex2f(-0.2f, -0.4f);
    glVertex2f(-0.2f, 0.4f);
    glVertex2f(-0.8f, 0.4f);

    glColor3f(0.2f, 0.4f, 1.0f);
    glVertex2f(0.2f, -0.4f);
    glVertex2f(0.8f, -0.4f);
    glVertex2f(0.8f, 0.4f);
    glVertex2f(0.2f, 0.4f);

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q11 - Two Quads");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}