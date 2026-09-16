
#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

void drawSun() {
    const int segments = 40;

    glBegin(GL_TRIANGLE_FAN);

    // Yellow center
    glColor3f(1.0f, 0.9f, 0.2f);
    glVertex2f(0.55f, 0.55f);

    // Orange outer edge
    for (int i = 0; i <= segments; i++) {
        float angle =
            static_cast<float>(i) / segments
            * 2.0f * 3.14159265f;

        glColor3f(1.0f, 0.5f, 0.0f);

        glVertex2f(
            0.55f + 0.18f * cosf(angle),
            0.55f + 0.18f * sinf(angle)
        );
    }

    glEnd();
}

void drawMountains() {
    glBegin(GL_QUAD_STRIP);

    glColor3f(0.35f, 0.25f, 0.45f);
    glVertex2f(-1.0f, -0.2f);

    glColor3f(0.55f, 0.45f, 0.65f);
    glVertex2f(-1.0f, 0.1f);

    glColor3f(0.35f, 0.25f, 0.45f);
    glVertex2f(-0.3f, -0.2f);

    glColor3f(0.55f, 0.45f, 0.65f);
    glVertex2f(-0.3f, 0.3f);

    glColor3f(0.35f, 0.25f, 0.45f);
    glVertex2f(0.4f, -0.2f);

    glColor3f(0.55f, 0.45f, 0.65f);
    glVertex2f(0.4f, 0.15f);

    glColor3f(0.35f, 0.25f, 0.45f);
    glVertex2f(1.0f, -0.2f);

    glColor3f(0.55f, 0.45f, 0.65f);
    glVertex2f(1.0f, 0.25f);

    glEnd();
}

void drawGround() {
    glColor3f(0.15f, 0.5f, 0.2f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.0f, -0.2f);
    glVertex2f(1.0f, -0.2f);
    glVertex2f(1.0f, -1.0f);
    glEnd();
}

void drawHorizonLine() {
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    glVertex2f(-1.0f, -0.2f);
    glVertex2f(1.0f, -0.2f);
    glEnd();

    glDisable(GL_LINE_STIPPLE);
}

void display() {
    glClearColor(0.6f, 0.75f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawSun();
    drawMountains();
    drawGround();
    drawHorizonLine();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);

    glutCreateWindow("Ex20 - Mini Scene: Primitives + Color");
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}