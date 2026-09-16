#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.8f, 0.2f, 0.7f);

    glBegin(GL_POLYGON);

    glVertex2f(-0.5f, 0.0f);
    glVertex2f(-0.25f, -0.45f);
    glVertex2f(0.25f, -0.45f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(0.25f, 0.45f);
    glVertex2f(-0.25f, 0.45f);

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q08 - Filled Hexagon");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}