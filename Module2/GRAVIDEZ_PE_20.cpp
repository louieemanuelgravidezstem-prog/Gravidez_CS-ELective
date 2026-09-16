#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

const int SEGMENT_COUNT = 100;
const float PI = 3.14159265f;

void setRainbowColor(float t) {
    float red = 0.5f + 0.5f * sinf(2.0f * PI * t);
    float green = 0.5f + 0.5f * sinf(2.0f * PI * t + 2.0f * PI / 3.0f);
    float blue = 0.5f + 0.5f * sinf(2.0f * PI * t + 4.0f * PI / 3.0f);

    glColor3f(red, green, blue);
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const float radius = 0.75f;

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);

    for (int i = 0; i <= SEGMENT_COUNT; i++) {
        float t = static_cast<float>(i) / SEGMENT_COUNT;
        float angle = t * 2.0f * PI;

        setRainbowColor(t);

        glVertex2f(
            radius * cosf(angle),
            radius * sinf(angle)
        );
    }

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Q20 - Procedural Rainbow Fan");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}