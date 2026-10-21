#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cmath>
#include <cstdlib>

GLuint ringVBO;

void initializeRing() {
    GLfloat vertices[30 * 2 * 3];
    const GLfloat pi = 3.14159265f;

    for (int i = 0; i < 30; i++) {
        GLfloat angle = 2.0f * pi * i / 30.0f;

        GLfloat innerX = cosf(angle) * 0.6f;
        GLfloat innerY = sinf(angle) * 0.6f;
        GLfloat outerX = cosf(angle) * 0.85f;
        GLfloat outerY = sinf(angle) * 0.85f;

        int index = i * 6;

        vertices[index] = innerX;
        vertices[index + 1] = innerY;
        vertices[index + 2] = 0.0f;

        vertices[index + 3] = outerX;
        vertices[index + 4] = outerY;
        vertices[index + 5] = 0.0f;
    }

    glGenBuffers(1, &ringVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ringVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2f, 0.8f, 1.0f);
    glLineWidth(3.0f);

    glBindBuffer(GL_ARRAY_BUFFER, ringVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glDrawArrays(GL_LINES, 0, 60);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        glDeleteBuffers(1, &ringVBO);
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Q17 - Procedural Dash Ring");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        return 1;
    }
#endif

    initializeRing();
    glutMainLoop();

    return 0;
}