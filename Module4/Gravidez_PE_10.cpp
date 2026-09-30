#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat vertices[] = {
        -0.8f, -0.2f,  -0.4f, -0.2f,   0.0f, -0.2f,   0.4f, -0.2f,   0.8f, -0.2f,
        -0.8f,  0.2f,  -0.4f,  0.2f,   0.0f,  0.2f,   0.4f,  0.2f,   0.8f,  0.2f
    };

    GLubyte quad1[] = { 0, 1, 6, 5 };
    GLubyte quad2[] = { 1, 2, 7, 6 };
    GLubyte quad3[] = { 2, 3, 8, 7 };
    GLubyte quad4[] = { 3, 4, 9, 8 };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glColor3f(0.0f, 0.0f, 0.0f);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, quad1);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, quad2);

    glColor3f(0.0f, 0.0f, 0.0f);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, quad3);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, quad4);

    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q10 - Checkerboard Row via glDrawElements");

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}