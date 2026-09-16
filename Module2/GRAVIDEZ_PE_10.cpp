#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const int segments = 8;
    const float radius = 0.6f;
    const float pi = 3.14159265f;

    glColor3f(1.0f, 0.5f, 0.0f);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(0.0f, 0.0f);

    for (int i = 0; i <= segments; i++) {
        float angle = pi * static_cast<float>(i) / segments;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);

        glVertex2f(x, y);
    }

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q10 - Half-Circle Fan");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}