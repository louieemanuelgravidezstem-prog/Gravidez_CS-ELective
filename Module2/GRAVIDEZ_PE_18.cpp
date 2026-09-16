#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void addPair(float x, float r, float g, float b) {
    glColor3f(r, g, b);
    glVertex2f(x, -0.35f);
    glVertex2f(x, 0.35f);
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUAD_STRIP);

    addPair(-0.90f, 1.0f, 0.0f, 0.0f);
    addPair(-0.54f, 1.0f, 0.0f, 0.0f);

    addPair(-0.54f, 0.0f, 1.0f, 0.0f);
    addPair(-0.18f, 0.0f, 1.0f, 0.0f);

    addPair(-0.18f, 0.0f, 0.3f, 1.0f);
    addPair(0.18f, 0.0f, 0.3f, 1.0f);

    addPair(0.18f, 1.0f, 1.0f, 0.0f);
    addPair(0.54f, 1.0f, 1.0f, 0.0f);

    addPair(0.54f, 1.0f, 0.0f, 1.0f);
    addPair(0.90f, 1.0f, 0.0f, 1.0f);

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 500);
    glutCreateWindow("Q18 - Alternating-Color Ribbon");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}