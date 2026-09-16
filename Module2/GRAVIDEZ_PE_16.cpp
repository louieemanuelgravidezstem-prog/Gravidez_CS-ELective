#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

void drawCircle() {
    const int segments = 50;
    const float pi = 3.14159265f;

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(0.0f, 0.35f);

    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * pi * i / segments;

        glColor3f(1.0f, 0.3f, 0.6f);
        glVertex2f(
            0.0f + 0.35f * cosf(angle),
            0.35f + 0.35f * sinf(angle)
        );
    }

    glEnd();
}

void drawBase() {
    glColor3f(0.5f, 0.2f, 0.8f);

    glBegin(GL_QUADS);

    glVertex2f(-0.22f, -0.55f);
    glVertex2f(0.22f, -0.55f);
    glVertex2f(0.22f, 0.05f);
    glVertex2f(-0.22f, 0.05f);

    glEnd();
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawBase();
    drawCircle();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q16 - Fan and Quad Composition");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}