#define GL_SILENCE_DEPRECATION 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include <iostream>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    GLfloat drawArraysVertices[] = {
        -0.8f, -0.4f,
        -0.2f, -0.4f,
        -0.2f,  0.4f,

        -0.8f, -0.4f,
        -0.2f,  0.4f,
        -0.8f,  0.4f
    };

    glVertexPointer(2, GL_FLOAT, 0, drawArraysVertices);
    glColor3f(0.8f, 0.3f, 0.3f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    GLfloat drawElementsVertices[] = {
        0.2f, -0.4f,
        0.8f, -0.4f,
        0.8f,  0.4f,
        0.2f,  0.4f
    };

    GLubyte indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glVertexPointer(2, GL_FLOAT, 0, drawElementsVertices);
    glColor3f(0.3f, 0.7f, 0.4f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Exercise Q16 - glDrawArrays vs glDrawElements");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}