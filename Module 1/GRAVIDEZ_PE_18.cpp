#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

const float PI = 3.14159265f;
float handAngle = PI / 2.0f;

void drawClockFace() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 100; i++) {
        float angle = 2.0f * PI * i / 100;
        glVertex2f(0.8f * cosf(angle), 0.8f * sinf(angle));
    }

    glEnd();

    glLineWidth(3.0f);

    glBegin(GL_LINES);

    for (int i = 0; i < 12; i++) {
        float angle = 2.0f * PI * i / 12;

        glVertex2f(0.68f * cosf(angle), 0.68f * sinf(angle));
        glVertex2f(0.78f * cosf(angle), 0.78f * sinf(angle));
    }

    glEnd();
}

void drawHand() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(5.0f);

    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(
        0.6f * cosf(handAngle),
        0.6f * sinf(handAngle)
    );
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawClockFace();
    drawHand();

    glutSwapBuffers();
}

void idle() {
    handAngle -= 0.0005f;

    if (handAngle < 0.0f) {
        handAngle += 2.0f * PI;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q18 - Rotating Clock Hand");

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}