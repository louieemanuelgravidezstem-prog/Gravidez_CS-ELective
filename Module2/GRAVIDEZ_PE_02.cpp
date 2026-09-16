#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINES);

    glVertex2f(-0.8f, 0.5f);
    glVertex2f(-0.2f, 0.5f);

    glVertex2f(0.5f, 0.2f);
    glVertex2f(0.5f, -0.6f);

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q02 - Two Unconnected Lines");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}