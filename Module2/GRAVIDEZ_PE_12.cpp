#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUAD_STRIP);

    glColor3f(0.2f, 0.5f, 1.0f);
    glVertex2f(-0.8f, -0.6f);
    glVertex2f(-0.8f, -0.3f);

    glColor3f(0.3f, 0.7f, 0.9f);
    glVertex2f(-0.3f, -0.6f);
    glVertex2f(-0.3f, 0.0f);

    glColor3f(0.4f, 0.9f, 0.7f);
    glVertex2f(0.2f, -0.6f);
    glVertex2f(0.2f, 0.3f);

    glColor3f(0.7f, 1.0f, 0.4f);
    glVertex2f(0.7f, -0.6f);
    glVertex2f(0.7f, 0.6f);

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q12 - Staircase Ribbon");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}