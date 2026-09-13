#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

const float PI = 3.14159265f;
const float radius = 0.1f;

float ballX = 0.0f;
float ballY = 0.0f;
float dx = 0.005f;
float dy = 0.004f;

void drawBall() {
    glColor3f(1.0f, 0.2f, 0.2f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ballX, ballY);

    for (int i = 0; i <= 60; i++) {
        float angle = 2.0f * PI * i / 60;

        glVertex2f(
            ballX + radius * cosf(angle),
            ballY + radius * sinf(angle)
        );
    }

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBall();
    glutSwapBuffers();
}

void idle() {
    ballX += dx;
    ballY += dy;

    if (ballX + radius >= 1.0f) {
        ballX = 1.0f - radius;
        dx = -dx;
    }

    if (ballX - radius <= -1.0f) {
        ballX = -1.0f + radius;
        dx = -dx;
    }

    if (ballY + radius >= 1.0f) {
        ballY = 1.0f - radius;
        dy = -dy;
    }

    if (ballY - radius <= -1.0f) {
        ballY = -1.0f + radius;
        dy = -dy;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q16 - Bouncing Ball");

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}