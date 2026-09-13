#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

const float PI = 3.14159265f;

void drawSun(float centerX, float centerY, float radius) {
    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);

    for (int i = 0; i <= 60; i++) {
        float angle = 2.0f * PI * i / 60;
        glVertex2f(
            centerX + radius * cosf(angle),
            centerY + radius * sinf(angle)
        );
    }

    glEnd();
}

void display() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.1f, 0.7f, 0.2f);

    glBegin(GL_POLYGON);
    glVertex2f(-2.0f, -1.0f);
    glVertex2f(2.0f, -1.0f);
    glVertex2f(2.0f, -0.3f);
    glVertex2f(-2.0f, -0.3f);
    glEnd();

    drawSun(1.2f, 0.5f, 0.25f);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q14 - Simple Landscape");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}