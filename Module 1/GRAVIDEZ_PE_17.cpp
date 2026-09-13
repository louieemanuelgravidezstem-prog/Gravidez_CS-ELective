#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

const float PI = 3.14159265f;
int activeLight = 0;

void drawCircle(float centerX, float centerY, float radius,
    float r, float g, float b) {
    glColor3f(r, g, b);

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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.05f, 0.05f, 0.05f);

    glBegin(GL_POLYGON);
    glVertex2f(-0.35f, -0.85f);
    glVertex2f(0.35f, -0.85f);
    glVertex2f(0.35f, 0.85f);
    glVertex2f(-0.35f, 0.85f);
    glEnd();

    if (activeLight == 0) {
        drawCircle(0.0f, 0.5f, 0.18f, 1.0f, 0.0f, 0.0f);
    }
    else {
        drawCircle(0.0f, 0.5f, 0.18f, 0.2f, 0.0f, 0.0f);
    }

    if (activeLight == 1) {
        drawCircle(0.0f, 0.0f, 0.18f, 1.0f, 1.0f, 0.0f);
    }
    else {
        drawCircle(0.0f, 0.0f, 0.18f, 0.2f, 0.2f, 0.0f);
    }

    if (activeLight == 2) {
        drawCircle(0.0f, -0.5f, 0.18f, 0.0f, 1.0f, 0.0f);
    }
    else {
        drawCircle(0.0f, -0.5f, 0.18f, 0.0f, 0.2f, 0.0f);
    }

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'n' || key == 'N') {
        activeLight = (activeLight + 1) % 3;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q17 - Traffic Light Simulator");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
};