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
        -0.8f, -0.6f,  -0.8f, -0.4f,
        -0.4f, -0.6f,  -0.4f, -0.4f,
        -0.4f, -0.4f,  -0.4f, -0.2f,
         0.0f, -0.4f,   0.0f, -0.2f,
         0.0f, -0.2f,   0.0f,  0.0f,
         0.4f, -0.2f,   0.4f,  0.0f,
         0.4f,  0.0f,   0.4f,  0.2f,
         0.8f,  0.0f,   0.8f,  0.2f
    };

    GLfloat colors[] = {
        1.0f, 0.2f, 0.2f,  1.0f, 0.2f, 0.2f,
        1.0f, 0.2f, 0.2f,  1.0f, 0.2f, 0.2f,
        0.2f, 0.8f, 0.2f,  0.2f, 0.8f, 0.2f,
        0.2f, 0.8f, 0.2f,  0.2f, 0.8f, 0.2f,
        0.2f, 0.4f, 1.0f,  0.2f, 0.4f, 1.0f,
        0.2f, 0.4f, 1.0f,  0.2f, 0.4f, 1.0f,
        1.0f, 0.8f, 0.2f,  1.0f, 0.8f, 0.2f,
        1.0f, 0.8f, 0.2f,  1.0f, 0.8f, 0.2f
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_QUAD_STRIP, 0, 16);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q14 - Colored Quad Strip Staircase");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}