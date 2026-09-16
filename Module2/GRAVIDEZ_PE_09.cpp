
#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.2f, 0.8f, 0.4f);

    glBegin(GL_TRIANGLE_STRIP);

    glVertex2f(-0.6f, -0.4f);
    glVertex2f(-0.6f, 0.4f);
    glVertex2f(0.6f, -0.4f);
    glVertex2f(0.6f, 0.4f);

    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q09 - Rectangle from Triangle Strip");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}