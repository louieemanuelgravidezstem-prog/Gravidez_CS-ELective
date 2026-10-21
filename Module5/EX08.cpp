#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

GLuint leftVBO;
GLuint rightVBO;

void drawLeftTriangle() {
    GLfloat vertices[] = {
        -0.7f,  0.4f,  0.0f,
        -0.9f, -0.2f,  0.0f,
        -0.5f, -0.2f,  0.0f
    };

    glGenBuffers(1, &leftVBO);
    glBindBuffer(GL_ARRAY_BUFFER, leftVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(1.0f, 0.4f, 0.1f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawRightTriangle() {
    GLfloat vertices[] = {
         0.7f,  0.4f,  0.0f,
         0.5f, -0.2f,  0.0f,
         0.9f, -0.2f,  0.0f
    };

    glGenBuffers(1, &rightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glColor3f(0.1f, 0.4f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawLeftTriangle();
    drawRightTriangle();
    glDeleteBuffers(1, &leftVBO);
    glDeleteBuffers(1, &rightVBO);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("EX08 - Two Independent VBOs");

    glutDisplayFunc(display);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    glutMainLoop();
    return 0;
}