#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

const float PI = 3.14159265f;
const int STRIPE_COUNT = 8;

void drawStripes() {
    float stripeHeight = 2.0f / STRIPE_COUNT;

    for (int i = 0; i < STRIPE_COUNT; i++) {
        float yTop = 1.0f - i * stripeHeight;
        float yBottom = yTop - stripeHeight;

        if (i % 2 == 0) {
            glColor3f(0.8f, 0.0f, 0.0f);
        }
        else {
            glColor3f(1.0f, 1.0f, 1.0f);
        }

        glBegin(GL_POLYGON);
        glVertex2f(-1.0f, yBottom);
        glVertex2f(1.0f, yBottom);
        glVertex2f(1.0f, yTop);
        glVertex2f(-1.0f, yTop);
        glEnd();
    }
}

void drawStar() {
    float centerX = -0.65f;
    float centerY = 0.60f;
    float outerRadius = 0.25f;
    float innerRadius = 0.10f;

    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_POLYGON);

    for (int i = 0; i < 10; i++) {
        float radius;

        if (i % 2 == 0) {
            radius = outerRadius;
        }
        else {
            radius = innerRadius;
        }

        float angle = PI / 2.0f + i * PI / 5.0f;

        glVertex2f(
            centerX + radius * cosf(angle),
            centerY + radius * sinf(angle)
        );
    }

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawStripes();
    drawStar();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 500);
    glutCreateWindow("Q20 - Procedural Striped Flag");

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}