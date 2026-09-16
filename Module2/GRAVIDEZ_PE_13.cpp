#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(10.0f);

    glBegin(GL_LINES);

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(-0.8f, 0.0f);

    glColor3f(0.6f, 0.0f, 0.8f);
    glVertex2f(0.8f, 0.0f);

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 400);
    glutCreateWindow("Q13 - Yellow-to-Purple Gradient Line");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}